#include <stdio.h>
#include <string.h>

int main(void) {
  remove("slate_stdio_gets_loop_unsupported.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_unsupported.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("one\n", f);
  fputs("two\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_unsupported.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[64];
  int  count = 0;
  while (fgets(line, sizeof line, g) != NULL) {
    // @lowering-begin
    // @rewrite-begin
    count += (int)strlen(line);
    // @rewrite-end
    // @lowering-end
  }
  fclose(g);
  printf("%d\n", count);
  remove("slate_stdio_gets_loop_unsupported.tmp");
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = line.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = line.as_mut_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = count;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG: count = {{__v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = line.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = line.as_mut_ptr() as *mut u8;
// REWRITES-DAG: let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// REWRITES-DAG: count += {{__v[0-9]+}} as i32;
// SLATE-FILECHECK-END rewrites
