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
    //printf("SO PHAN TU TRONG TOKEN %d\n",outputIndex);
    while ( index_post < outputIndex ){
        if (postfix[index_post].type == OPERAND ){
            arr[index_arr] = postfix[index_post].value.operand;
            //printf("GIA TRI CUA STACK HIEN TAI %f\n",arr[index_arr]);
            index_arr ++;
        }
        else if (postfix[index_post].type == VARIABLE){
            arr[index_arr] = x_value;
            //printf("GIA TRI CUA STACK HIEN TAI %f\n",arr[index_arr]);
            index_arr ++;
        }
        else if (postfix[index_post].type == OPERATOR){
            if (index_arr < 1){
                printf("Stack underflow\n");
                return NAN;
            }
        
            float a = arr[--index_arr];
            
            if (strcmp(postfix[index_post].value.operator, "+") == 0){
                float b = arr[--index_arr];
                arr[index_arr++] = b + a;
                //printf("GIA TRI CUA STACK HIEN TAI %f\n",b+a);
                //break;
                //continue;
            }
            else if (strcmp(postfix[index_post].value.operator, "-") == 0){
                float b = arr[--index_arr];
                arr[index_arr++] = b - a;
                //printf("GIA TRI CUA STACK HIEN TAI %f\n",arr[index_arr--]);
                //break;
                //continue;
            }
            else if (strcmp(postfix[index_post].value.operator, "*") == 0){
                float b = arr[--index_arr];
                arr[index_arr++] = b * a;
                //printf("GIA TRI CUA STACK HIEN TAI %f\n",arr[index_arr--]);
                //break;
                //continue;
            }
            else if (strcmp(postfix[index_post].value.operator, "/") == 0){
                if (a==0){
                    printf("Error because divide with 0");
                    return NAN;
                }
                float b = arr[--index_arr];
                arr[index_arr++] = b / a;
                //printf("GIA TRI CUA STACK HIEN TAI %f\n",arr[index_arr--]);
                //break;
                //continue;
            }

            else if (strcmp(postfix[index_post].value.operator, "^") == 0){
                float b = arr[--index_arr];
                arr[index_arr++] = pow(b,a);
                //printf("GIA TRI CUA STACK HIEN TAI %f\n",arr[index_arr--]);
                //break;
                //continue;
            }
            else if (strcmp(postfix[index_post].value.operator, "sin") == 0){
                arr[index_arr++] = sin(a);
                //printf("GIA TRI CUA STACK HIEN TAI %f\n",arr[index_arr--]);
                //break;
                //continue;
            }
            else if (strcmp(postfix[index_post].value.operator, "cos") == 0){
                //printf("CO HAM COS THUC HIEN cos(%f)= %f\n",a,cos(a));
                arr[index_arr++] = cos(a);
                //printf("GIA TRI CUA STACK HIEN TAI %f\n",arr[index_arr--]);
                //break;
                //continue;
            }
            else if (strcmp(postfix[index_post].value.operator, "tan") == 0){
                arr[index_arr++] = tan(a);
                //printf("GIA TRI CUA STACK HIEN TAI %f\n",arr[index_arr--]);
                //break;
                //continue;
            }                    
        }
        
        index_post ++;
    }
    return arr[0];
}