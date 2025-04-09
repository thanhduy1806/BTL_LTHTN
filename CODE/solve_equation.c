#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
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
    float step = 0.123;
    int i = 0;
    int max = 100;
    fx1 = evaluatePostfix(postfix, x1);
    fx2 = evaluatePostfix(postfix, x2);
    if (fx1 == 0) return x1;
    if (fx2 == 0) return x2;

    if (!(fx1*fx2<0)){
        while(x1<x2){
            x1 = x1 + step;
            fx1 = evaluatePostfix(postfix, x1);
            if (fx1 * fx2 <= 0) break;
        }
    }

    printf("X1 CUA BISECTION SAU KHI THU HEP %f\n",x1);

    while (i < max){

        c = (x1+x2)/2;
        fxc = evaluatePostfix(postfix, c);
        if (fabs(fxc) < 1e-5){
            return c;
        }
        if (fx1*fxc < 0){
            x2=c;
            fx2=fxc;
        }
        else if (fx2*fxc < 0){
            x1=c;
            fx1=fxc;
        }
        else
        i++;
    }
    return NAN;
}

//Ham tính Secant
float secantMethod(Token *postfix){
    float x0 = -10.00;
    float x1 = 10.00;
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
    float h = 0.000001;
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

        if (fabs(d2fx) > 1e-3) {
            m = (dfx * dfx) / (fx0 * d2fx);
            if (m > 11) m = 11;  // Giới hạn m để tránh sai số quá lớn
            if (m < 1) m = 1;  // m tối thiểu là 1
        } else {
            m = 1;  // Khi đạo hàm cấp 2 quá nhỏ, quay lại Newton-Raphson bình thường
        }
        
        
        x0 = x0 - (m * fx0 / dfx);

        i++;
    }
    return NAN;
}


// float Laguerre(Token *postfix,int degree,float x0){
//     float fx0, dfx,d2fx;
//     int i = 0;
//     int max = 100;
//     float h = 0.000001;
//     while (i<max){
//         fx0 = evaluatePostfix(postfix, x0);
//         dfx = derivative(postfix,x0);
//         d2fx = secondDerivative(postfix,x0);

        

//         if (fabs(fx0)<1e-6){
//             return x0;
//         }

//         else{
//             x0 = x0 - (degree*fx0/(dfx+sqrt((degree-1)*((degree-1)*(dfx*dfx-degree*fx0*d2fx)))));
//         }
        
//         i++;
//     }
//     return NAN;

// }


float Laguerre(Token *postfix, int degree, float x0) {
    int i = 0;
    for (float x = x0; x < 10; x += 0.1) {
        float fx1 = evaluatePostfix(postfix, x);
        float fx2 = evaluatePostfix(postfix, x + 0.1);
        if (fx1 * fx2 <= 0) {
            x0 = (x + x + 0.1) / 2;
            // Dùng x0 này cho Newton hoặc Laguerre
        }
    }
    
    while (i < 100) {
        float fx0 = evaluatePostfix(postfix, x0);
        float dfx = derivative(postfix, x0);
        float d2fx = secondDerivative(postfix, x0);

        if (fabs(fx0) < 1e-6) {
            return x0;  // Nếu giá trị đa thức nhỏ, trả về nghiệm
        }

        float G = dfx / fx0;
        float H = G * G - (d2fx / fx0);
        float sqrtTerm = sqrt(fabs((degree - 1) * (degree * H - G * G)));  // Đảm bảo không sqrt số âm

        // Chọn dấu lớn nhất về độ lớn để tránh sai số
        float denom1 = G + sqrtTerm;
        float denom2 = G - sqrtTerm;
        float denom = (fabs(denom1) > fabs(denom2)) ? denom1 : denom2;

        if (fabs(denom) < 1e-6) {  // Tránh chia cho số gần 0
            return x0;
        }

        float dx = degree / denom;
        x0 -= dx;

        if (fabs(dx) < 1e-6) {
            return x0;  // Nếu thay đổi nhỏ, nghiệm đã hội tụ
        }

        i++;
    }
    return NAN;  // Không hội tụ sau MAX_ITER lần lặp
}


