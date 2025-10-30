#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// Global variables
int expression_index = 1, operator_count = 0, assignment_no = 0; // 'expression_index' starts at 1 for processing
char expression_str[100]; // Stores the input expression (e.g., "w:a+b*c+d")
char left_operand[15];  // To store the left operand of a subexpression
char right_operand[15]; // To store the right operand of a subexpression
char temp_var_start = 'Z'; // ASCII for 'Z' (90), used for temporary variables (Z, Y, X, ...)

// Structure to hold operator information (position and the operator itself)
struct operator_info {
    int pos;  // Position of the operator in the expression string
    char op;  // The operator character (+, -, *, /, :)
} operator_list[15]; // Array to store all operators and their positions

// Function prototypes
void find_operators();
void generate_code();
void get_left_operand(int position);
void get_right_operand(int position);

void main() {
    printf("\t\tINTERMEDIATE CODE GENERATION\n\n");
    printf("Enter the Expression :");
    scanf("%s", expression_str);
    printf("The intermediate code:\n");
    
    // Step 1: Find all operators and store them in precedence order
    find_operators(); 
    
    // Step 2: Generate intermediate code (Three-Address Code)
    generate_code(); 
}

// Function to find all operators in the expression and store them in the 'operator_list' 
// following a specific precedence order (Assignment, Division, Multiplication, Addition, Subtraction)
void find_operators() {
    // 1. Assignment (:)
    for (int i = 0; expression_str[i] != '\0'; i++)
        if (expression_str[i] == ':') {
            operator_list[operator_count].pos = i;
            operator_list[operator_count++].op = ':';
        }
    // 2. Division (/)
    for (int i = 0; expression_str[i] != '\0'; i++)
        if (expression_str[i] == '/') {
            operator_list[operator_count].pos = i;
            operator_list[operator_count++].op = '/';
        }
    // 3. Multiplication (*)
    for (int i = 0; expression_str[i] != '\0'; i++)
        if (expression_str[i] == '*') {
            operator_list[operator_count].pos = i;
            operator_list[operator_count++].op = '*';
        }
    // 4. Addition (+)
    for (int i = 0; expression_str[i] != '\0'; i++)
        if (expression_str[i] == '+') {
            operator_list[operator_count].pos = i;
            operator_list[operator_count++].op = '+';
        }
    // 5. Subtraction (-)
    for (int i = 0; expression_str[i] != '\0'; i++)
        if (expression_str[i] == '-') {
            operator_list[operator_count].pos = i;
            operator_list[operator_count++].op = '-';
        }
    // Note: The precedence order in this implementation is hardcoded by the loop order.
    // The loop order is: ':', '/', '*', '+', '-'
}

// Function to generate the three-address code
void generate_code() {
    expression_index = 1; // Start from the second operator (index 1) as index 0 is typically the main assignment
    
    // Process all operators except the final assignment
    while (operator_list[expression_index].op != '\0') {
        // Get the operands for the current operator
        get_left_operand(operator_list[expression_index].pos);
        get_right_operand(operator_list[expression_index].pos);
        
        // Replace the subexpression with a temporary variable (Z, Y, X, ...)
        expression_str[operator_list[expression_index].pos] = temp_var_start--;
        
        // Print the three-address code for the subexpression
        printf("\t%c := %s%c%s\t\t", 
               expression_str[operator_list[expression_index].pos], 
               left_operand, 
               operator_list[expression_index].op, 
               right_operand);
        printf("\n");
        
        expression_index++;
    }
    
    // Final assignment code generation
    get_right_operand(-1); // Get the variable on the right side of the final assignment (or the last temp variable)
    
    if (assignment_no == 0) { // If there was only one operation (or no assignment operator)
        get_left_operand(strlen(expression_str)); // Get the variable on the left side (or the first operand)
        printf("\t%s := %s", right_operand, left_operand); // This case handles simple assignment or single op
        exit(0);
    }
    
    // The main assignment: target := result_of_last_subexpression
    // Note: The original code uses 'right_operand' for the final target variable.
    printf("\t%s := %c", right_operand, expression_str[operator_list[--expression_index].pos]);
}

// Function to get the left operand of a subexpression
void get_left_operand(int position) {
    int w = 0, flag = 0;
    position--; // Start searching left of the operator
    
    // Search backward until an operator or end of string is found
    while (position != -1 && expression_str[position] != '+' && expression_str[position] != '*' && 
           expression_str[position] != '=' && expression_str[position] != '\0' && 
           expression_str[position] != '-' && expression_str[position] != '/' && 
           expression_str[position] != ':') {
        
        // If the character is not already marked as processed ('$') and we haven't found the operand yet
        if (expression_str[position] != '$' && flag == 0) {
            left_operand[w++] = expression_str[position]; // Store the operand
            left_operand[w] = '\0';
            expression_str[position] = '$'; // Mark the character as processed
            flag = 1; // Indicate that the operand has been found
        }
        position--;
    }
}

// Function to get the right operand of a subexpression
void get_right_operand(int position) {
    int w = 0, flag = 0;
    position++; // Start searching right of the operator
    
    // Search forward until an operator or end of string is found
    while (position != -1 && expression_str[position] != '+' && expression_str[position] != '*' && 
           expression_str[position] != '\0' && expression_str[position] != '=' && 
           expression_str[position] != ':' && expression_str[position] != '-' && 
           expression_str[position] != '/') {
        
        // If the character is not already marked as processed ('$') and we haven't found the operand yet
        if (expression_str[position] != '$' && flag == 0) {
            right_operand[w++] = expression_str[position]; // Store the operand
            right_operand[w] = '\0';
            expression_str[position] = '$'; // Mark the character as processed
            flag = 1; // Indicate that the operand has been found
        }
        position++;
    }
}
/*
		INTERMEDIATE CODE GENERATION

Enter the Expression :w:a+b*c+d
The intermediate code:
	Z := b*c		
	Y := a+Z		
	X := Y+d		
	w := X
*/
