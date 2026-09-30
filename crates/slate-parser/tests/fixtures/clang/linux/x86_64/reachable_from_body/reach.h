typedef unsigned long reach_size_t;
typedef long          reach_cast_t;
typedef int           reach_unused_t;

struct reach_point {
  int x;
  int y;
};
struct reach_unused {
  int z;
};

enum reach_color { REACH_RED, REACH_GREEN };
enum reach_unused_enum { REACH_UNUSED };

int reach_called(struct reach_point *point);
int reach_unused_fn(void);

extern int reach_counter;
extern int reach_unused_global;
