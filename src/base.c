#include "matmul.h"

// Perform matrix multiplication
/*
void matmul(int N, DType A[N][N], DType B[N][N], DType C[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}
*/

void matmul(int n, DType *A, DType *B, DType *C) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        C[i*n+j] += A[i*n+k] * B[k*n+j];
      }
    }
  }
}
