#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "token.h"
#include "solve_equation.h"
#include "evaluate.h"
#include "global_variable.h"




// Tính đạo hàm cấp 1
float derivative(Token *postfix, float x) {
    float h = 0.0001;
    return (evaluatePostfix(postfix, x + h) - evaluatePostfix(postfix, x )) / h;
}

//
float secondDerivative(Token *postfix, float x) {
    float h = 0.0001;
    return (evaluatePostfix(postfix, x + h) - 2 * evaluatePostfix(postfix, x) + evaluatePostfix(postfix, x - h)) / (h * h);
}
 

//Hôi quy đạo hàm bậc m
float nthDerivative(Token *postfix, float x, int m) {
    float h = 1e-5;
    if (m == 1) return derivative(postfix, x);
    return (nthDerivative(postfix, x + (1e-5), m - 1) - nthDerivative(postfix, x - (1e-5), m - 1)) / (2 * (1e-5));
}

//Kiểm tra bậc m
int multipleroot(Token *postfix, float x){
    int m = 1;
    float dfx;
    
    do {
        dfx = nthDerivative(postfix, x, m);
        printf("BẬC CỦA HÀM SỐ: %d\n", m);
        if (fabs(dfx) < 1e-2) {
            m++;  // Chỉ tăng nếu dfx vẫn nhỏ
        } else {
            break;  // Thoát khi tìm thấy bậc không bị triệt tiêu
        }
    } while (m < 10);

    return m;
}



// int isMultipleRoot(Token *postfix, float x) {
//     float fx = evaluatePostfix(postfix, x);
//     float dfx = derivative(postfix, x);
//     // printf("TRI TUYET DOI F(X0)= %f\n",fabs(fx));
//     // printf("DAO HAO TAI X: %f \n",fabs(dfx));
//     return (fabs(fx) < 1e-6 && fabs(dfx) < 1e-4);
// }

// Hàm kiểm tra nghiệm trùng
int isDuplicate(float root) {
    for (int i = 0; i < count_roots; i++) {
        if (fabs(roots[i] - root) < 1e-3) {
            return 1; // Trùng nghiệm
        }
    }
    return 0;
}



 //Ham tính Bisection
 float bisectionMethod(Token *postfix, float x1, float x2){
    float c;
    float fx1,fx2,fxc;
    int i = 0;
    int max = 100;
    fx1 = evaluatePostfix(postfix, x1);
    fx2 = evaluatePostfix(postfix, x2);
    if (fx1 == 0) return x1;
    if (fx2 == 0) return x2;
    while (i < max){
        
        c = (x1+x2)/2;
        fxc = evaluatePostfix(postfix, c);
        if (fabs(fxc) < 1e-6){
            return c;
        }
        if (fx1*fxc < 0){
            x2=c;
            fx2=fxc;
        }
        if (fx2*fxc < 0){
            x1=c;
            fx1=fxc;
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
float newtonRaphson(Token *postfix, float x0, int m){
    //float x0 = 1.0;
    float fx0, dfx, d2fx;
    int i = 0;
    int max = 100;
    float h = 0.0001;
    while (i<max){
        fx0 = evaluatePostfix(postfix, x0);
        dfx = derivative(postfix,x0);
        d2fx = secondDerivative(postfix,x0);
        
        if (fabs(dfx) < 1e-6){
            return NAN;
        }

        if (fabs(fx0)<1e-6){
            return x0;
        }

        x0 = x0 - (m * fx0 / dfx);

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






void FindAllRoot_newton(Token *postfix){

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
            root = newtonRaphson(postfix,(x1+x2)/2,1);
        }

        else if (fabs(fx1) < 1e-6){
            root = newtonRaphson(postfix,x1,1);
        }

        else if (fabs(fx2)<1e-6 && fabs(fx2)<fabs(fx1)){
            root = newtonRaphson(postfix,x2,1);
        }

        // else if ((fx1<1e-5)||(fx2<1e-5)){
        //     root = newtonRaphson(postfix,(x1+x2)/2);
        // }

        else{
            root = NAN;
        }
        
        if(!isnan(root)&&!isDuplicate(root)){
            int m = multipleroot(postfix, root); // Xác định bội số của nghiệm
            if (m%2 == 0) {
                type_root[count_roots] = 'd';  // Nghiệm kép hoặc bội
            } else {
                type_root[count_roots] = 's';  // Nghiệm đơn
            }
            roots[count_roots] = root;
            count_roots++;
        }
        x1 = x2;
    }
    
}


void FindAllRoots_Bisection(Token *postfix) {
    float step = 0.5;  // Khoảng chia nhỏ
    float x1, x2, fx1, fx2, root;
    int start = -100, stop = 100;
    count_roots = 0;

    x1 = start;
    while (x1 < stop) {
        fx1 = evaluatePostfix(postfix, x1);
        x2 = x1 + step;
        fx2 = evaluatePostfix(postfix, x2);

        if (fx1 * fx2 < 0) {  // Nếu dấu đổi, gọi Bisection
            root = bisectionMethod(postfix,x1,x2);
        } else if (fabs(fx1) < 1e-6) {  // Nếu x1 gần nghiệm
            root = x1;
        } else if (fabs(fx2) < 1e-6) {  // Nếu x2 gần nghiệm
            root = x2;
        } else {
            root = NAN;
        }

        if (!isnan(root) && !isDuplicate(root)) {  
            int m = multipleroot(postfix, root); // Xác định bội số của nghiệm
            if (m%2 == 0) {
                type_root[count_roots] = 'd';  // Nghiệm kép hoặc bội
            } else {
                type_root[count_roots] = 's';  // Nghiệm đơn
            }
            roots[count_roots] = root;
            count_roots++;
        }

        x1 = x2;  // Chuyển sang đoạn tiếp theo
    }
}
