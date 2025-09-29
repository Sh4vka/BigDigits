#include <stdio.h>
#include <stdlib.h>
#include "bigdigits.h"

int main() {
    FILE* file = fopen("../numbers.txt", "r");
    if (!file) {
        printf("Error file!");
        return 1;
    }

    char buffer1[1000], buffer2[1000];
    fscanf(file, "%s %s", buffer1, buffer2);
    fclose(file);

    BigDigits* num1 = create_from_char(buffer1);
    BigDigits* num2 = create_from_char(buffer2);

    printf("Первое число: \n");
    big_print(num1);
    printf("\n");

    printf("Второе число: \n");
    big_print(num2);
    printf("\n");

    BigDigits* sum = big_add(num1, num2);
    printf("Результат суммы: \n");
    big_print(sum);
    printf("\n");

    BigDigits* sub = big_sub(num1, num2);
    printf("Результат вычитания: \n");
    big_print(sub);
    printf("\n");

    BigDigits* pow = big_pow(num1, 10);
    printf("Результат степени: \n");
    big_print(pow);
    printf("\n");

    BigDigits* mul = big_mul(num1, num2);
    printf("Результат перемножения: \n");
    big_print(mul);
    printf("\n");

    BigDigits* g = big_gcd(num1, num2);
    BigDigits* l = big_lcm(num1, num2);

    printf("GCD: "); big_print(g); printf("\n");
    printf("LCM: "); big_print(l); printf("\n");
}