#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    OPERAND,
    OPERATOR,
    VARIABLE
} TokenType;


typedef struct {
    TokenType type;
    union {
        float operand;
        char operator[5];
        float variable;
    } value;
} Token;


int isOperator(char c);
int isTrigoFunction(const char *str);
Token *infixToPostfix(char* myFunction);
void printTokens(Token *output);
int findDegree(Token *output, int length);



#endif 