double brent(Token *postfix, double a, double b, double tol) {
    double fa = evaluatePostfix(postfix, a);
    double fb = evaluatePostfix(postfix, b);

    if (fa * fb > 0) return NAN;

    if (fabs(fa) < fabs(fb)) {
        // Đảm bảo fa luôn lớn hơn để tối ưu
        double temp = a; a = b; b = temp;
        temp = fa; fa = fb; fb = temp;
    }

    double c = a;
    double fc = fa;
    double d = b - a;
    double e = d;

    for (int iter = 0; iter < 100; iter++) {
        if (fabs(fc) < fabs(fb)) {
            a = b; b = c; c = a;
            fa = fb; fb = fc; fc = fa;
        }

        double tol1 = 2 * tol * fabs(b) + 1e-10;
        double xm = 0.5 * (c - b);

        if (fabs(xm) <= tol1 || fb == 0.0) {
            return b;
        }

        if (fabs(e) >= tol1 && fabs(fa) > fabs(fb)) {
            // Nội suy
            double s = fb / fa;
            double p, q;

            if (a == c) {
                // Cát tuyến
                p = 2 * xm * s;
                q = 1.0 - s;
            } else {
                // Nội suy bậc 2
                double q1 = fa / fc;
                double r = fb / fc;
                p = s * (2 * xm * q1 * (q1 - r) - (b - a) * (r - 1));
                q = (q1 - 1) * (r - 1) * (q1 - r);
            }

            if (p > 0) q = -q;
            p = fabs(p);

            if (2 * p < fmin(3 * xm * q - fabs(tol1 * q), fabs(e * q))) {
                e = d;
                d = p / q;
            } else {
                d = xm;
                e = d;
            }
        } else {
            d = xm;
            e = d;
        }

        a = b;
        fa = fb;

        if (fabs(d) > tol1) {
            b += d;
        } else {
            b += (xm > 0 ? tol1 : -tol1);
        }

        fb = evaluatePostfix(postfix, b);
        fc = evaluatePostfix(postfix, c);
    }

    return NAN; // Nếu không hội tụ sau 100 bước
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






// void FindAllRoot_newton(Token *postfix){
//     int num = 0;
//     float root;
//     double step = 0.1;
//     double x1,x2;
//     double fx1,fx2;
//     int start = -100;
//     int stop = 100;
//     count_roots = 0;
//     x1 = start;
//     int i =0;
//     while (strcmp(postfix[i].value.operator, "E") != 0) {
//         if (strcmp(postfix[i].value.operator, "sin")==0 || strcmp(postfix[i].value.operator, "cos")==0 || strcmp(postfix[i].value.operator, "tan")==0){
//            start = -3.14; // -pi 
//            stop = 3.14; // pi
//            break;
//         }
//         i++;
//     }
//     while (x1 < stop){

//         fx1 = evaluatePostfix(postfix,x1);
//         x2 = x1 + step;
//         fx2 = evaluatePostfix(postfix,x2);

//         if (fx1*fx2<0){
//             root = newtonRaphson(postfix,(x1+x2)/2,1);
//             printf("NGHIEM THU %d CUA NEWTON LA: %lf\n",num,root);
//         }

//         else if (fabs(fx1) < 1e-7 ){
//             root = newtonRaphson(postfix,x1,1);
//             printf("NGHIEM THU %d CUA NEWTON LA: %lf\n",num,root);
//         }

//         else if (fabs(fx2) < 1e-7 && fabs(fx2)<fabs(fx1)){
//             root = newtonRaphson(postfix,x2,1);
//             printf("NGHIEM THU %d CUA NEWTON LA: %lf\n",num,root);
//         }

//         else{
//             root = NAN;
//         }

//         // if(!isnan(root)&&!isDuplicate(root)){
//         //     int m = multipleroot(postfix, root); // Xác định bội số của nghiệm
//         //     if (m%2 == 0) {
//         //         type_root[count_roots] = 'd';  // Nghiệm kép hoặc bội
//         //     } else {
//         //         type_root[count_roots] = 's';  // Nghiệm đơn
//         //     }
//         //     roots[count_roots] = root;
//         //     count_roots++;
//         // }
//         x1 = x2;
//         num ++;
//     }
// }


// void FindAllRoots_Bisection(Token *postfix) {
//     double step = 0.5;  // Khoảng chia nhỏ
//     double x1, x2, fx1, fx2, root;
//     int i = 0;
//     float start = -100, stop = 100;
//     while (strcmp(postfix[i].value.operator, "E") != 0) {
//         if (strcmp(postfix[i].value.operator, "sin")==0 || strcmp(postfix[i].value.operator, "cos")==0 || strcmp(postfix[i].value.operator, "tan")==0){
//            start = -3.14; // -2pi 
//            stop = 3.14; // 2pi
//            break;
//         }
//         i++;
//     }
    
//     count_roots = 0;

//     x1 = start;
//     while (x1 < stop) {
//         fx1 = evaluatePostfix(postfix, x1);
//         x2 = x1 + step;
//         fx2 = evaluatePostfix(postfix, x2);

//         if (fx1 * fx2 < 0) {  // Nếu dấu đổi, gọi Bisection
//             root = bisectionMethod(postfix,x1,x2);
//         } else if (fabs(fx1) < 1e-6) {  // Nếu x1 gần nghiệm
//             root = x1;
//         } else if (fabs(fx2) < 1e-6) {  // Nếu x2 gần nghiệm
//             root = x2;
//         } else {
//             root = NAN;
//         }

//         if (!isnan(root) && !isDuplicate(root)) {
//             int m = multipleroot(postfix, root); // Xác định bội số của nghiệm
//             if (m%2 == 0) {
//                 type_root[count_roots] = 'd';  // Nghiệm kép hoặc bội
//             } else {
//                 type_root[count_roots] = 's';  // Nghiệm đơn
//             }
//             roots[count_roots] = root;
//             count_roots++;
//         }

//         x1 = x2;  // Chuyển sang đoạn tiếp theo
//     }
// }
