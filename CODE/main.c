#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#include "token.h"
#include "solve_equation.h"
#include "global_variable.h"
#include "evaluate.h"
#define MAX 100
#define NUM_THREAD 4

typedef enum {INPUT, OPTION, CALC, SOLVE, EVALUATE} state;

typedef struct{
    Token *data;
    float result;
    int degree;
} ThreadData;

int flag;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
float best_result = 0;
Token *output;

//5 luong cua 5 ham tiem nghiem
// void* laguerre_thread(void* arg) {
//     ThreadData* args = (ThreadData*)arg;
//     args->result = Laguerre(args->data, args->degree, -2);
//     pthread_mutex_lock(&mutex);
//     if (flag == 0 && !isnan(args->result)){
//         flag = 1;   
//         best_result = args->result;
//         printf("NGHIEM TU LAGUERRE %f",best_result);
//     }
//     pthread_mutex_unlock(&mutex);
//     pthread_exit(NULL);
// }

void* newton_thread(void* arg) {
    ThreadData* args = (ThreadData*)arg;
    args->result = newtonRaphson(args->data, 1, args->degree);
    pthread_mutex_lock(&mutex);
    if (flag == 0 && !isnan(args->result)){
        flag = 1;
        best_result = args->result;
        printf("NGHIEM TU NEWTON %f",best_result);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* bisection_thread(void* arg) {
    ThreadData* args = (ThreadData*)arg;
    args->result = bisectionMethod(args->data, -10, 10);
    pthread_mutex_lock(&mutex);
    if (flag == 0 && !isnan(args->result)){
        flag = 1;
        best_result = args->result;
        printf("NGHIEM TU BISECTION %f",best_result);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* brent_thread(void* arg) {
    ThreadData* args = (ThreadData*)arg;
    args->result = brent(args->data, -10, 10, 1e-6);
    pthread_mutex_lock(&mutex);
    if (flag == 0 && !isnan(args->result)){
        flag = 1;
        best_result = args->result;
        printf("NGHIEM TU BRENT %f",best_result);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* secant_thread(void* arg) {
    ThreadData* args = (ThreadData*)arg;
    args->result = secantMethod(args->data);
    pthread_mutex_lock(&mutex);
    if (flag == 0 && !isnan(args->result)){
        flag = 1;
        best_result = args->result;
        printf("NGHIEM TU SCANT %f",best_result);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}




pthread_t threads[NUM_THREAD];
ThreadData args[NUM_THREAD];



void create_thread(){
    //pthread_create(&threads[0], NULL, laguerre_thread, &args[4]);
    pthread_create(&threads[1], NULL, newton_thread, &args[0]);
    pthread_create(&threads[2], NULL, bisection_thread, &args[1]);
    pthread_create(&threads[3], NULL, brent_thread, &args[2]);
    pthread_create(&threads[4], NULL, secant_thread, &args[3]); 
}
    

int main(){
    state in_state = INPUT;
    float x;
    char str[MAX];
    char status;
    struct timespec start, end;
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

                degree = findDegree(output, outputIndex);

                
                do{
                    printf("NHAN PHIM = DE CHON OPTION ");
                    scanf("%c",&status);
                    while (getchar() != '\n');
                }
                
                while(status != '=');
                in_state = OPTION;
                break;

            case OPTION:
                printf("NHAN PHIM 1 DE TIM NGHIEM, PHIM 2 DE TINH GIA TRI ");
                scanf("%c",&status);
                while (getchar() != '\n');
                if (status == '1') in_state = SOLVE;
                else if (status == '2') in_state = CALC;
                break;
            
            case CALC:
                printf("NHAP VAO x ");
                scanf("%f",&x);
                while (getchar() != '\n');
                in_state = EVALUATE;
                break;
            case EVALUATE:
                if (output != NULL) {
                    printTokens(output);
                    float result = evaluatePostfix(output, x);
                    printf("Giá trị của biểu thức với x = %.5f là: %.5f\n", x, result);
                }
                do{
                    printf("NHAN 0 DE VE BUOC NHAP PHUONG TRINH ");
                    scanf("%c",&status);
                    while (getchar() != '\n');
                }
                while(status != '0');
                in_state = INPUT;
                break;

            case SOLVE:
                // Gán lại dữ liệu đầu vào cho chắc
                for (int i = 0; i < NUM_THREAD; i++) {
                    args[i].data = output;
                    args[i].degree = degree;
                }
                flag = 0;
                // Tạo luồng xử lý
                clock_gettime(CLOCK_MONOTONIC, &start);
                create_thread();

                // Chờ các luồng hoàn thành
                for (int i = 0; i < NUM_THREAD; i++) {
                    pthread_join(threads[i], NULL);
                }
                clock_gettime(CLOCK_MONOTONIC, &end);
                double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
                printf("\nThời gian tìm nghiệm: %f giây\n", elapsed);

                //printf("\nNGHIEM CUA BISECTION %f",bisectionMethod(output,-10,10));

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