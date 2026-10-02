#include <stdio.h>

union Interrupt { volatile int flag; double padding; };
struct State {
  int before;
  union { volatile int interrupted; double padding; };
  volatile unsigned short counters[3];
  volatile int *pointer;
};
static volatile int global;
static int calls;

static struct State *once(struct State *state) { ++calls; return state; }

int main(void) {
  union Interrupt interrupt = {0};
  struct State state = {0};
  int target = 7;
  state.pointer = &target;
  once(&state)->interrupted = 17;
  state.counters[2] = 100;
  state.counters[2] += 23;
  *state.pointer = 11;
  const volatile union Interrupt *view = &interrupt;
  interrupt.flag = 9;
  global = view->flag + state.interrupted;
  if (state.counters[2] != 123 || target != 11 || global != 26 || calls != 1) return 1;
  volatile struct State *all = &state;
  all->before = 31;
  if (all->before != 31) return 2;
  printf("%d %u %d %d\n", global, state.counters[2], target, all->before);
  return 0;
}
