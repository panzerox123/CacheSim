# Building a cache coherant Cache Access Simulator 
`cache_sim.c` is a simple cache simulator. There is a memory region along with an L1 Cache. The cache is replaced using a simple direct-mapped policy.

The program reads a file `input_0.txt` which consists of a set of instructions, which can be either Reads or Writes, and then executes them. 

Your job is to parallelize this simulator using OpenMP, i.e., turn the single core cache simulator into a multi-core cache simulator. This also means you will need to implement a simple cache coherancy protocol. Implement MESI protocol.

This assignment will be manually evaluated. Grading will be transparent and any loss of marks will be explained.

## Instruction type:
There are two instruction types that the simulator can run:
`RD <address>` and `WR <address> <value>`.

## Input format
The emulator will be fed multiple input files, `input_0.txt` to `input_n.txt` where `n` is the number of threads we support. Each of the files will be read by individual threads which will run the instructions. 

There is a global memory area `memory`, and each "cpu core" will have it's own cache area `c`. Make sure you understand the code and feel free to ask for any questions you have with the code.

## Output format
The output format should be a bunch of print statements:
```
Thread <thread_num>: RD <address>: <current_value>
Thread <thread_num>: WR <address>: <current_value>

```
