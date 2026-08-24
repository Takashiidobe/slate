#include <stdio.h>

int main(void) {
  char  greeting[] = "hé";
  char  mutate[]   = "abc";
  char *p          = mutate;
  *p               = 'Z';
  printf("%s\n", greeting);
  printf("%c\n", mutate[0]);
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: println!("{}", "h\u{e9}");
// REWRITES-DAG: let mut mutate: [i8; 4] = [97, 98, 99, 0];
// REWRITES-DAG: mutate.as_mut_ptr()
// REWRITES-NOT: greeting
// REWRITES-NOT: mutate: &str
// REWRITES: {{^}}}
