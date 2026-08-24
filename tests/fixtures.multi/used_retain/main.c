static int used_only __attribute__((used))                 = 11;
static int used_and_retained __attribute__((used, retain)) = 22;
static int retain_only __attribute__((retain))             = 33;

int main(void) { return 0; }
// LOWERING-DAG: #![feature(used_with_arg)]
// LOWERING-DAG: #[used]
// LOWERING-DAG: static mut used_only: i32 = 11;
// LOWERING: #[used(linker)]
// LOWERING-NEXT: static mut used_and_retained: i32 = 22;
// LOWERING-NOT: retain_only
