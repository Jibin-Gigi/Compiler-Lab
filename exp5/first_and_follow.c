#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 10

char productions[MAX][MAX];
int productionCount;

char firstSet[MAX][MAX];
char followSet[MAX][MAX];
int firstCount[MAX];
int followCount[MAX];

// Check if a symbol is a non-terminal (uppercase letter)
int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// -------------------- FIND FIRST --------------------
void findFirst(char symbol, int index) {
    for (int i = 0; i < productionCount; i++) {
        if (productions[i][0] == symbol) {
            char next = productions[i][2];

            if (next == '#') {
                // Epsilon
                firstSet[index][firstCount[index]++] = '#';
            } 
            else if (!isNonTerminal(next)) {
                // Terminal symbol
                firstSet[index][firstCount[index]++] = next;
            } 
            else {
                // Non-terminal symbol: recursive call
                findFirst(next, index);
            }
        }
    }
}

// -------------------- FIND FOLLOW --------------------
void findFollow(char symbol, int index) {
    // Rule 1: Add $ to FOLLOW(start symbol)
    if (symbol == productions[0][0]) {
        followSet[index][followCount[index]++] = '$';
    }

    // Rule 2: Traverse all productions
    for (int i = 0; i < productionCount; i++) {
        for (int j = 2; productions[i][j] != '\0'; j++) {
            if (productions[i][j] == symbol) {
                char next = productions[i][j + 1];

                // Case 1: Next symbol is a terminal
                if (next != '\0' && !isNonTerminal(next)) {
                    followSet[index][followCount[index]++] = next;
                }

                // Case 2: Next symbol is a non-terminal
                else if (next != '\0' && isNonTerminal(next)) {
                    int nextIndex = -1;

                    // Find index of 'next' non-terminal
                    for (int k = 0; k < productionCount; k++) {
                        if (productions[k][0] == next) {
                            nextIndex = k;
                            break;
                        }
                    }

                    // Add FIRST(next) to FOLLOW(symbol), except epsilon
                    if (nextIndex != -1) {
                        for (int k = 0; k < firstCount[nextIndex]; k++) {
                            if (firstSet[nextIndex][k] != '#') {
                                followSet[index][followCount[index]++] = firstSet[nextIndex][k];
                            }
                        }
                    }
                }

                // Case 3: Symbol is at end of production
                else if (next == '\0') {
                    char lhs = productions[i][0];

                    if (lhs != symbol) {
                        // Find index of LHS non-terminal
                        int lhsIndex = -1;
                        for (int k = 0; k < productionCount; k++) {
                            if (productions[k][0] == lhs) {
                                lhsIndex = k;
                                break;
                            }
                        }

                        // Add FOLLOW(lhs) to FOLLOW(symbol)
                        if (lhsIndex != -1) {
                            findFollow(lhs, lhsIndex);
                            for (int k = 0; k < followCount[lhsIndex]; k++) {
                                followSet[index][followCount[index]++] = followSet[lhsIndex][k];
                            }
                        }
                    }
                }
            }
        }
    }
}

// -------------------- MAIN FUNCTION --------------------
int main() {
    printf("Enter number of production rules: ");
    scanf("%d", &productionCount);

    printf("Enter productions (example: E=TR):\n");
    for (int i = 0; i < productionCount; i++) {
        scanf("%s", productions[i]);
    }

    // Initialize counts
    for (int i = 0; i < MAX; i++) {
        firstCount[i] = 0;
        followCount[i] = 0;
    }

    // Compute FIRST sets
    for (int i = 0; i < productionCount; i++) {
        char nonTerminal = productions[i][0];
        findFirst(nonTerminal, i);
    }

    printf("\n--- FIRST sets ---\n");
    for (int i = 0; i < productionCount; i++) {
        char nonTerminal = productions[i][0];
        printf("FIRST(%c) = { ", nonTerminal);
        for (int j = 0; j < firstCount[i]; j++) {
            printf("%c ", firstSet[i][j]);
        }
        printf("}\n");
    }

    // Compute FOLLOW sets
    for (int i = 0; i < productionCount; i++) {
        char nonTerminal = productions[i][0];
        findFollow(nonTerminal, i);
    }

    printf("\n--- FOLLOW sets ---\n");
    for (int i = 0; i < productionCount; i++) {
        char nonTerminal = productions[i][0];
        printf("FOLLOW(%c) = { ", nonTerminal);
        for (int j = 0; j < followCount[i]; j++) {
            printf("%c ", followSet[i][j]);
        }
        printf("}\n");
    }

    return 0;
}
