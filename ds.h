#ifndef DATA_STRUCTS
#define DATA_STRUCTS

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int int_cmp(void *x, void *y);

char *int_p(void *v);

int s_cmp(void *x, void *y);

char *s_p(void *v);

typedef struct list {
  void *v;
  struct list *n;
} list;

list *cons(void *v, list *lox);

void *car(list *lox);

list *cdr(list *lox);

int list_length(list *lox);

char *list_contents(list *lox, char *(*pfunc)(void *v));

void *list_ref(unsigned int n, list *lox);

list *list_insert(void *v, unsigned int i, list *lox);

list *list_delete(unsigned int i, list *lox);

list *list_append(list *alox, list *blox);

list *list_reverse(list *lox);

void list_free(list *lox);

typedef struct bst {
  void *v;
  struct bst *l;
  struct bst *r;
  int (*cmpfunc)(void *x, void *y);
} bst;

bst *BST(int (*cmpfunc)(void *x, void *y));

void addbst(void *v, bst *bst);

void *getbst(void *v, bst *btree);

typedef struct {
    void *mem;
    size_t type;
    int size;
    size_t capacity;
    int init;
} vector;

vector *v_alloc(size_t type, size_t capacity);

#define Vector(type, capacity) v_alloc(sizeof(type), (capacity))

void *v_ref(int i, vector *vec);

void v_set(int i, void *v, vector *vec);

void v_append(void *v, vector *vec);

void v_fill(int start, int stop, void *v, vector *vec);

char *v_contents(vector *vec, char *(*vpfunc)(void *e));

void v_insert(int i, void *v, vector *vec);

void v_delete(int i, vector *vec);

void v_free(vector *vec);

void v_clear(vector *vec);

vector *v_copy(vector *vec);

#endif
