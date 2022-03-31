#include "../ds.h"
#include <stdbool.h>

/* Pipeline
 * tokenize: stdin -> tokens
 * parse: tokens -> Object (SyntaxTree)
 * eval: Object -> Object
 * print: Object -> stdout
 */

typedef enum {
    LPAREN,
    RPAREN,

    TRUE,
    FALSE,
    NIL,

    IF,
    COND,
    DEFINE,
    LAMBDA,
    QUOTE,

    INT,
    FLOAT,
    SYMBOL,
    STRING
} Symbol;

typedef struct {
    char *lex;
    Symbol dat;
} Token;

void tok_dump(vector*);
vector *tokenize(char*);

typedef struct Object {
    Token sym;
    object *left;
    object *right;
} Object

