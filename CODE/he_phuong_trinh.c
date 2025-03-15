#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define MAX 100

float arr[5][5];
float x[5];

void input(int step, float arr[][5]){
    printf("NHAP CAC HE SO: ");
    for (int i = 0; i < step ; i ++){
        for (int j = 0; j < step + 1; j++){
            scanf("%f", &arr[i][j]);
        }
    }
}

void print_input(int step){
    
    printf("MA TRAN HE SO: \n");
    for (int i = 0; i < step ; i ++){
        for (int j = 0; j < step + 1; j++){
            printf("%.2f ", arr[i][j]);
        }
        printf("\n");
    }
}



void gauss(int step,float arr[][5]){
    for (int i = 0; i<step; i++){
        //SAP XEP
        int maxrow = i;
        for (int k = i+1 ; k < step ; k++){
            if (fabs(arr[k][i] > fabs(arr[maxrow][i]))){
                maxrow = k;
            }
        }

        //DOI VI TRI HANG
        if (maxrow != i){
            for (int j = i; j <= step; j++){    //j duyet colum
                float temp = arr[i][j];
                arr[i][j] = arr[maxrow][j];
                arr[maxrow][j] = temp;
            }
        }

        //KHU GAUSS

        for (int j = i+1; j < step; j++ ){
            float factor = arr[j][i] / arr [i][i];
            for (int m = i; m <= step; m++){
                arr[j][m] -= arr[i][m]*factor;
            }
        }

    }
}


void solve_x(int step, float arr[][5], float x[5]){
    for(int i = step-1; i>=0; i--){
        x[i] = arr[i][step];
        for (int j = i+1; j<step; j++){
            x[i] -= x[j]*arr[i][j];
        }
        x[i] = x[i] / arr[i][i];
    }
    for (int i = 0; i< step; i++){
        printf("x[%d]= %.2f\n",i,x[i]);
    }
}





void main(){
    int step;
    printf("Nhập bậc hệ phương trình: ");
    scanf("%d",&step);
    input(step,arr);
    print_input(step);
    printf("______________\n");
    gauss(step,arr);
    printf("SAO KHI KHU GAUSS: \n");
    print_input(step);
    printf("______________\n");
    solve_x(step,arr,x);
    

}