#include <stdio.h>

int counter = 4;
int zeroed;
int numbers[4] = {1, 2};

struct Pair {
  int left;
  int right;
};

struct Pair pair = {3, 5};

static int adjust(int by) {
  counter    += by;
  zeroed     += counter;
  numbers[2]  = zeroed - numbers[0];
  pair.right += numbers[1];
  return pair.left + pair.right;
}

int main(void) {
  printf("%d\n", adjust(6));
  printf("%d %d %d\n", counter, zeroed, numbers[2]);
  return 0;
}
// REWRITES-DAG: static mut counter: i32 = 4;
// REWRITES-DAG: static mut zeroed: i32 = 0;
// REWRITES-DAG: static mut numbers: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([1, 2, 0, 0]);
// REWRITES-DAG: static mut pair: Pair = Pair { left: 3, right: 5 };
// REWRITES-DAG: (unsafe { pair.left }) + unsafe { pair.right }
// REWRITES-DAG: counter = (unsafe { counter }) + by;
// REWRITES-DAG: (*numbers)[2] =
// REWRITES-DAG: pair.right = (unsafe { pair.right }) + unsafe { (*numbers)[1] };
