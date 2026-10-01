#include <stdio.h>

struct Flags {
  unsigned  x : 3;
  unsigned  set_x : 3;
  int       a : 4;
  unsigned  A : 4;
  int       type : 5;
  int       : 4;
  _Bool     ready : 1;
  char      tag;
  int       _flags2 : 24;
  long long wide : 40;
  _Bool     last : 1;
  int       : 0;
  short     after;
};

union Overlay {
  unsigned whole;
  struct {
    unsigned low : 12;
    unsigned high : 20;
  } parts;
  int signed_bits : 7;
};

static struct Flags global = {5, 2, -3, 9, -11, 1, 'g', -200, -5000000000LL, 1, 77};

static void bump(struct Flags *flags) {
  flags->a += 3;
  flags->wide++;
  flags->_flags2 = flags->_flags2 * 2;
  flags->ready = !flags->ready;
}

int main(void) {
  struct Flags local = {0};
  local.x       = 9;
  local.set_x   = 7;
  local.a       = -8;
  local.A       = 15;
  local.type    = 15;
  local.ready   = 1;
  local.tag     = 't';
  local._flags2 = 0x7fffff;
  local.wide    = 549755813887LL;
  local.last    = 1;
  local.after   = -2;
  int assigned  = (local.x = 12);
  printf("%d %u %u %d %u %d %d %c %d %lld %d %d %d\n", assigned, local.x, local.set_x,
         local.a, local.A, local.type, local.ready, local.tag, local._flags2,
         local.wide, local.last, local.after, (int)sizeof(struct Flags));
  bump(&local);
  bump(&global);
  printf("%d %lld %d %d\n", local.a, local.wide, local._flags2, local.ready);
  printf("%u %u %d %u %d %d %c %d %lld %d %d\n", global.x, global.set_x, global.a,
         global.A, global.type, global.ready, global.tag, global._flags2, global.wide,
         global.last, global.after);

  union Overlay overlay;
  overlay.whole = 0xabcdef12u;
  printf("%x %x %d\n", overlay.parts.low, overlay.parts.high, overlay.signed_bits);
  overlay.parts.high = 0x12345;
  overlay.signed_bits = -1;
  printf("%x %d\n", overlay.whole, (int)sizeof(union Overlay));

  fprintf(stdout, "%s\n", "file ok");
  return 0;
}
