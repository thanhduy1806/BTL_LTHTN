#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "token.h"
#include "solve_equation.h"
#include "evaluate.h"
#include "global_variable.h"



// Tính đạo hàm số bằng phương pháp sai phân
float derivative(Token *postfix, float x) {
    float h = 0.0001;
    return (evaluatePostfix(postfix, x + h) - evaluatePostfix(postfix, x ) / h);
}

int isMultipleRoot(Token *postfix, float x) {
    float fx = evaluatePostfix(postfix, x);
    float dfx = derivative(postfix, x);
    // printf("TRI TUYET DOI F(X0)= %f\n",fabs(fx));
    // printf("DAO HAO TAI X: %f \n",fabs(dfx));
    return (fabs(fx) < 1e-6 && fabs(dfx) < 1e-4);
}

// Hàm kiểm tra nghiệm trùng
int isDuplicate(float root) {
    for (int i = 0; i < count_roots; i++) {
        if (fabs(roots[i] - root) < 1e-6) {
            return 1; // Trùng nghiệm
        }
    }
    return 0;
}



 //Ham tính Bisection
 float bisectionMethod(Token *postfix){
    float a = -100.0;
    float b = 100.0;
    float c;
    float fa,fb,fc;
    int i = 0;
    int max = 100;
    while (i < max){
        fa = evaluatePostfix(postfix, a);
        fb = evaluatePostfix(postfix, b);
        c = (a+b)/2;
        fc = evaluatePostfix(postfix, c);
        if (fabs(fc) < 1e-6){
            return c;
        }
        if (fa*fc < 0){
            b=c;
        }
        if (fb*fc < 0){
            a=c;
        }
        i++;
    }
    return NAN;
}

//Ham tính Secant
float secantMethod(Token *postfix){
    float x0 = 0.00;
    float x1 = 1.00;
    float x2;
    float fx0, fx1, fx2;
    int i = 0;
    int max = 100;
    while (i < max){
        fx0 = evaluatePostfix(postfix, x0);
        fx1 = evaluatePostfix(postfix, x1);
        x2 = x1 - fx1*(x1-x0)/(fx1-fx0);
        fx2 = evaluatePostfix(postfix, x2); 
        if (fabs(fx2) < 1e-6){
            return x2;
        }
        x0 = x1;
        x1 = x2;
        i++;
    return NAN;
    }
}

//Hàm tính newton
float newtonRaphson(Token *postfix, float x0){
    //float x0 = 1.0;
    float x1;
    float fx0, fxh, fdx0;
    int i = 0;
    int max = 100;
    float h = 0.0001;
    while (i<max){
        fx0 = evaluatePostfix(postfix, x0);
        fxh = evaluatePostfix(postfix, (x0+h));
        fdx0 = (fxh-fx0)/h;

        if (fabs(fdx0) < 1e-6){
            return NAN;
        }

        x1 = x0 - fx0/fdx0;
        if (fabs(x1-x0) < h){
            return x1;
        }
        x0 = x1;
        i++;
    }
    return NAN;
}


/*
Ý TƯỞNG ĐỂ TÌM TẤT CẢ CÁC NGHIỆM CỦA PHƯƠNG TRÌNH SỐ THỰC:
Phân tích vấn đề:
1/ Cần phải return về tất cả các nghiệm -> cần có 1 vùng nhớ để lưu trữ các nghiệm, để nó return về cái vùng đó
2/ Làm sao để tìm hết các nghiệm -> Thử bằng nhiều khoảng nhỏ, và mỗi khoảng đó sẽ gọi hàm tìm nghiệm 1 lần
3/ Kiểm tra các nghiệm nếu có trùng thì không lấy

----------------------------------------------------------------------------------
- Giải quyết vấn đề 2: giờ ta sẽ có 1 khoảng to, sau đó sẽ bắt đầu từng chặn nhỏ với độ dài là 10, khi nào quét hết các chặn thì thôithôi



*/






void FindAllRoot(Token *postfix){

    float root;
    float step = 0.3;
    float x1,x2;
    float fx1,fx2;
    int start = -100;
    int stop = 100;
    count_roots = 0;
    x1 = start;
    while (x1 < stop){
        fx1 = evaluatePostfix(postfix,x1);
        x2 = x1 + step;
        fx2 = evaluatePostfix(postfix,x2);
        if (fx1*fx2<0){
            root = newtonRaphson(postfix,(x1+x2)/2);
        }

        else if (fabs(fx1) < 1e-6){
            root = newtonRaphson(postfix,x1);
        }
        else if (fabs(fx2)<1e-6 && fabs(fx2)<fabs(fx1)){
            root = newtonRaphson(postfix,x2);
        }
        else{
            root = NAN;
        }
        
        if(!isnan(root)&&!isDuplicate(root)){
            if (isMultipleRoot(postfix, root)) {
                type_root[count_roots] = 'd';  // Nghiệm kép
                
            } 
            else {
                type_root[count_roots] = 's';  // Nghiệm đơn
            }
            roots[count_roots] = root;
            count_roots ++;
        }
        x1 = x2;
    }
}