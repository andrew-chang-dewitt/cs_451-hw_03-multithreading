**CS451: Introduction to Parallel and Distributed Computing** \
Spring 2026

# Assignment 3: Multi-threading

## Setup:

1. Create a FABRIC slice that contains a single node
   provisioned with Ubuntu 22.04.5.
2. On that node, install GCC, Valgrind, and gprof.

## Development:

3. Divide the Life's world into four equal quadrants, and adapt
   the Life program to apply the `step()` function in parallel
   for each quadrant by using multi-threading. Thus you should
   have five threads in all&mdash;one for each quadrant, and
   one for the invoked program. The invoked program runs the
   "main" thread (that spawns the quadrants' threads). The
   cells that border the quadrant are not computed by the
   quadrant's thread, since that computation requires
   synchronization with at least one other quadrant. Instead,
   the cells that border quadrants are computed by the main
   thread, which waits until all four threads complete before
   computing the border cells, and thus finishing the call to
   step().
4. Run the (updated) Life program for 100 cycles and time its
   execution.
5. Using Valgrind, check the (multi-threaded) Life binary for
   memory leaks.
6. Using gprof, profile the (multi-threaded) Life binary.

## Bonus: Variable granularity (+30%)

7. Copy your program into a new file and add a parameter G and
   code to set the granularity of each thread's scope. That is,
   the original Life program had `G=1`, and the Life program from
   step 3 had `G=4`.
8. By varying `G` you can increase the parallelism of your
   program– but doesn't that increase the number of
   synchronizations that are needed across borders? Investigate
   this trade-off by measuring the run-times when `G=1..10`. Use
   command `time` in bash to measure the run-time of each
   invocation.
9. Setting `G=10` and using `valgrind`, check the (multi-threaded)
   Life binary for memory leaks.
10. Setting `G=10` and using `gprof`, profile the (multi-threaded)
    Life binary.

## What to submit:

- A file called "multithreaded_life.c" containing the code of
  your updated Life program from step 3, and any other ".c" and
  ".h" files that accompany the partitioned program.
- _(Bonus only)_ A file called "granular_multithreaded_life.c"
  containing the code of your updated Life program from step 6.
- A set of files named "a.out", "a.err", "b.out", etc,each
  containing the contents specified below:
  - **a:** The output from the compilation of your program from
    step 3.
  - **b:** Theoutput from running your program from step 4.
  - **c:** The output from step 5. **(Penalty:-10% if there are
    memory leaks)**
  - **d:** The output from step 6.
  - **e:** _(Bonus only)_ The output from the compilation of
    your program from step 7.
  - **f:** _(Bonus only)_ The output from step 8.
  - **g:** _(Bonus only)_ The output from step 9.
  - **h:** _(Bonus only)_ The output from step 10.

Where files ending in ".out" should contain the standard output,
and files ending in ".err" should contain the error output.
