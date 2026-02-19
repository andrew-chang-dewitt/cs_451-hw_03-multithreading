#include <assert.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#ifndef ARGH
#include "args.h"
#endif

const Config default_config = {
    .size = 3, .cycles = 3,
    // .init_world is NULL
};

Config parse_args(const int argc, char *const *argv) {
  Config config = default_config;

  int option;
  while ((option = getopt(argc, argv, "s:i:c:")) != -1) {
    switch (option) {
    case 'i':
      config.init_world = malloc(strlen(optarg) + 1);
      strcpy(config.init_world, optarg);
      break;
    case 's':
      config.size = strtoul(optarg, NULL, 10);
      break;
    case 'c':
      config.cycles = strtoul(optarg, NULL, 10);
      break;
    default:
      // FIXME: print error message.
      exit(EXIT_FAILURE);
      break;
    }
  }

  assert(config.size > 1);
  assert(config.cycles > 1);

  return config;
}
