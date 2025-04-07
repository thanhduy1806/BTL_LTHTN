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


typedef enum {INPUT, OPTION, CALC, SOLVE, EVALUATE} state;


int main(){
    state in_state = INPUT;
    Token *output;
    float x;
    char str[MAX];
    char status;
    while(1){
        switch(in_state){
            case INPUT: 
                // Giải phóng output cũ nếu có
                if (output != NULL) {
                    free(output);
                    output = NULL;
                }
                printf("nhập biểu thức: ");
                fgets(str, MAX, stdin);
                str[strcspn(str, "\n")] = 0;
                output = infixToPostfix(str);
                printTokens(output);
                

                do{
                    printf("NHAN PHIM = DE CHON OPTION ");
                    scanf(" %c",&status);
                    while (getchar() != '\n');
                }
                while(status != '=');
                in_state = OPTION;
                break;

            case OPTION:
                printf("NHAN PHIM 1 DE TIM NGHIEM, PHIM 2 DE TINH GIA TRI ");
                scanf(" %c",&status);
                while (getchar() != '\n');
                if (status == '1') in_state = SOLVE;
                else if (status == '2') in_state = CALC
                ;
                break;
            
            case CALC:
                printf("NHAP VAO x");
                scanf("%f",&x);
                while (getchar() != '\n');
                in_state = EVALUATE;
                break;
            case EVALUATE:
                if (output != NULL) {
                    printTokens(output);
                    float result = evaluatePostfix(output, x);
                    printf("Giá trị của biểu thức với x = %.2f là: %.2f\n", x, result);
                }
                do{
                    printf("NHAN 0 DE VE BUOC NHAP PHUONG TRINH ");
                    scanf(" %c",&status);
                    while (getchar() != '\n');
                }
                while(status != '0');
                in_state = INPUT;
                break;

            case SOLVE:
                printf("NGHIEM LAGURRE %f\n",(Laguerre(output,degree,1)));
                printf("NGHIEM NEWRAPHSON %f\n",(newtonRaphson(output,-1,degree)));
                printf("NGHIEM BISECTION %f\n",bisectionMethod(output,-20,20));
                printf("NGHIEM BRENT %lf\n",brent(output,-10,10,1e-6));
                printf("NGHIEM SECANT %f",secantMethod(output));
                do{
                    printf("\nNHAN 0 DE VE BUOC NHAP PHUONG TRINH ");
                    scanf(" %c",&status);
                    while (getchar() != '\n');
                }
                while(status != '0');
                in_state = INPUT;
                break;

        }
    }
    return 0;

}







// int main(){
//     Token *output;
//     float x;
//     char str[MAX];
//     printf("nhập biểu thức: ");
//     fgets(str, MAX, stdin);
//     str[strcspn(str, "\n")] = 0;

    
//     printf("Nhập giá trị x: ");
//     scanf("%f", &x);

//     degree = findDegree(output, outputIndex);

//     printf("Bậc của đa thức là: %d\n", degree);
//     if (output != NULL) {
//         printTokens(output);
//         float result = evaluatePostfix(output, x);
//         printf("Giá trị của biểu thức với x = %.2f là: %.2f\n", x, result);
//     }
//     printf("NGHIEM LAGURRE %f\n",(Laguerre(output,degree,1)));
    

//     // FindAllRoots_Bisection(output);
//     // for (int i = 0; i<count_roots;i++){
//     //     if (type_root[i]=='s'){
//     //         printf("NGHIEM DON Bisec x%d: %.2f\n",i,roots[i]);
//     //     }
//     //     if (type_root[i]=='d'){
//     //         printf("NGHIEM KEP Bisec x%d: %.2f\n",i,roots[i]);
//     //     }
//     // }
//     printf("NGHIEM NEWRAPHSON %f\n",(newtonRaphson(output,-1,degree)));
//     printf("NGHIEM BISECTION %f\n",bisectionMethod(output,-20,20));
//     printf("NGHIEM BRENT %lf\n",brent(output,-10,10,1e-6));
//     printf("NGHIEM SECANT %f",secantMethod(output));

//     // for (int i = 0; i<count_roots;i++){
//     //     if (type_root[i]=='s'){
//     //         printf("NGHIEM DON x%d: %.2f\n",i,roots[i]);
//     //     }
//     //     if (type_root[i]=='d'){
//     //         printf("NGHIEM KEP x%d: %.2f\n",i,roots[i]);
//     //     }
//     // }
//     printf("SO LUONG NGHIEM %d\n",count_roots);
    
//     return 0;
// }