#include "matmul.h"
//#define USE_PAPI
#ifdef USE_PAPI
#include <papi.h>
#endif

void print_matrix(int n, DType *A);

int main(int argc, char* argv[]) {
  int N = 1024;
  if (argc > 1) N = atoi(argv[1]);
  DType *A = malloc(sizeof(DType) * N * N);
  DType *B = malloc(sizeof(DType) * N * N);
  DType *C = malloc(sizeof(DType) * N * N);

  // Initialize matrices A and B with example values
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i*N+j] = i + j;
      B[i*N+j] = i - j;
    }
  }

  // Initialize the result matrix with zeros
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      C[i*N+j] = 0;
    }
  }

#ifdef USE_PAPI
  if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
    fprintf(stderr, "PAPI library initialization error!\n");
    return 1;
  }
  //printf("PAPI version is %d.%d\n", PAPI_VERSION_MAJOR(ver), PAPI_VERSION_MINOR(ver));

  // PAPI variables
  int EventSet = PAPI_NULL;
  long long values[2];
  int retval;

  // Create an EventSet
  if ((retval = PAPI_create_eventset(&EventSet)) != PAPI_OK) {
    fprintf(stderr, "PAPI create eventset error: %s\n", PAPI_strerror(retval));
    exit(1);
  }

  // Add L1 data cache miss events
  if ((retval = PAPI_add_event(EventSet, PAPI_L1_DCM)) != PAPI_OK) {
    fprintf(stderr, "PAPI add event error: %s\n", PAPI_strerror(retval));
    exit(1);
  }

  // Add L1 data cache access events
  if ((retval = PAPI_add_event(EventSet, PAPI_L1_DCA)) != PAPI_OK) {
    fprintf(stderr, "PAPI add event error: %s\n", PAPI_strerror(retval));
    exit(1);
  }

  // Start counting events
  if ((retval = PAPI_start(EventSet)) != PAPI_OK) {
    fprintf(stderr, "PAPI start error: %s\n", PAPI_strerror(retval));
    exit(1);
  }
#endif
  printf("start kernel\n");
  ctimer_t t;
  ctimer_start(&t);

  // Perform matrix multiplication
  matmul(N, A, B, C);

  ctimer_stop(&t);
  ctimer_measure(&t);
  ctimer_print(t, "matmul");
#ifdef USE_PAPI
  // Stop counting events
  if ((retval = PAPI_stop(EventSet, values)) != PAPI_OK) {
    fprintf(stderr, "PAPI stop error: %s\n", PAPI_strerror(retval));
    exit(1);
  }

  // Calculate and print L1 data cache miss rate
  printf("L1 Data Cache Misses: %lld\n", values[0]);
  printf("L1 Data Cache Accesses: %lld\n", values[1]);
  printf("L1 Data Cache Miss Rate: %.2f%%\n", (double)values[0] / values[1] * 100.0);
#endif

  check(N, A, B, C);
  //print_matrix(N, C);

  free(A);
  free(B);
  free(C);

  return 0;
}

void print_matrix(int n, int *A) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", A[i*n+j]);
    }
    printf("\n");
  }
}

void check(int n, DType *A, DType *B, DType *C_to_check) {
  DType *C = malloc(sizeof(DType) * n * n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      C[i*n+j] = 0;
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        C[i*n+j] += A[i*n+k] * B[k*n+j];
      }
    }
  }

  bool correct = true;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (C_to_check[i*n+j] != C[i*n+j]) {
        correct = false;
        printf("C[%d][%d]=%d incorrect, should be %d\n", i, j, C_to_check[i*n+j], C[i*n+j]);
        break;
      }
    }
  }

  if (correct) printf("correct\n");
  else printf("wrong");
}

