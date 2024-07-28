#include <ctimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

typedef int DType;

void matmul(int n, DType *A, DType *B, DType *C);
void check(int n, DType *A, DType *B, DType *C);

