#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "token.h"
#include "solve_equation.h"
#include "evaluate.h"
#include "global_variable.h"

float evaluatePostfix(Token *postfix, float x_value) {
    int index_post = 0;
    int index_arr = 0;
    float arr[100];

    while ( index_post < outputIndex ){
        if (postfix[index_post].type == OPERAND ){
            arr[index_arr] = postfix[index_post].value.operand;
            index_arr ++;
        }
        else if (postfix[index_post].type == VARIABLE){
            arr[index_arr] = x_value;
            index_arr ++;
        }
        else if (postfix[index_post].type == OPERATOR){
            if (index_arr < 1){
                printf("Stack underflow\n");
                return NAN;
            }
        
            
            float a = arr[--index_arr];
            float b = arr[--index_arr];
            switch (postfix[index_post].value.operator)
            {
            case '+':
                arr[index_arr++] = b + a;
                break;

            case '-':
                arr[index_arr++] = b - a;
                break;
            
            case '*':
                arr[index_arr++] = b * a;
                break;

            case '/':
                if (a==0){
                    printf("Error because divide with 0");
                    return NAN;
                }
            
                arr[index_arr++] = b / a;
                break;
                
            case '^':
                arr[index_arr++] = pow(b,a);
                break;    
            
            }
        }
        index_post ++;
    } 
    return arr[0];
}