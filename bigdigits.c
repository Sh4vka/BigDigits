#include "bigdigits.h"

BigDigits* init_digits() {
    BigDigits* num = malloc(sizeof(BigDigits));
    num->digits = malloc(sizeof(int));
    num->digits[0] = 0;
    num->size = 1;            
    num->sign = POSITIVE;
    return num;
}

BigDigits* create_from_char(const char* str) {
    BigDigits* num = init_digits();
    
    int start = 0;

    if (str[0] == '-') {
        num->sign = NEGATIVE;
        start = 1;
    }

    while (str[start] == '0' && str[start+1] != '\0') 
        start++;

    int len = strlen(str) - start;
    if (len == 0) 
        return init_digits();

    num->size = (len + BLOCK - 1)/BLOCK;
    num->digits = (int*)calloc(num->size, sizeof(int));

    for (size_t i = 0; i < len; i++) {
        char c = str[strlen(str) - 1 - i];
        int _digit = c - '0';
        if (_digit < 0 || _digit > 9) {
            free(num->digits);
            free(num);
            return init_digits();
        }
        int power = i % BLOCK;
        int block_index = i / BLOCK;
        int multiplier = 1;
        for (int j = 0; j < power; j++) multiplier *= 10;
        num->digits[block_index] += _digit * multiplier;
    }
    return num;
}

int big_compare_abs(const BigDigits* a, const BigDigits* b) {
    if (a->size > b->size) return 1;
    if (a->size < b->size) return -1;
    for (int i = a->size - 1; i >= 0; i++) {
        if(a->digits[i] > b->digits[i]) return 1;
        if(a->digits[i] < b->digits[i]) return -1;
    }
    return 0;
}

BigDigits* create_copy(const BigDigits* num) {
    BigDigits* copy = (BigDigits*)malloc(sizeof(BigDigits));
    copy->size = num->size;
    copy->sign = num->sign;

    copy->digits = (int*)malloc(copy->size * sizeof(int));
    for(int i = 0; i < copy->size; i++) {
        copy->digits[i] = num->digits[i];
    }
    return copy;
}

BigDigits* add_abs(const BigDigits* a, const BigDigits* b) {
    int max_size = (a->size > b->size) ? a->size : b->size;
    BigDigits* result = (BigDigits*)malloc(sizeof(BigDigits));
    result->digits = (int*)calloc(max_size+1, sizeof(int));
    result->size = max_size;
    result->sign = POSITIVE;

    int carry = 0;
    for(int i = 0; i < max_size; i++) {
        int sum = carry;
        if(i < a->size) sum += a->digits[i];
        if(i < b->size) sum += b->digits[i];
        result->digits[i] = sum % BASE;
        carry = sum / BASE;
    }

    if(carry > 0) {
        result->size++;
        result->digits = realloc(result->digits, result->size * sizeof(int));
        result->digits[result->size - 1] = carry;
    }

    return result;
}

BigDigits* sub_abs(const BigDigits* a, const BigDigits* b) {
    BigDigits* result = create_copy(a);
    int borrow = 0;
    for (size_t i = 0; i < result->size; i++) {
        result->digits[i] -= borrow;
        borrow = 0;

        if (i < b->size) result->digits[i] -= b->digits[i];
        if (result->digits[i] < 0) {
            result->digits[i] += BASE;
            borrow = 1;
        }
    }
    while(result->size > 1 && result->digits[result->size - 1] == 0) result->size--;
    
    result->digits = realloc(result->digits, result->size * sizeof(int));
    
    if(result->size == 1 && result->digits[0] == 0) result->sign = POSITIVE;

    return result;
}

BigDigits* big_add(const BigDigits* a, const BigDigits* b) {
    if (a->sign == b->sign) {
        BigDigits* result = add_abs(a, b);
        result->sign = a->sign;
        return result;
    } else {
        int cmp = big_compare_abs(a, b);
        if (cmp == 0) return init_digits();
        else if (cmp > 0) {
            BigDigits* result = sub_abs(a, b);
            result->sign = a->sign;
            return result;
        } else {
            BigDigits* result = sub_abs(b, a);
            result->sign = b->sign;
            return result;
        }
    }
}

void big_print(const BigDigits* num) {
    if (num->sign == NEGATIVE) {
        printf("-");
    }
    
    printf("%d", num->digits[num->size - 1]);
    
    for (int i = num->size - 2; i >= 0; i--) {
        printf("%09d", num->digits[i]);
    }
}

