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

#endif 
