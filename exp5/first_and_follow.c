#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10

char productions[MAX][MAX];
int productionCount;

char nonTerminals[MAX];
int nonTerminalCount = 0;

char firstSet[MAX][MAX];
char followSet[MAX][MAX];
int firstCount[MAX];
int followCount[MAX];
int visitedFollow[MAX];

// Helper to check if a symbol is a non-terminal
int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Avoid duplicates
void addToSet(char set[MAX][MAX], int index, char symbol, int *count) {
    for (int i = 0; i < *count; i++)
        if (set[index][i] == symbol) return;
    set[index][(*count)++] = symbol;
}

// -------------------- FIND FIRST --------------------
void findFirst(char symbol, int index) {
    for (int i = 0; i < productionCount; i++) {
        if (productions[i][0] == symbol) {
            // Start after '='
            for (int j = 2; productions[i][j] != '\0'; j++) {
                char next = productions[i][j];

                if (next == '#') {
                    addToSet(firstSet, index, '#', &firstCount[index]);
                    break;
                } else if (!isNonTerminal(next)) {
                    addToSet(firstSet, index, next, &firstCount[index]);
                    break;
                } else {
                    // Non-terminal — find its FIRST and merge
                    int nextIndex = -1;
                    for (int k = 0; k < nonTerminalCount; k++) {
                        if (nonTerminals[k] == next) {
                            nextIndex = k;
                            break;
                        }
                    }
                    if (nextIndex != -1) {
                        findFirst(next, nextIndex);
                        for (int k = 0; k < firstCount[nextIndex]; k++) {
                            addToSet(firstSet, index, firstSet[nextIndex][k], &firstCount[index]);
                        }
                    }

                    // Stop unless epsilon was found
                    int hasEpsilon = 0;
                    for (int k = 0; k < firstCount[nextIndex]; k++) {
                        if (firstSet[nextIndex][k] == '#') {
                            hasEpsilon = 1;
                            break;
                        }
                    }
                    if (!hasEpsilon) break;
                }
            }
        }
    }
}

// -------------------- FIND FOLLOW --------------------
void findFollow(char symbol, int index) {
    if (visitedFollow[index]) return;
    visitedFollow[index] = 1;

    if (symbol == productions[0][0])
        addToSet(followSet, index, '$', &followCount[index]);

    for (int i = 0; i < productionCount; i++) {
        for (int j = 2; productions[i][j] != '\0'; j++) {
            if (productions[i][j] == symbol) {
                char next = productions[i][j + 1];

                if (next != '\0' && !isNonTerminal(next))
                    addToSet(followSet, index, next, &followCount[index]);

                else if (next != '\0' && isNonTerminal(next)) {
                    int nextIndex = -1;
                    for (int k = 0; k < nonTerminalCount; k++)
                        if (nonTerminals[k] == next) nextIndex = k;

                    for (int k = 0; k < firstCount[nextIndex]; k++)
                        if (firstSet[nextIndex][k] != '#')
                            addToSet(followSet, index, firstSet[nextIndex][k], &followCount[index]);
                }

                else if (next == '\0') {
                    char lhs = productions[i][0];
                    if (lhs != symbol) {
                        int lhsIndex = -1;
                        for (int k = 0; k < nonTerminalCount; k++)
                            if (nonTerminals[k] == lhs) lhsIndex = k;

                        findFollow(lhs, lhsIndex);
                        for (int k = 0; k < followCount[lhsIndex]; k++)
                            addToSet(followSet, index, followSet[lhsIndex][k], &followCount[index]);
                    }
                }
            }
        }
    }
}

// -------------------- MAIN --------------------
int main() {
    printf("Enter number of production rules: ");
    scanf("%d", &productionCount);

    printf("Enter productions (example: E=TR):\n");
    for (int i = 0; i < productionCount; i++) {
        scanf("%s", productions[i]);
        char lhs = productions[i][0];
        int exists = 0;
        for (int j = 0; j < nonTerminalCount; j++)
            if (nonTerminals[j] == lhs) exists = 1;
        if (!exists) nonTerminals[nonTerminalCount++] = lhs;
    }

    memset(firstCount, 0, sizeof(firstCount));
    memset(followCount, 0, sizeof(followCount));
    memset(visitedFollow, 0, sizeof(visitedFollow));

    for (int i = 0; i < nonTerminalCount; i++)
        findFirst(nonTerminals[i], i);

    printf("\n--- FIRST sets ---\n");
    for (int i = 0; i < nonTerminalCount; i++) {
        printf("FIRST(%c) = { ", nonTerminals[i]);
        for (int j = 0; j < firstCount[i]; j++)
            printf("%c ", firstSet[i][j]);
        printf("}\n");
    }

    for (int i = 0; i < nonTerminalCount; i++)
        findFollow(nonTerminals[i], i);

    printf("\n--- FOLLOW sets ---\n");
    for (int i = 0; i < nonTerminalCount; i++) {
        printf("FOLLOW(%c) = { ", nonTerminals[i]);
        for (int j = 0; j < followCount[i]; j++)
            printf("%c ", followSet[i][j]);
        printf("}\n");
    }

    return 0;
}
