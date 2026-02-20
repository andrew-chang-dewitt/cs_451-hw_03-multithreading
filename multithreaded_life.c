/*
    Conway's Game of Life (and rough edges in the code for teaching purposes)
    Copyright (C) 2025 Nik Sultana

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


gcc life.c -o life # -pg
valgrind -s --leak-check=full --show-leak-kinds=all \
  ./life -s 20 -c 20 \
  -i 010000000001000000000010000000010000000011100000000100000000

./life -s 3 -c 3 -i 011001010

./life -s 5 -c 10 -i 011001010110101111

./life -s 20 -c 10 \
  -i 011001010110101111011001010110101111010111101010101111100110111

# Glider and blinker:
./life -s 20 -c 20 \
  -i 010000000001000000000010000000010000000011100000000100000000

# Bee-hive and loaf:
./life -s 10 -c 100 \
  -i 010000000001000000000010000000010000000011100000000100000000
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef ARGH
#include "args.h"
#endif
#ifndef STEPH
#include "step.h"
#endif
#ifndef WORLDH
#include "world.h"
#endif

int main(int argc, char *const *argv) {
  Config cfg = parse_args(argc, argv);
  char *world_history = init_world(cfg.size, cfg.cycles, cfg.init_world);

  print_world(world_history, cfg.size, 0);
  printf("\n");
  char *cur_step;
  char *new_step = world_history;
  unsigned long part_start;
  unsigned long part_end;

  // TODO: make this actually multithreaded!
  // - needs to allow each thread to run its _own_ loop, calling step_part for
  //   each step until all cycles completed, then calling pthread_exit
  //   in that thread...
  // - will require first making an array of pthread_t's created w/
  //   pthread_create (one thread for each partition), passing a function
  //   containing the loop logic & a part start offset & part end offset
  // - main thread will need to coordinate that each thread waits for the
  //   others to be ready before moving on to the next step; this might be
  //   doable using pthread_barrier_wait?
  for (unsigned long i = 1; i < cfg.cycles; i++) {
    // move step pointers ahead one step
    cur_step = new_step;
    new_step = cur_step + (cfg.size * cfg.size);
    // & start part offset back beginning of step
    part_start = 0;

    for (unsigned long p = 0; p < cfg.num_parts; p++) {
      // lookup part end offset in parts size array
      part_end = part_start + cfg.parts[p];
      step_part(cur_step, part_start, part_end, cfg.size, new_step);
      // shift part end to start
      part_start = part_end;
    }

    print_world(world_history, cfg.size, i);
    printf("\n");
  }

  free(world_history);
  free(cfg.init_world);

  return EXIT_SUCCESS;
}
