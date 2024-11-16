#include "math.h"

uint32_t isqrt(float32_t number) {
    uint32_t i = 0;
    uint32_t power = 1;

    while ((float32_t)(i * i) <= number)
        i++;

    return i - 1;
}


float32_t sqrt(float32_t number) {
    float32_t guess = (float32_t)isqrt(number);

    for (uint8_t i = 0; i < __SQRT_ITERATIONS; i++)
        guess -= ((guess * guess - number) / (2 * guess)); 

    return guess;
}

float32_t pow(float32_t x, uint32_t n) {
    float32_t result = 1.00;
    for (uint32_t i = 0; i < n; i++)
        result *= x;

    return result; 
}

uint32_t factorial(uint32_t n) {
    uint32_t result = 1;

    for (uint32_t i = 2; i <= n; i++)
        result *= i;
    
    return result;
}

float32_t sin(float32_t x) {
    if ((M_PI / 2) < x && x <= M_PI)
        return sin(x - (M_PI / 2));

    if (M_PI < x && x <= (3/2)*M_PI)
        return -sin(x - M_PI);

    if ((3/2)*M_PI < x && x <= 2*M_PI)
        return -sin(x - (3/2)*M_PI);

    float32_t result = 0.0;

    for (uint8_t i = 0; i < __SIN_COS_ITERATIONS; i++) {
        float32_t term = (i % 2 == 0 ? 1 : -1) * (pow(x, 2 * i + 1) / factorial(2 * i + 1));
        result += term;
    }

    return result;
}

float32_t cos(float32_t x) {
    if ((M_PI / 2) < x && x <= M_PI)
        return -cos(x - (M_PI / 2));

    if (M_PI < x && x <= (3/2)*M_PI)
        return -cos(x - M_PI);

    if ((3/2)*M_PI < x && x <= 2*M_PI)
        return cos(x - (3/2)*M_PI);

    float32_t result = 0.0;

    for (uint8_t i = 0; i < __SIN_COS_ITERATIONS; i++) {
        float32_t term = (i % 2 == 0 ? 1 : -1) * (pow(x, 2 * i) / factorial(2 * i));
        result += term;
    }

    return result;
}