# ParallelConvexHull
Quick Hull parallel algorithm for Parallel Convex Hull computation using OpenMP.  
  ## CPU Specifications
  * Architecture: x86_64
  * CPU op-mode(s): 32-bit, 64-bit
  * Byte Order: Little Endian
  * CPU(s): 8
  * On-line CPU(s) list: 0-7
  * Thread(s) per core: 2
  * Core(s) per socket: 4
  * CPU family: 6
  * Model: 142
  * Model name: Intel(R) Core(TM) i5-8250U CPU @ 1.60GHz
  * NUMA node0 CPU(s): 0-7
  
  ![alt text](https://github.com/aman33459/ParallelConvexHull/blob/master/images/Screenshot%20from%202019-11-12%2020-55-16.png)
  
  ![alt text](https://github.com/aman33459/ParallelConvexHull/blob/master/images/Screenshot%20from%202019-11-12%2020-55-27.png)
  
  ![alt text](https://github.com/aman33459/ParallelConvexHull/blob/master/images/Screenshot%20from%202019-11-12%2020-56-23.png)

  ![alt text](https://github.com/aman33459/ParallelConvexHull/blob/master/images/Screenshot%20from%202019-11-12%2020-56-36.png)
 ## Speed Comparision:
 
   Number of Points | PARALLEL RUNNING TIME(in seconds) | SEQUENTIAL RUNNING TIME(in seconds)
  ------------ | ------------- | -------------
  1000 | 0.000049 | 0.000042
  1000 | 0.000026 | 0.000016
  1000 | 0.000196 | 0.000173
  10000 | 0.000296 | 0.000412
  10000 | 0.000128 | 0.000152
  10000 | 0.00209 | 0.0022
  100000 | 0.00114 | 0.00272
  100000 | 0.000336 | 0.00108
  100000 | 0.0208 | 0.203
  1000000 | 0.0197 | 0.0343
  1000000 | 0.00799 | 0.0112
  1000000 | 0.299 | 0.314
  10000000 | 0.233 | 0.394
  10000000 | 0.0845 | 0.113
  10000000 | 4.69 | 5.23