BigDigits* big_sub(const BigDigits* a,const BigDigits* b) {
    if (a->sign != b->sign) {
        BigDigits* tmp = create_copy(b);
        tmp->sign = a->sign;
        BigDigits* res = big_add(a, tmp);
        free(tmp->digits); free(tmp);
        return res;
    } else {
        int cmp = big_compare_abs(a, b);
        if (cmp == 0) return init_digits();
        else if (cmp > 0) {
            BigDigits* res = sub_abs(a, b);
            res->sign = a->sign;
            return res;
        } else {
            BigDigits* res = sub_abs(b, a);
            res->sign = (a->sign == POSITIVE ? NEGATIVE : POSITIVE);
            return res;
        }
    }
}

BigDigits* big_mul(const BigDigits* a, const BigDigits* b) {
    BigDigits* res = malloc(sizeof(BigDigits));
    res->size = a->size + b->size;
    res->digits = calloc(res->size, sizeof(int));
    res->sign = (a->sign == b->sign) ? POSITIVE : NEGATIVE;

    long long carry;
    for (int i = 0; i < a->size; i++) {
        carry = 0;
        for (int j = 0; j < b->size || carry; j++) {
            long long cur = res->digits[i+j] +
                            (long long)a->digits[i] * (j < b->size ? b->digits[j] : 0) + carry;
            res->digits[i+j] = (int)(cur % BASE);
            carry = cur / BASE;
        }
    }

    while (res->size > 1 && res->digits[res->size-1] == 0) res->size--;

    return res;
}

BigDigits* big_pow(const BigDigits* a, int n) {
    BigDigits* res = create_from_char("1");
    BigDigits* base = create_copy(a);

    while (n > 0) {
        if (n & 1) {
            BigDigits* tmp = big_mul(res, base);
            free(res->digits); free(res);
            res = tmp;
        }
        BigDigits* tmp2 = big_mul(base, base);
        free(base->digits); free(base);
        base = tmp2;
        n >>= 1;
    }
    return res;
}

BigDigits* big_div_big(BigDigits* a, BigDigits* b, BigDigits** remainder) {
    if (b->size == 1 && b->digits[0] == 0) {
        printf("Error. Divided zero!\n");
        exit(1);
    }

    if (big_compare_abs(a, b) < 0) {
        BigDigits* q = init_digits(); 
        *remainder = create_copy(a);
        return q;
    }

    BigDigits* x = create_copy(a);
    BigDigits* y = create_copy(b);
    x->sign = y->sign = POSITIVE;

    BigDigits* q = init_digits();
    free(q->digits);
    q->digits = (int*)calloc(x->size, sizeof(int)); 
    q->size = x->size;

    BigDigits* cur = init_digits(); 
    for (int i = x->size - 1; i >= 0; i--) {
        if (!(cur->size == 1 && cur->digits[0] == 0)) {
            cur->digits = (int*)realloc(cur->digits, (cur->size + 1) * sizeof(int));
            for (int j = cur->size; j > 0; j--)
                cur->digits[j] = cur->digits[j - 1];
            cur->size++;
        }
        cur->digits[0] = x->digits[i];

        while (cur->size > 1 && cur->digits[cur->size - 1] == 0)
            cur->size--;

        int qdigit = 0;
        while (big_compare_abs(cur, y) >= 0) {
            BigDigits* tmp = big_sub(cur, y);
            free(cur->digits);
            *cur = *tmp;
            free(tmp);
            qdigit++;
        }
        q->digits[i] = qdigit;
    }

    while (q->size > 1 && q->digits[q->size - 1] == 0)
        q->size--;

    *remainder = cur;
    return q;
}

BigDigits* big_gcd(BigDigits* a, BigDigits* b) {
    BigDigits* x = create_copy(a);
    BigDigits* y = create_copy(b);

    x->sign = y->sign = POSITIVE;

    while (!(y->size == 1 && y->digits[0] == 0)) {
        BigDigits* remainder;
        BigDigits* q = big_div_big(x, y, &remainder);
        free(q->digits); free(q);
        free(x->digits); free(x);
        x = y;
        y = remainder;
    }
    return x;
}

BigDigits* big_lcm(BigDigits* a, BigDigits* b) {
    BigDigits* g = big_gcd(a, b);
    BigDigits* prod = big_mul(a, b);

    BigDigits* remainder;
    BigDigits* result = big_div_big(prod, g, &remainder);
    free(remainder->digits); free(remainder);
    if (result->sign == NEGATIVE) result->sign = POSITIVE;
    return result;
}

