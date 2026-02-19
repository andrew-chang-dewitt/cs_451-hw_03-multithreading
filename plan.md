this'll be something like this for the main fn:

```
// ... same arg parsing & setup
//
// calc part_size from G as size divided by int sqrt of G
// make array of part start offsets for faster lookup later
// make arr of border cell offsets & store num of border cells
// for each step:
//     for each part (1..G):
//         call part_step(...), modifying next step state in place
//     for each border cell:
//         apply life algo modifying next step state in place
//     print newly created step state
//
// ...finish as before
```

which requires replacing `step(size, step_num)` w/ similar fn
`part_step(world_size, step_num, part_size, part_num)` that does the following:

```
// get ptr to start of part using part_get_start()
// increment part start ptr by 1 row (to skip top border
// iterate through part_size - 2 rows (skipping bottom border:
//     iterate from cell 1 through cell part_size - 1 (skipping left & right borders):
//         ...apply life algo modifying associated part in place
```

some deeper exploration of how to calculate pats & merge results

```
## ex 1: 6 x 6 world, G = 4

part a: 0
part b: a + 3 = 3
part c: 0 + 6(3) = 18
part d: c + 3 = 21

a a a b b b
a a a b b b
a a a b b b
c c c d d d
c c c d d d
c c c d d d

## ex 2: 12 x 12 world, G = 16

part a: 0
part b: a + 3 = 3
part c: b + 3 = 6
part d: d + 3 = 9
part e: a + 9(4) = 36
part f: e + 3 = 39
part g: f + 3 = 42
part h: g + 3 = 45
part i: e + 9(4) = 72
part j: i + 3 = 75
part k: j + 3 = 78
part l: k + 3 = 81
part m: i + 9(4) = 108
part n: n + 3 = 111
part o: o + 3 = 114
part p: p + 3 = 117

a a a b b b c c c d d d
a a a b b b c c c d d d
a a a b b b c c c d d d
e e e f f f g g g h h h
e e e f f f g g g h h h
e e e f f f g g g h h h
i i i j j j k k k l l l
i i i j j j k k k l l l
i i i j j j k k k l l l
m m m n n n o o o p p p
m m m n n n o o o p p p
m m m n n n o o o p p p

// seems that finding the offsets for each part is
// something like this

parts_per_size = sqrt(G)
part_size = size / parts_per_size

unsigned int part_offsets[G] = Null
part_offsets[0] = 0 // always starts @ 0

for pt in 1..G:
    // part is either beginning a new row of parts
    if (0 == ((pt * part_size) % size)):
        part_offsets[pt] = part_offsets[pt - parts_per_size]
                           + (size * parts_per_size)
    // or is to the right of the previous part
    else:
        part_offsets[pt] = part_offsets[pt - 1] + parts_per_size


// finding border cell offsets

border_cell_count_per_part = (4 * part_size) - 4 // corners are duplicated in mult
border_cell_count = G * border_cell_count_per_part
unsigned int border_offsets[border_cell_count] = Null

// populate border offset array by
// iterating by top rows
// pushing border cell offsets to next idx
next_offset_idx = 0

for top in 0..parts_per_size:
    // note start of top row
    start = top * size
    // add all top border cells
    for i in 0..size:
        border_offsets[next_offset_idx] = start + i
        next_offset_idx++

    // add vert border cells
    i = 0
    j = 0
    // for each row between top & bottom
    while (j < (part_size - 2)):
        // update start of current row
        start += size
        // keep going until end of row
        while (i < parts_per_size * 2):
            // add left border cell
            border_offsets[next_offset_idx] = start + i
            // & right border cell
            border_offsets[next_offset_idx] = start + i + part_size - 1
            // then increment i to next left border
            i += part_size

    // move start to bot row
    start += + size
    // then add all bot border cells
    for i in 0..size:
        border_offsets[next_offset_idx] = start + i
        next_offset_idx++
```

for now might be able to just have all partition threads just clobber away at
the history state since they all operate on disjoint subsets of it, then only
when all done begin the merge loop for border calculations.

ideally, would like some way of orchestrating each step to signal main thread
that all parts of the next step are done so main thread can mege parts in
parallel
