#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "token.h"
#include "global_variable.h"
#define MAX 100

typedef enum {S_START, S_OPERAND, S_OPERATOR, S_OPEN, S_CLOSE, S_ERROR, S_END} state_t;

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}



int isTrigoFunction(const char *str) {
    return (strcmp(str, "sin") == 0 || strcmp(str, "cos") == 0 || strcmp(str, "tan") == 0);
}

int precedence(const char *op) {
    if (strlen(op) == 1) {  // Nếu chỉ là một ký tự
        if (op[0] == '+' || op[0] == '-') return 1;
        if (op[0] == '*' || op[0] == '/') return 2;
        if (op[0] == '^') return 3;
    }
    if (isTrigoFunction(op)) return 4;  // Chỉ kiểm tra nếu là chuỗi dài hơn 1 ký tự
    return 0;
}


Token *infixToPostfix(char* myFunction){
    state_t current_state = S_START;
    Token *output = (Token *)malloc(MAX * sizeof(Token));
    char *stack[MAX];
    int stackTop = -1;
    outputIndex = 0;
    while (1){
        switch (current_state){
            
            case S_START:
                
                if (isdigit(*myFunction) || *myFunction == '.' || *myFunction == 'x') {
                    current_state = S_OPERAND;
                }
                else if (*myFunction == 's' || *myFunction == 'c' || *myFunction == 't' || isOperator(*myFunction)){
                    current_state = S_OPERATOR;
                }
                else if (*myFunction == '(') {
                    current_state = S_OPEN;
                }
                else if (*myFunction == 0) {
                    current_state = S_END;
                }
                else {
                    current_state = S_ERROR;
                }
                break;

            case S_OPERAND: {
                float operand = 0.0;
                int decimal_flag = 0;
                float decimal_divisor = 1.0;
                if (isdigit(*myFunction) || *myFunction == '.'){
                    while (isdigit(*myFunction) || *myFunction == '.') {
                        if (*myFunction == '.') {
                            decimal_flag = 1;
                        }
                        else {
                            if (decimal_flag == 0) {
                                operand = operand * 10 + (*myFunction - '0');
                            }
                            else {
                                decimal_divisor *= 10;
                                operand = operand + (*myFunction - '0') / decimal_divisor;
                            }
                        }
                        myFunction++;
                    }
                    output[outputIndex].type = OPERAND;
                    output[outputIndex].value.operand = operand;
                    outputIndex++;
                }
                else if (*myFunction == 'x') {
                    myFunction++;
                    output[outputIndex].type = VARIABLE;
                    output[outputIndex].value.variable = 0.0;
                    outputIndex++;                    
                }       
                if (isOperator(*myFunction)) {
                    current_state = S_OPERATOR;
                }
                else if (*myFunction == ')') {
                    current_state = S_CLOSE;
                }
                else if (*myFunction == 0) {
                    current_state = S_END;
                }
                else {
                    current_state = S_ERROR;
                }
                break;
            }

            case S_OPERATOR: {
                char operatorbuffer[5] = {0};  // Đủ chứa "atan", "sqrt"
                int opLen = 0;
                //printf("TOAN TU DANG XU LY: %c\n",*myFunction);
                if (isalpha(*myFunction)){
                    while (opLen < 3) {
                        operatorbuffer[opLen++] = *myFunction;
                        myFunction++; 
                    }
                    operatorbuffer[opLen] = '\0';  // Đảm bảo kết thúc chuỗi
                }
                //printf("TOAN TU LUONG GIAC: %s\n",operatorbuffer);
            
                if (opLen == 0) {  // Toán tử bình thường (+, -, *, /)
                    char currentOp[2] = { *myFunction, '\0' };
                    while (stackTop >= 0 && isOperator(stack[stackTop][0]) &&
                            ((precedence(stack[stackTop]) > precedence(currentOp)) ||
                            (precedence(stack[stackTop]) == precedence(currentOp) && currentOp[0] != '^'))) {

                        output[outputIndex].type = OPERATOR;
                        strcpy(output[outputIndex].value.operator, stack[stackTop]);
                        output[outputIndex].value.operator[sizeof(output[outputIndex].value.operator) - 1] = '\0';
                        outputIndex++;
                        free(stack[stackTop]);  // Giải phóng bộ nhớ trước khi giảm stack
                        stackTop--;
                    }
                    
                    stack[++stackTop] = (char*)malloc(2);  // 1 ký tự + '\0'
                    stack[stackTop][0] = *myFunction;      // Sao chép ký tự
                    stack[stackTop][1] = '\0';             // Kết thúc chuỗi

                    //printf("VALUE PUSH IN STACK %c\n",stack[stackTop]);
                }
            
                else if (isTrigoFunction(operatorbuffer)) {  // Hàm sin, cos, tan
                    char *trigoCopy = strdup(operatorbuffer);
                    if (trigoCopy == NULL) {
                        fprintf(stderr, "Memory allocation failed!\n");
                        exit(1);
                    }
                    stack[++stackTop] = trigoCopy;
                    continue;
                }
            
                if (*myFunction != '\0') {
                    myFunction++;
                }
            
                current_state = S_START;
                break;
            }
            
            case S_OPEN:
                //stack[++stackTop] = *myFunction;
                //stack[++stackTop] = strdup(myFunction);
                stack[++stackTop] = (char*)malloc(2);  // 1 ký tự + '\0'
                stack[stackTop][0] = *myFunction;      // Sao chép ký tự
                stack[stackTop][1] = '\0';

                myFunction++;
                current_state = S_START;
                break;

            
            case S_CLOSE:
                // Pop khỏi stack cho đến khi gặp '('
                while (stackTop >= 0 && strcmp(stack[stackTop], "(") != 0) {
                    output[outputIndex].type = OPERATOR;
                    strcpy(output[outputIndex].value.operator, stack[stackTop]);
                    outputIndex++;
                    free(stack[stackTop]);
                    stackTop--;
                }

                // Pop luôn '(' khỏi stack
                if (stackTop >= 0){
                    free(stack[stackTop]);
                    stackTop--;
                } 
                    

                // Nếu trên đỉnh stack là một hàm lượng giác thì pop nó vào output[]
                if (stackTop >= 0 && isTrigoFunction(stack[stackTop])) {
                    output[outputIndex].type = OPERATOR;
                    strcpy(output[outputIndex].value.operator, stack[stackTop]);
                    outputIndex++;
                    stackTop--;
                }

                myFunction++;

                // Chuyển trạng thái dựa vào ký tự tiếp theo
                if (isOperator(*myFunction)) {
                    current_state = S_OPERATOR;
                }
                else if (*myFunction == ')') {
                    current_state = S_CLOSE;
                }
                else if (*myFunction == 0) {
                    current_state = S_END;
                }
                else if (isdigit(*myFunction) || *myFunction == '.') {
                    current_state = S_OPERAND;
                }
                else {
                    current_state = S_ERROR;
                }
                break;

            case S_END:
                while (stackTop >= 0) {
                    output[outputIndex].type = OPERATOR;
                    strcpy(output[outputIndex].value.operator, stack[stackTop]); 
                    outputIndex++;
                    stackTop--;
                }
                output[outputIndex].type = OPERATOR;
                strcpy(output[outputIndex].value.operator, "E");
                outputIndex++;
                return output;
                break;

            case S_ERROR:
            default:
                printf("Input function error!!!\n");
                return NULL;
        }
    }
}

