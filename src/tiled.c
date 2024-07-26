#include "matmul.h"

#if 1
void matmul(int n, DType *A, DType *B, DType *C) {
  #pragma omp parallel for
  for (int i = 0; i < n; ++i)
    for (int k = 0; k < n; ++k)
      for (int j = 0; j < n; ++j)
        C[i*n+j] += A[i*n+k] * B[k*n+j];
}
#else

void matmul(int n, DType *A, DType *B, DType *C) {
  const int s = 32;
  #pragma omp parallel for
  for (int ih = 0; ih < n; ih += s)
    for (int jh = 0; jh < n; jh += s)
      for (int kh = 0; kh < n; kh += s)
        for (int il = 0; il < s; ++il)
          for (int kl = 0; kl < s; ++kl)
            for (int jl = 0; jl < s; ++jl)
              C[(ih+il)*n+jh+jl] += A[(ih+il)*n+kh+kl] * B[(kh+kl)*n+jh+jl];
}

#endif