#include <stdio.h>

int square(int x);
int cube(int x);

// @rewrite-fn-begin
int main(void) {
  printf("%d %d\n", square(6), cube(4));
  return 0;
}
// @rewrite-fn-end

// LOWERING-DAG: mod math;
// LOWERING-DAG: use crate::math::square;
// LOWERING-DAG: use crate::math::cube;
// LOWERING-NOT: fn square
// LOWERING-NOT: fn cube

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     println!("{} {}", square(6), cube(4));
// REWRITES-DAG:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// SLATE-FILECHECK-END rewrites-aarch64-gnu
