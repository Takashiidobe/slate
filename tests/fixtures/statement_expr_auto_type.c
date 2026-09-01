#include <stdio.h>

#define EXCHANGE(pointer, replacement)                                        \
  ({                                                                          \
    __auto_type exchange_pointer = (pointer);                                 \
    __auto_type exchange_value = *exchange_pointer;                           \
    *exchange_pointer = (replacement);                                        \
    exchange_value;                                                           \
  })

int main(void) {
  long values[] = {4, 9, 16};
  int index = 0;
  // @lowering-begin
  // @rewrite-begin
  long old = EXCHANGE(&values[index++], 25L);
  printf("%ld %ld %d\n", old, values[0], index);
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: {
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = index;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG: index = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = values[({{_v[0-9]+}} as usize)];
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 25;
// LOWERING-DAG: values[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-DAG: tmp = {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = tmp;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%ld %ld %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = values[({{_v[0-9]+}} as usize)];
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = index;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: {
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = index;
// REWRITES-DAG: index = {{_v[0-9]+}} + 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = values[({{_v[0-9]+}} as usize)];
// REWRITES-DAG: values[({{_v[0-9]+}} as usize)] = 25;
// REWRITES-DAG: tmp = {{_v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: unsafe {
// REWRITES-DAG: printf(
// REWRITES-DAG: c"%ld %ld %d\n".as_ptr(),
// REWRITES-DAG: tmp,
// REWRITES-DAG: values[((0 as i64) as usize)],
// REWRITES-DAG: index,
// REWRITES-DAG: )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
