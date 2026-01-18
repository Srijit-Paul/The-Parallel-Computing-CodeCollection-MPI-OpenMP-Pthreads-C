# ⚡️ Parallel-Algorithms-Collection

![Language](https://img.shields.io/badge/language-C%2F%2FC%2B%2B-00599C.svg)
![MPI](https://img.shields.io/badge/MPI-Enabled-red.svg)
![OpenMP](https://img.shields.io/badge/OpenMP-Enabled-green.svg)
![Pthreads](https://img.shields.io/badge/Pthreads-Enabled-orange.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

**Master the Art of High-Performance Computing (HPC).**

This repository is a comprehensive "cookbook" of parallel algorithms implemented in **C/C++**. It features optimized solutions for Sorting, Graph Theory, and Linear Algebra using the three major parallel programming paradigms: **MPI** (Message Passing Interface), **OpenMP** (Shared Memory), and **Pthreads**.

## 📂 Repository Structure

The code is organized by parallelization technique. Click the file names below to view the source code.

### 1. 🔄 Sorting Algorithms
Efficient parallel sorting implementations demonstrating bitonic networks, divide-and-conquer, and odd-even swaps.

| Algorithm | MPI Implementation 🔴 | OpenMP Implementation 🟢 | Pthreads / Core 🟠 |
| :--- | :--- | :--- | :--- |
| **Bitonic Sort** | [Hypercube Bitonic Sort](./MPI/hypercube_bitonic_sort_207173.c) | [Bitonic Sort (C++)](./OpenMP/207173_bitonic_omp.cpp) | [Bitonic Sort](./Pthreads/bitonic_sort.c) |
| **Quick Sort** | [Quick Sort](./MPI/quicksort_207173.c) | [Quick Sort](./OpenMP/207173_quickshort_omp.c) | [Quick Sort](./Pthreads/Quicksort.c) |
| **Bubble / Odd-Even** | [Odd-Even Sort](./MPI/odd_even_207173.c) | [Odd-Even Sort](./OpenMP/207173_oddeven_omp.c) | [Odd-Even Transposition](./Pthreads/odd_even_transportation.c) |
| **Shell Sort** | [Shell Sort](./MPI/Shellsort_207173.c) | [Shell Sort (C++)](./OpenMP/207173_shell_omp.cpp) | [Shell Sort](./Pthreads/shellsort.c) |
| **Radix Sort** | -- | [Radix Sort](./OpenMP/207173_radixshort_omp.c) | [Radix Sort](./Pthreads/radixsort.c) |
| **Bucket Sort** | [Bucket Sort](./MPI/bucketsort_207173.c) | [Bucket Sort](./OpenMP/207173_bucketsort_omp.c) | [Bucket Sort](./Pthreads/bucketsort.c) |
| **Merge Sort** | [Merge Sort](./MPI/mergesort_207173.c) | -- | -- |
| **Heap Sort** | [Heap Sort](./MPI/heap_sort_207173.c) | -- | -- |
| **Insertion Sort** | [Insertion Sort](./MPI/insertion_sort_207173.c) | -- | -- |
| **Selection Sort** | [Selection Sort](./MPI/selectionsort_207173.c) | -- | -- |

### 2. 🌐 Graph Algorithms
Distributed solvers for graph traversal, shortest paths, and spanning trees.

| Algorithm | MPI Implementation 🔴 | OpenMP Implementation 🟢 | Pthreads / Core 🟠 |
| :--- | :--- | :--- | :--- |
| **Dijkstra (SSSP)** | [Dijkstra Algorithm](./MPI/Dijkstra_SSSP_s%20Algorithm.c) | [Dijkstra OMP](./OpenMP/207173_dij_omp.c) | [Dijkstra](./Pthreads/Dijkstra.c) |
| **Floyd-Warshall (APSP)** | [Floyd-Warshall](./MPI/All-Pairs%20Shortest%20Paths_Floyd%20Warshall_207173.c) | [Floyd-Warshall OMP](./OpenMP/207173_floydwarshal_omp.c) | [Floyd](./Pthreads/floyd.c) |
| **Prim's (MST)** | [Prim's MST](./MPI/Minimum%20Spanning%20Tree_Prim_s%20Algorithm_207173.c) | [Min Spanning Tree](./OpenMP/207173_minspann_omp.c) | [MST](./Pthreads/Minimum_spanning_tree.c) |
| **Connected Components** | [Connected Comp](./MPI/Connected%20Components_207173.c) | [Connected Comp](./OpenMP/207173_ConnectedComponents_omp.c) | [Connected Comp](./Pthreads/connected_comp.c) |
| **Single Source Shortest**| -- | [SSSP](./OpenMP/207173_singlesource_shortest.c) | [SSSP](./Pthreads/Single_source_shortest.c) |
| **Transitive Closure** | -- | [Transitive Closure](./OpenMP/207173_transtive_omp.cpp) | [Transitive Closure](./Pthreads/transative_closure.c) |

### 3. 🧮 Linear Algebra & Matrices
Foundational algorithms for scientific computing and AI.

| Algorithm | MPI Implementation 🔴 | OpenMP Implementation 🟢 | Pthreads / Core 🟠 |
| :--- | :--- | :--- | :--- |
| **Matrix Multiplication** | [Cannon's Algo](./MPI/Matrix%20Multiplication_Cannon_s%20Algorithm_207173.c) <br> [Scatter Gather](./MPI/Matrix%20Multiplication_Scatter%20Gather_207173.c) <br> [Striping](./MPI/Matrix%20Multiplication_Striping%20Data%20Decomposition_207173.c) | [Matrix Mul](./OpenMP/207173_matrixmul_omp.c) <br> [Cannon's](./OpenMP/207173_Cannon's.c) | [Simple Mul](./Pthreads/Matrix_Matrix_multiplication_simple.c) <br> [Cannon](./Pthreads/canon.c) |
| **Gaussian Elimination** | [Gaussian Simple](./MPI/Gauss%20Elimination_Simple_207173.c) <br> [Partial Pivoting](./MPI/Gauss%20Elimination_Partial%20Pivoting_207173.c) | [Gaussian](./OpenMP/207173_gaussian_omp.c) | [Gaussian](./Pthreads/gaussian.c) |
| **Back Substitution** | -- | [Back Substitution](./OpenMP/207173_back_substitution.c) | [Back Substitution](./Pthreads/backsubstitution.c) |
| **Partial Pivoting** | -- | [Partial Pivoting](./OpenMP/207173_partial_pivoting.c) | [Partial Pivoting](./Pthreads/partialpivoting.c) |

### 4. 🛠 System & Misc Algorithms

| Algorithm | MPI Implementation 🔴 | OpenMP Implementation 🟢 | Pthreads / Core 🟠 |
| :--- | :--- | :--- | :--- |
| **DNS Algorithm** | [DNS Algo](./MPI/DNS_Algorithm_207173.c) | -- | [DNS](./Pthreads/dns.c) |
| **Maximal Independent Set**| -- | [MIS](./OpenMP/207173_maximalindependent_omp.c) | [MIS](./Pthreads/maximal_ind_set.c) |

---

## 🚀 Getting Started

### Prerequisites
* **GCC/G++ Compiler**
* **MPI Implementation** (MPICH or OpenMPI)
* **OpenMP** (Usually included with GCC)
* **Pthreads** (Standard POSIX library)

### Compilation & Execution Guide

#### 1. Running MPI Programs (Distributed Memory)
Use `mpicc` to compile and `mpirun` to execute on multiple processes.

```bash
# Compile
mpicc MPI/hypercube_bitonic_sort_207173.c -o bitonic_mpi -lm

# Run with 4 Processes
mpirun -np 4 ./bitonic_mpi

```
<details> <summary><strong>🔍 Search Keywords</strong></summary>

#ParallelComputing #HighPerformanceComputing #HPC #DistributedSystems #Supercomputing #ComputerScience #CProgramming #Cpp #MPI #OpenMP #Pthreads #MessagePassingInterface #SharedMemory #Multithreading #Concurrency #POSIXThreads #Algorithms #DataStructures #SortingAlgorithms #GraphTheory #MatrixMultiplication #LinearAlgebra #BitonicSort #QuickSort #Dijkstra #FloydWarshall #GaussianElimination #ScientificComputing #Optimization #CodeOptimization #PerformanceTuning #SoftwareEngineering #GitHub #OpenSource #Coding #Programmer #DevLife #TechCommunity #CodeNewbie #ComputerEngineering #CSStudent #AlgorithmDesign #ComputationalScience #TechEducation #ProgrammingLife #SourceCode #ParallelAlgorithms #Developers #CodingResources #StemEducation

</details>

⭐ Star this repo if you found it helpful!