void printTokens(Token *output) {
    int i = 0;
    printf("Output Tokens: ");
    while (output[i].type != OPERATOR || strcmp(output[i].value.operator, "E") != 0) {

        if (output[i].type == OPERAND) {
            printf("%.2f ", output[i].value.operand);
        } else if (output[i].type == OPERATOR) {
            printf("%s ", output[i].value.operator);
        } else if (output[i].type == VARIABLE) {
            printf("x ");
        }
        i++;
    }
    printf("\n");
}

int findDegree(Token *output, int length) {
    int stack[MAX];  
    int top = -1;

    for (int i = 0; i < length; i++) {
        if (strcmp(output[i].value.operator, "E")!=0){
            if (output[i].type == OPERAND) {
                if (strcmp(output[i+1].value.operator, "^") == 0){
                    stack[++top] = output[i].value.operand;
                    //printf("PHIA SAO SO NAY CO DAU MU\n");
                    //stack[++top] = output[i].value.operand;
                }
                // Số thực có bậc 0
                else stack[++top] = 0;
            } 
            else if (output[i].type == VARIABLE) {
                // Biến (x) có bậc 1
                stack[++top] = 1;
            } 
            else if (output[i].type == OPERATOR) {
                if (top < 1) {  
                    //printf("Lỗi: Stack không đủ phần tử cho toán tử %s.\n", output[i].value.operator);
                    return -1;
                }
    
                if (strcmp(output[i].value.operator, "^") == 0) {
                    int exponent = stack[top--];  // Bậc của số mũ
                    int base = stack[top];        // Bậc của cơ số
                    //printf("BASE = %d, exponent = %d\n",base,exponent);
                    if (exponent < 0) {
                        //printf("Lỗi: Không hỗ trợ số mũ âm.\n");
                        return -1;
                    }
    
                    // Nếu cơ số là biến (x), nhân bậc với số mũ
                    stack[top] = base * exponent;
                    //printf("BAC DANG CO TRONG STACK %d\n",stack[top]); 
                } 
                else if (strcmp(output[i].value.operator, "*") == 0) {
                    int operand1 = stack[top--];
                    int operand2 = stack[top];
                    stack[top] = operand1 + operand2;  // Nhân thì cộng bậc
                } 
                else if (strcmp(output[i].value.operator, "+") == 0 || strcmp(output[i].value.operator, "-") == 0) {
                    int operand1 = stack[top--];
                    int operand2 = stack[top];
                    stack[top] = operand1 > operand2 ? operand1 : operand2;  // Cộng/trừ lấy bậc lớn nhất
                }
                else {
                    //printf("Lỗi: Toán tử không hợp lệ [%s]\n", output[i].value.operator);
                    return -1;
                }
            }
        }
        
    }

    return (top >= 0) ? stack[top] : -1;
}