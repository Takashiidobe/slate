// { dg-options "-std=gnu23" }
int main(void) {
  register int x asm("eax") = 5;
  __asm__ __volatile__("incl %0" : "+r"(x));
  return x;
}
