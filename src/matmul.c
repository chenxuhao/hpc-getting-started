#include <ctimer.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024

//#define USE_PAPI
#ifdef USE_PAPI
#include <papi.h>
#endif

void matrixMultiply(int A[SIZE][SIZE], int B[SIZE][SIZE], int result[SIZE][SIZE]) {
  // Initialize the result matrix with zeros
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      result[i][j] = 0;
    }
  }

  // Perform matrix multiplication
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      for (int k = 0; k < SIZE; k++) {
        result[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

int main() {
  //int ver = PAPI_library_init(PAPI_VER_CURRENT);
  //printf("PAPI version is %d.%d\n", PAPI_VERSION_MAJOR(ver), PAPI_VERSION_MINOR(ver));

  int (*A)[SIZE] = malloc(sizeof(int) * SIZE * SIZE);
  int (*B)[SIZE] = malloc(sizeof(int) * SIZE * SIZE);
  int (*result)[SIZE] = malloc(sizeof(int) * SIZE * SIZE);

  // Initialize matrices A and B with example values
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      A[i][j] = i + j;
      B[i][j] = i - j;
    }
  }
#ifdef USE_PAPI
  if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
    fprintf(stderr, "PAPI library initialization error!\n");
    return 1;
  }

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
  matrixMultiply(A, B, result);

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
  printf("L1 data Cache Hits: %lld\n", values[0]);
  printf("L1 data Cache Accesses: %lld\n", values[1]);
  printf("L1 Data Cache Miss Rate: %.2f%%\n", (double)values[0] / values[1] * 100.0);
#endif
  // Optionally, print the result matrix
  // for (int i = 0; i < SIZE; i++) {
  //     for (int j = 0; j < SIZE; j++) {
  //         printf("%d ", result[i][j]);
  //     }
  //     printf("\n");
  // }

  free(A);
  free(B);
  free(result);

  return 0;
}

