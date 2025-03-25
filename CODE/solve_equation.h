#ifndef SOLVE_EQUATION
#define SOLVE_EQUATION
#include "token.h"

float newtonRaphson(Token *postfix, float x0, int m);
float bisectionMethod(Token *postfix,float x1, float x2);
float secantMethod(Token *postfix);
void FindAllRoot_newton(Token *postfix);
void FindAllRoots_Bisection(Token *postfix);
int isDuplicate(float root);
float derivative(Token *postfix, float x);
float nthDerivative(Token *postfix, float x, int m);
int multipleroot(Token *postfix, float x);
float secondDerivative(Token *postfix, float x);

#endif