#define ARGH

typedef struct {
  unsigned long size;
  unsigned long cycles;
  char *init_world;
} Config;

Config parse_args(int argc, char *const *argv);
