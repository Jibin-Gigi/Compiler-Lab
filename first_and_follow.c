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

// Function declarations
// Check if a symbol is a non-terminal (uppercase)
int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}


// FIND FIRST SET
void findFirst(char symbol, int index) {
    for (int i = 0; i < productionCount; i++) {
        if (productions[i][0] == symbol) {
            char next = productions[i][2];
            if (!isNonTerminal(next)) {
                // If terminal, add directly
                firstSet[index][firstCount[index]++] = next;
            } else if (next == '#') {
                firstSet[index][firstCount[index]++] = '#';
            } else {
                // Recursive call for non-terminal
                findFirst(next, index);
            }
        }
    }
}


// FIND FOLLOW SET
void findFollow(char symbol) {
    // If symbol is start symbol, add $
    if (symbol == productions[0][0]) {
        followSet[symbol - 'A'][followCount[symbol - 'A']++] = '$';
    }

    for (int i = 0; i < productionCount; i++) {
        for (int j = 2; productions[i][j] != '\0'; j++) {
            if (productions[i][j] == symbol) {
                char next = productions[i][j + 1];
                if (next != '\0') {
                    if (!isNonTerminal(next)) {
                        followSet[symbol - 'A'][followCount[symbol - 'A']++] = next;
                    } else {
                        // Add FIRST(next) to FOLLOW(symbol)
                        int ntIndex = next - 'A';
                        for (int k = 0; k < firstCount[ntIndex]; k++) {
                            if (firstSet[ntIndex][k] != '#') {
                                followSet[symbol - 'A'][followCount[symbol - 'A']++] = firstSet[ntIndex][k];
                            }
                        }
                    }
                } else {
                    // End of production: FOLLOW(lhs) → FOLLOW(symbol)
                    char lhs = productions[i][0];
                    if (lhs != symbol) {
                        findFollow(lhs);
                        int lhsIndex = lhs - 'A';
                        for (int k = 0; k < followCount[lhsIndex]; k++) {
                            followSet[symbol - 'A'][followCount[symbol - 'A']++] = followSet[lhsIndex][k];
                        }
                    }
                }
            }
        }
    }
}


// Global variables used while building sets
int firstIndex = 0;
int followIndex = 0;

//-----------------------------------------------
int main() {
    printf("Enter number of production rules: ");
    scanf("%d", &productionCount);

    printf("Enter the productions (example: E=TR):\n");
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
        findFollow(nonTerminal);
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
