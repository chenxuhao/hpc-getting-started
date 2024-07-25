## Quick Start

On NCSA Delta machine, enable PAPI

```
$ module load papi
$ papi_avail
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/cray/pe/papi/7.0.1.2/lib
```

Build and run matrix multiplication

```
$ make
$ ./matmul 
start kernel
Time(matmul) = 6.092509699 sec
L1 data Cache Hits: 1119882014
L1 data Cache Accesses: 19852791210
L1 Data Cache Miss Rate: 5.64%
```
