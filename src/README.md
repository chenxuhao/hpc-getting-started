## Quick Start

On NCSA Delta machine, load PAPI

```
$ module load papi
$ papi_avail
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/cray/pe/papi/7.0.1.2/lib
```

Build and run matrix multiplication with PAPI enabled

```
$ make PAPI=1
$ ./matmul 
start kernel
Time(matmul) = 5.587797456 sec
L1 data Cache Hits: 1245134553
L1 data Cache Accesses: 3248585561
L1 Data Cache Miss Rate: 38.33%
```

To use cachegrind, we should disable PAPI

```
$ make
$ valgrind --tool=cachegrind ./matmul 
==990186== Cachegrind, a cache and branch-prediction profiler
==990186== Copyright (C) 2002-2017, and GNU GPL'd, by Nicholas Nethercote et al.
==990186== Using Valgrind-3.19.0 and LibVEX; rerun with -h for copyright info
==990186== Command: ./matmul
==990186== 
--990186-- warning: L3 cache found, using its data for the LL simulation.
start kernel
Time(matmul) = 28.890766424 sec
==990186== 
==990186== I   refs:      8,605,705,910
==990186== I1  misses:            1,306
==990186== LLi misses:            1,266
==990186== I1  miss rate:          0.00%
==990186== LLi miss rate:          0.00%
==990186== 
==990186== D   refs:      3,227,073,182  (2,148,596,042 rd   + 1,078,477,140 wr)
==990186== D1  misses:    1,075,199,426  (1,075,002,112 rd   +       197,314 wr)
==990186== LLd misses:          199,418  (        2,195 rd   +       197,223 wr)
==990186== D1  miss rate:          33.3% (         50.0%     +           0.0%  )
==990186== LLd miss rate:           0.0% (          0.0%     +           0.0%  )
==990186== 
==990186== LL refs:       1,075,200,732  (1,075,003,418 rd   +       197,314 wr)
==990186== LL misses:           200,684  (        3,461 rd   +       197,223 wr)
==990186== LL miss rate:            0.0% (          0.0%     +           0.0%  )
```
