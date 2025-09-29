#ifndef BIGDIGITS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 1000000000 
#define BLOCK 9

typedef enum { 
    POSITIVE, 
    NEGATIVE 
} signs_t;

typedef struct {
    int* digits; 
    int size;
    signs_t sign;
} BigDigits;

BigDigits* init_digits();
BigDigits* create_from_char(const char* str);
int big_compare_abs(const BigDigits* a, const BigDigits* b);
BigDigits* create_copy(const BigDigits* num);
BigDigits* add_abs(const BigDigits* a, const BigDigits* b);
BigDigits* sub_abs(const BigDigits* a, const BigDigits* b);
BigDigits* big_add(const BigDigits* a, const BigDigits* b);
void big_print(const BigDigits* num);
BigDigits* big_sub(const BigDigits* a,const BigDigits* b);
BigDigits* big_mul(const BigDigits* a, const BigDigits* b);
BigDigits* big_pow(const BigDigits* a, int n);
BigDigits* big_div_big(BigDigits* a, BigDigits* b, BigDigits** remainder);
BigDigits* big_gcd(BigDigits* a, BigDigits* b);
BigDigits* big_lcm(BigDigits* a, BigDigits* b);

#endif