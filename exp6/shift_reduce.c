#include <stdio.h>
#include <string.h>

// Global variables
int stackTop = 0, inputIndex = 0, totalLen = 0;
char input[50], stackStr[50], action[20];

void reduce();

int main() {
    printf("GRAMMAR:\n");
    printf("E -> E + E\n");
    printf("E -> E * E\n");
    printf("E -> (E)\n");
    printf("E -> id\n\n");

    printf("Enter input string: ");
    scanf("%s", input);
    totalLen = strlen(input);

    strcpy(action, "SHIFT->");

    printf("\n%-10s%-10s%-10s\n", "STACK", "INPUT", "ACTION");

    for (inputIndex = 0; inputIndex < totalLen; inputIndex++, stackTop++) {
        if (input[inputIndex] == 'i' && input[inputIndex + 1] == 'd') {
            // Handle "id" as a single token
            stackStr[stackTop] = input[inputIndex];
            stackStr[stackTop + 1] = input[inputIndex + 1];
            stackStr[stackTop + 2] = '\0';

            input[inputIndex] = ' ';
            input[inputIndex + 1] = ' ';
            inputIndex++; // Skip next character ('d')

            printf("\n$%-8s%-10s$%s id", stackStr, input, action);
            reduce();
        } else {
            // Handle single symbols like +, *, (, )
            stackStr[stackTop] = input[inputIndex];
            stackStr[stackTop + 1] = '\0';
            input[inputIndex] = ' ';
            printf("\n$%-8s%-10s$%s symbol", stackStr, input, action);
            reduce();
        }
    }
    return 0;
}

// Reduction logic
void reduce() {
    char reduceMsg[] = "REDUCE TO E";

    // Rule: id → E
    for (int k = 0; k < totalLen; k++) {
        if (stackStr[k] == 'i' && stackStr[k + 1] == 'd') {
            stackStr[k] = 'E';
            stackStr[k + 1] = '\0';
            printf("\n$%-8s%-10s$%s", stackStr, input, reduceMsg);
        }
    }

    // Rule: E + E → E
    for (int k = 0; k < totalLen; k++) {
        if (stackStr[k] == 'E' && stackStr[k + 1] == '+' && stackStr[k + 2] == 'E') {
            stackStr[k] = 'E';
            stackStr[k + 1] = '\0';
            stackStr[k + 2] = '\0';
            printf("\n$%-8s%-10s$%s", stackStr, input, reduceMsg);
            stackTop -= 2;
        }
    }

    // Rule: E * E → E
    for (int k = 0; k < totalLen; k++) {
        if (stackStr[k] == 'E' && stackStr[k + 1] == '*' && stackStr[k + 2] == 'E') {
            stackStr[k] = 'E';
            stackStr[k + 1] = '\0';
            stackStr[k + 2] = '\0';
            printf("\n$%-8s%-10s$%s", stackStr, input, reduceMsg);
            stackTop -= 2;
        }
    }

    // Rule: (E) → E
    for (int k = 0; k < totalLen; k++) {
        if (stackStr[k] == '(' && stackStr[k + 1] == 'E' && stackStr[k + 2] == ')') {
            stackStr[k] = 'E';
            stackStr[k + 1] = '\0';
            stackStr[k + 2] = '\0';
            printf("\n$%-8s%-10s$%s", stackStr, input, reduceMsg);
            stackTop -= 2;
        }
    }
}
