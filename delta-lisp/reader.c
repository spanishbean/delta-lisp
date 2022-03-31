#include "common.h" 
#include <ctype.h>
#include <regex.h>

#define TOK_INIT 50
#define BUF_INIT 50
#define SE_INIT 50

static const Symbol tbl[] = {
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
};

void tok_dump(vector *tokens)
{
    static char *stbl[] = {
        "LPAREN",
        "RPAREN",
        "TRUE",
        "FALSE",
        "NIL",
        "IF",
        "COND",
        "DEFINE",
        "LAMBDA",
        "QUOTE",
        "INT",
        "FLOAT",
        "SYMBOL",
        "STRING"
    };

    for (int i = 0; i < tokens->size; ++i) {
        Token *tok = v_ref(i, tokens);
        printf("(\"%s\", %s)\n", tok->lex, stbl[tok->dat]);
    }
}

static int match(char *pat, char *str)
{
    regex_t re;
    if (regcomp(&re, pat, REG_EXTENDED))
        return -1;
    if (!regexec(&re, str, 0, NULL, 0))
        return 0;
    else return -1;
}

static Symbol classify(char *str)
{ 
    static char *rtbl[]= {
        "^\($",
        "^\)$",
        "^#t$",
        "^#f$",
        "^nil$",
        "^if$",
        "^cond$",
        "^define$",
        "^lambda$",
        "^('|quote)$",
        "^\-?[0-9]+$", // INT
        "^\-?(\\.{1}[0-9]+|[0-9]+\\.{1}[0-9]*)$" // FLOAT
    };

    if (str[0] == '\0') return -1;

    size_t sz = sizeof(rtbl)/sizeof(rtbl[0]);
    for (int i = 0; i < sz; ++i)
        if (match(rtbl[i], str) == 0)
            return tbl[i];
    return SYMBOL;
}

static char *zstrip(char *str)
// remove unneeded leading zeroes
{
    int i = 0;
    char neg = 0;
    if (str[i] == '-') {++i; neg = 1;}
    for (; (str[i] == '0') && (str[i+1] != '.'); ++i);
    return (neg)? strcat(strdup("-"), &str[i]) : &str[i];
}

static void add_tok(vector *buf, vector *out, Symbol sym)
{
    Token tok;
    tok.lex = strdup(buf->mem);
    tok.dat = sym;
    if (tok.dat == INT || tok.dat == FLOAT)
        tok.lex = zstrip(tok.lex);
    v_append(&tok, out);
}

static void look_back(vector *buf, vector *out)
{
    Symbol sym = classify(buf->mem);
    //printf("sym: %d\n", sym);
    if (sym == -1) return;
    add_tok(buf, out, sym);
    v_clear(buf);
}

static char *handle_string(vector *buf, vector *out, char *pos)
{
    char c;
    for (; ((c = *pos) != '"') && (c != '\0'); ++pos)
        v_append(&c, buf);

    if (c == '"') {
        v_append(&c, buf);
        add_tok(buf, out, STRING);
        v_clear(buf);
        return pos;
    } else {
        fprintf(stderr, "error: unmatched quote\n");
        exit(-1);
    }
}

vector *tokenize(char *in) {
    vector *out = Vector(Token, TOK_INIT);
    vector *buf = Vector(char, BUF_INIT);
    char c; // lexical representation of current character

    for (; *in != '\0'; ++in) {
        switch (c = *in) {
            case '(':
                look_back(buf, out);
                v_append(&c, buf);
                add_tok(buf, out, LPAREN);
                v_clear(buf);
                break;
            case ')':
                look_back(buf, out);
                v_append(&c, buf);
                add_tok(buf, out, RPAREN);
                v_clear(buf);
                break;
            case ' ':
                look_back(buf, out);
                break;
            case '"':
                v_append(&c, buf);
                char *npos = handle_string(buf, out, ++in);
                in = npos;
                break;
            default:
                v_append(&c, buf);
                break;
        }
    }

    look_back(buf, out);
    v_free(buf);
    return out;
}

