#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "token.h"
#include "solve_equation.h"
#include "global_variable.h"
#include "evaluate.h"
#define MAX 100

/*
S_START: Trạng thái bắt đầu
S_OPERAND: Trạng thái nhận toán hạng
S_OPERATOR: Trạng thái nhận toán tử
S_OPEN: Trạng thái nhận dấu mở ngoặc
S_CLOSE: Trạng thái nhận dấu đóng ngoặc
S_ERROR: Trạng thái lỗi
S_END: Trạng thái kết thúc
*/
typedef enum {S_START, S_OPERAND, S_OPERATOR, S_OPEN, S_CLOSE, S_ERROR, S_END} state_t;


int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int precedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}



Token *infixToPostfix(char* myFunction){
    state_t current_state = S_START;
    Token *output = (Token *)malloc(MAX * sizeof(Token));
    
    char stack[MAX];
    int stackTop = -1;
    outputIndex = 0;
    while (1){
        switch (current_state){
            case S_START:
                if (isdigit(*myFunction) || *myFunction == '.' || *myFunction == 'x') {
                    current_state = S_OPERAND;
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

            case S_OPERATOR:
                while (stackTop >= 0 && isOperator(stack[stackTop]) && 
                       precedence(stack[stackTop]) >= precedence(*myFunction)) {
                    output[outputIndex].type = OPERATOR;
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                stack[++stackTop] = *myFunction;
                myFunction++;
                current_state = S_START;
                break;

            case S_OPEN:
                stack[++stackTop] = *myFunction;
                myFunction++;
                current_state = S_START;
                break;

            case S_CLOSE:
                while (stackTop >= 0 && stack[stackTop] != '(') {
                    output[outputIndex].type = OPERATOR;
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                if (stackTop >= 0) stackTop--; 
                myFunction++;
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
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                output[outputIndex].type = OPERATOR;
                output[outputIndex].value.operator = 'E';
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


/*
Hàm này tính giá trị khi thay x
*/

// float evaluatePostfix(Token *postfix, float x_value) {
//     int index_post = 0;
//     int index_arr = 0;
//     float arr[100];

//     while ( index_post < outputIndex ){
//         if (postfix[index_post].type == OPERAND ){
//             arr[index_arr] = postfix[index_post].value.operand;
//             index_arr ++;
//         }
//         else if (postfix[index_post].type == VARIABLE){
//             arr[index_arr] = x_value;
//             index_arr ++;
//         }
//         else if (postfix[index_post].type == OPERATOR){
//             if (index_arr < 1){
//                 printf("Stack underflow\n");
//                 return NAN;
//             }
        
            
//             float a = arr[--index_arr];
//             float b = arr[--index_arr];
//             switch (postfix[index_post].value.operator)
//             {
//             case '+':
//                 arr[index_arr++] = b + a;
//                 break;

//             case '-':
//                 arr[index_arr++] = b - a;
//                 break;
            
//             case '*':
//                 arr[index_arr++] = b * a;
//                 break;

//             case '/':
//                 if (a==0){
//                     printf("Error because divide with 0");
//                     return NAN;
//                 }
            
//                 arr[index_arr++] = b / a;
//                 break;
                
//             case '^':
//                 arr[index_arr++] = pow(b,a);
//                 break;    
            
//             }
//         }
//         index_post ++;
//     } 
//     return arr[0];
// }







void printTokens(Token *output) {
    int i = 0;
    printf("Output Tokens: ");
    while (output[i].type != OPERATOR || output[i].value.operator != 'E') {
        if (output[i].type == OPERAND) {
            printf("%.2f ", output[i].value.operand);
        } else if (output[i].type == OPERATOR) {
            printf("%c ", output[i].value.operator);
        } else if (output[i].type == VARIABLE) {
            printf("x ");
        }
        i++;
    }
    printf("\n");
}

int main(){
    Token *output;
    float x;
    char str[MAX];
    printf("nhập biểu thức: ");
    fgets(str, MAX, stdin);
    str[strcspn(str, "\n")] = 0;

    output = infixToPostfix(str);
    printf("Nhập giá trị x: ");
    scanf("%f", &x);

    if (output != NULL) {
        printTokens(output);
        float result = evaluatePostfix(output, x);
        printf("Giá trị của biểu thức với x = %.2f là: %.2f\n", x, result);
    }

    float no = secantMethod(output);
    printf("NGHIEM CUA PHUONG TRINH: %.2f\n",no);

    FindAllRoot(output);
    for (int i = 0; i<count_roots;i++){
        if (type_root[i]=='s'){
            printf("NGHIEM DON x%d: %.2f\n",i,roots[i]);
        }
        if (type_root[i]=='d'){
            printf("NGHIEM KEP x%d: %.2f\n",i,roots[i]);
        }
    }
    printf("CHAY DUOC FIND ALL ROOT");
    return 0;
}