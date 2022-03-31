#include "ds.c"

#ifndef ALGORITHM
#define ALGORITHM

/* C lib for common algorithms */

#define ASZ(a) sizeof((a))/sizeof((a)[0])

void aiprint(int *a, int max)
{
    for (int i = 0; i < max; ++i)
        printf("%d ", *(a+i));
    putchar('\n');    
}

void swap(void *p, void *q, size_t type)
{
    // extend for sortv
    void *t = malloc(type);
    memcpy(t, p, type);
    memcpy(p, q, type);
    memcpy(q, t, type);
    free(t);
}

/*
gtcmp & ltcmp work for numbers and strings
structs require unique functions
*/

int gtcmp(void *x, void *y, size_t type)
{
    int r = memcmp(x, y, type);
    if (r > 0)
        return 1;
    else if (r < 0)
        return -1;
    else
        return 0;
}


int ltcmp(void *x, void *y, size_t type)
{
    int r = memcmp(x, y, type);
    if (r > 0)
        return -1;
    else if (r < 0)
        return 1;
    else
        return 0;
}

int find_max(void *a, int asz, size_t type)
{
    int max = 0;
    for (int i = 0; i < asz; ++i)
        if (gtcmp(a+i*type, a+max*type, type) == 1)
            max = i;
    return max;
}

/********SORTING********/

/* note on cmpf's return value:
 * 0 => equal
 * -1 => not intended (x is greater/less than y, when it should be vice versa)
 * 1 => intended (x is greater/less than y, as it should)
 */

void *slice(void *a, int l, int r, size_t type, int *nsz)
/* generates new array from [l, r] zero-based
 * integer division always rounds down */
{
    // {1 2 3 4 5} 1 3
    int n = (r-l)+1;
    void *asub = malloc(type*n);
    for (int i = 0; i < n; ++i, ++l)
        memcpy(asub+(i*type), a+(l*type), type);
    *nsz = n;
    return asub;
}

static void *merge(void *a, size_t asz,
        void *b, size_t bsz, 
        size_t type, 
        int(*cmpf)(void *x, void *y, size_t t))
/* generates new sorted array from merger of two sorted arrays,
 * a and b must be malloc'd */
{
    int ret, p, q, i;
    size_t nsz = (asz+bsz)*type;
    void *na = malloc(nsz);

    for (p = 0, q = 0, i = 0; p < asz && q < bsz; ++i) {
        ret = cmpf(a+p*type, b+q*type, type);
        if ((ret == 1) || (ret == 0)) {
            memcpy(na+i*type, a+p*type, type);
            ++p;
        } else if (ret == -1) {
            memcpy(na+i*type, b+q*type, type);
            ++q;
        }
    }

    int nszm = nsz/type;
    if (p == asz)
        memcpy(na+i*type, b+q*type, (bsz-q)*type);
    else if (q == bsz)
        memcpy(na+i*type, a+p*type, (asz-p)*type);

    free(a);
    free(b);
    return na;
}

void *Msort(void *a, int asz, size_t type, int(*cmpf)(void *x, void*y, size_t t))
/* out of place mergesort */
{
    if (asz == 1)
        return a;

    int mid = asz/2;
    int slz1, slz2;
    void *sl1 = slice(a, 0, mid-1, type, &slz1);
    void *sl2 = slice(a, mid, asz-1, type, &slz2);
    
    return merge(Msort(sl1, slz1, type, cmpf), slz1,
                 Msort(sl2, slz2, type, cmpf), slz2,
                 type, cmpf);
}

static int partition(void *a, int l, int r, size_t type, int(*cmpf)(void *x, void *y, size_t t))
{
    int i = l-1, j = r;
    void *v = a+r*type;
    
    for (;;) {
        while (cmpf(a+(++i)*type, v, type) == 1);
        while (cmpf(v, a+(--j)*type, type) == 1) 
            if (j == l) break;
        if (i >= j) break;
        swap(a+i*type, a+j*type, type);
    }
    swap(a+i*type, a+r*type, type);

    return i;
}

void Qsort(void *a, int l, int r, size_t type, int(*cmpf)(void *x, void *y, size_t t))
/* in place quicksort */
{
    int i;
    if (r <= l) return;
    i = partition(a, l, r, type, cmpf);
    Qsort(a, l, i-1, type, cmpf);
    Qsort(a, i+1, r, type, cmpf);
}

void Ssort(void *a, int asz, size_t type, int(*cmpf)(void *x, void *y, size_t t))
/* in place selection sort */
{
    int i, j;
    int m = 0; // max or min
    
    for (i = 0; i < asz; ++i) {
        for (j = i, m = i; j < asz; ++j)
            if (cmpf(a+j*type, a+m*type, type) == 1)
                m = j;
        swap(a+i*type, a+m*type, type); 
    }
}

#define v_sort(vec, cmpf) Qsort((vec)->mem, 0, ((vec)->capacity/(vec)->type)-1, (vec)->type, (cmpf))

#endif

