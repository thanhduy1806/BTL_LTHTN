#ifndef SOLVE_EQUATION
#define SOLVE_EQUATION
#include "token.h"

float newtonRaphson(Token *postfix, float x0);
float bisectionMethod(Token *postfix);
float secantMethod(Token *postfix);
void FindAllRoot(Token *postfix);
int isDuplicate(float root);
float derivative(Token *postfix, float x);
int isMultipleRoot(Token *postfix, float x);

#endif