# multithreaded conway's game of life

a multithreaded implementation of conway's game of life. partitions the world
into equal number of rows (or nearly equal, sometimes a few will have 1 more
than the others), then uses a pthread to compute the steps for each partition
independently. synchronizes work between threads by having each wait until all
have completed a step before moving on to the next step using
`pthread_barrier_wait()` since computing the changes at the border between two
partitions requires both partition to have already completed the preceding
step.

> [!NOTE]
>
> both the requested files `./multithreaded_life.c` &
> `./granular_multithreaded_life.c` are included; however, a quick `diff -u` of
> them will show they are in fact the same. this is because instead of first
> implementing the multithreaded version with a fixed number of partition, I
> implemented the flexible version only, just with a default of 4 partitions
> (set in the `parse_args()` function found in `./args.c`).
