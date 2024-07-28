DEBUG ?= 0
BIN = ../../bin/
CC := gcc 
CXX := g++ 
ICC := $(ICC_HOME)/icc
ICPC := $(ICC_HOME)/icpc
MPICC := mpicc
MPICXX := mpicxx
NVCC := nvcc
#NVCC := $(CUDA_HOME)/bin/nvcc
CLANG := $(CILK_HOME)/bin/clang
CLANGXX := $(CILK_HOME)/bin/clang++

INCLUDES := -I./
LIBS := -lgomp

CFLAGS    := -Wall -fopenmp
CXXFLAGS  := -Wall -fopenmp -std=c++17
ICPCFLAGS := -Wall -qopenmp

ifeq ($(DEBUG), 1)
    CFLAGS += -g -O0 
    CXXFLAGS += -g -O0 
    NVFLAGS += -G
else
    CFLAGS += -O3 
    CXXFLAGS += -O3 
    NVFLAGS += -O3 -w
endif

CFLAGS += –ffast-math -march=native

ifeq ($(PAPI), 1)
CFLAGS += -DUSE_PAPI
CXXFLAGS += -DUSE_PAPI
INCLUDES += -I$(PAPI_HOME)/include
LIBS += -L$(PAPI_HOME)/lib -lpapi
endif

all: matmul_base matmul_tiled

matmul_base:
	$(CC) $(CFLAGS) matmul.c base.c $(INCLUDES) $(LIBS) -o matmul_base

matmul_tiled:
	$(CC) $(CFLAGS) matmul.c tiled.c $(INCLUDES) $(LIBS) -o matmul_tiled

clean:
	rm matmul_base matmul_tiled
