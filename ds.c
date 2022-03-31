#include "ds.h"

int int_cmp(void *x, void *y)
{
  return memcmp(x, y, sizeof(int));
}

char *int_p(void *v)
{
  char *buf = malloc(sizeof(char));
  sprintf(buf, "%d", *(int*) v);
  return buf;
}

int s_cmp(void *x, void *y)
{
  return memcmp(x, y, sizeof(char));
}

char *s_p(void *v)
{
  return v;
}

/* All data structs are homogenous! */

/********LISTS********/

list *cons(void *v, list *lox)
{
  list *lp = malloc(sizeof(list));
  lp->v = v;
  lp->n = lox;

  return lp;
}

void *car(list *lox)
{
  return lox->v;
}

list *cdr(list *lox)
{
  return lox->n;
}

int list_length(list *lox)
{
  if (lox == NULL)
    return 0;
  else
    return 1 + list_length(cdr(lox));
}

char *list_contents(list *lox, char *(*pfunc)(void *v))
{
  char *vsep = NULL;
  char *str = NULL;
  size_t len;
  int rsize = 0;
  
  list *p;
  for (p = lox; p != NULL; p = p->n) {
    len = strlen(pfunc(p->v));
    rsize += len;
    vsep = realloc(vsep, rsize+1);
    str = realloc(str, len+1);
    
    sprintf(str, "%s,", pfunc(p->v));
    strcat(vsep, str);
  }
  
  free(str);
  vsep[strlen(vsep)-1] = '\0';
  return vsep;
}

void *list_ref(unsigned int n, list *lox)
{
  list *lp = lox;
  for (int i = 0; i < n; i++, lp = lp->n) {
    if (lp == NULL)
      return NULL;
  }

  return lp->v;
}

list *list_insert(void *v, unsigned int i, list *lox)
{
  if (lox == NULL)
    return NULL;
  else if (i == 0)
    return cons(v, lox);
  else
    return cons(car(lox), list_insert(v, --i, cdr(lox)));
}

list *list_delete(unsigned int i, list *lox)
{
  if (lox == NULL)
    return NULL;
  else if (i == 0)
    return cdr(lox);
  else
    return cons(car(lox), list_delete(--i, cdr(lox)));
}

list *list_append(list *alox, list *blox)
{
  if (alox == NULL)
    return blox;
  return cons(car(alox), list_append(cdr(alox), blox));
}

list *list_reverse(list *lox)
{
  if (lox == NULL)
    return NULL;
  return list_append(list_reverse(cdr(lox)), cons(car(lox), NULL));
}

void list_free(list *lox)
{
    list *p = lox;
    list **stack = malloc(sizeof(list*)*list_length(lox));
    int i = 0;

    for (; p != NULL; p = cdr(p))
        stack[i++] = p;
    --i;
    while (i-- >= 0) free(stack[i]);
}

/********TREES********/

bst *BST(int (*cmpfunc)(void *x, void *y)) // cmpfunc used for pointers
{
  bst *bp = malloc(sizeof(bst));
  bp->l = NULL;
  bp->r = NULL;
  bp->cmpfunc = cmpfunc;

  return bp;
}

void addbst(void *v, bst *bst)
{ 
  if (bst->v == NULL) {
    bst->v = v;
    bst->l = BST(bst->cmpfunc);
    bst->r = BST(bst->cmpfunc);
  }
  else {
    int r = bst->cmpfunc(v, bst->v);
    if (r == 0)
      return;
    else if (r < 0)
      addbst(v, bst->l);
    else
      addbst(v, bst->r);
  }
}

void *getbst(void *v, bst *btree)
{
  if (btree->v != NULL) {
      int r = btree->cmpfunc(v, btree->v);
      switch (r) {
        case 0:
            return v;
            break;
        case -1:
            getbst(v, btree->l);
            break;
        case 1:
            getbst(v, btree->r);
            break;
      
      }
  }

  return NULL;
}

/********VECTORS********/

vector *v_alloc(size_t type, size_t capacity)
{
    vector *v = malloc(sizeof(vector));
    v->mem = malloc(type * capacity);
    memset(v->mem, 0, capacity*type);
    v->type = type;
    v->size = 0;
    v->capacity = type*capacity;
    v->init = type*capacity;

    return v;
}

void *v_ref(int i, vector *vec)
{
    return vec->mem + (vec->type*i); 
}

void v_set(int i, void *v, vector *vec)
{
    if (vec->size == (vec->capacity/vec->type)) {
        vec->mem = realloc(vec->mem, vec->capacity + vec->init);
        vec->capacity = vec->capacity + vec->init;
    }

    memcpy(v_ref(i, vec), v, vec->type);
    vec->size++;
}

void v_append(void *v, vector *vec)
{
    v_set(vec->size, v, vec);
}

void v_fill(int start, int stop, void *v, vector *vec)
{
    // inclusive, zero first
    for (int i = start; i <= stop; ++i)
        v_set(i, v, vec);
}

char *v_contents(vector *vec, char *(*vpfunc)(void *e))
{
    // remember to free after usage
    // can only be freed if result is assigned
  char *vsep = NULL;
  size_t rsize = 0;
  size_t len;
    
  for (int i = 0; i < (vec->capacity/vec->type); ++i) {
    len = strlen(vpfunc(v_ref(i, vec)));
    rsize += len;
    vsep = realloc(vsep, rsize+1);
    
    sprintf(vsep, "%s%s,", vsep, vpfunc(v_ref(i, vec)));
  }
  
  vsep[strlen(vsep)-1] = '\0';

  return vsep;
}

void v_insert(int i, void *v, vector *vec)
{
    unsigned long nsz = vec->capacity+vec->type;
    void *bmem = malloc(nsz);
    char s = 0;
    for (unsigned long k=0; k < (nsz/vec->type); ++k) {
        if (k == i) {
            memcpy(bmem+(vec->type*k), v, vec->type);
            s = 1;
        } else {
            memcpy(bmem+(vec->type*k), vec->mem+(vec->type*(k-s)), vec->type);
            if (s) s = 0;
        }
    }
    vec->size++;
    vec->capacity += vec->type;
    free(vec->mem);
    vec->mem = bmem;
}

void v_delete(int i, vector *vec)
{
    unsigned long nsz = vec->capacity-vec->type;
    void *bmem = malloc(nsz);
    char s = 0;
    for (unsigned long k=0; k < (nsz/vec->type); ++k) {
        if (k == i) {
            s = 1;
        } else {
            memcpy(bmem+(vec->type*(k-s)), vec->mem+(vec->type*k), vec->type);
        }
    }
    vec->size--;
    vec->capacity -= vec->type;
    free(vec->mem);
    vec->mem = bmem;
}

void v_free(vector *vec)
{
    free(vec->mem);
    free(vec);
}

void v_clear(vector *vec)
{
    free(vec->mem);
    vec->mem = malloc(sizeof(vec->type)*(vec->capacity/vec->type));
    vec->size = 0;
}

vector *v_copy(vector *vec)
{
    vector *w = v_alloc(vec->type, vec->capacity);
    for (int i = 0; i < (vec->capacity/vec->type); ++i)
        v_set(i, v_ref(i, vec), w);
    return w;
}
