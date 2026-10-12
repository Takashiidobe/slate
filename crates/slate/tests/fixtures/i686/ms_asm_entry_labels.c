/* { dg-options "--flavor=msvc" } */
int printf(const char *, ...);

static int asm_to_c(int value) {
  __asm {
    mov eax, value
    cmp eax, 10
    jg big
  }
  return value;
big:
  return value + 1000;
}

static int c_to_asm(int value) {
  int result = 0;
  goto inside;
  __asm {
    mov result, 1
  inside:
    mov eax, value
    add result, eax
  }
  return result;
}

static int c_to_asm_loop(int n) {
  int total = 0;
  int i = 0;
again:
  if (i >= n)
    return total;
  goto step;
  __asm {
    add total, 100
  step:
    add total, 1
  }
  i++;
  goto again;
}

static int both(int value) {
  int result = 0;
  if (value > 0)
    goto entry;
  __asm {
    mov result, 5
  entry:
    add result, 7
    cmp value, 50
    jg far_away
  }
  return result;
far_away:
  return -result;
}

int main(void) {
  printf("%d %d %d %d\n", asm_to_c(4), asm_to_c(40), c_to_asm(9), c_to_asm_loop(3));
  printf("%d %d %d\n", both(0), both(7), both(70));
  return 0;
}
