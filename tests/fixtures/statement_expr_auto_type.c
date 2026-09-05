#include <stdio.h>

#define EXCHANGE(pointer, replacement)                                         \
  ({                                                                           \
    __auto_type exchange_pointer = (pointer);                                  \
    __auto_type exchange_value   = *exchange_pointer;                          \
    *exchange_pointer            = (replacement);                              \
    exchange_value;                                                            \
  })

int main(void) {
  long values[] = {4, 9, 16};
  int  index    = 0;
  // @lowering-begin
  // @rewrite-begin
  long old      = EXCHANGE(&values[index++], 25L);
  printf("%ld %ld %d\n", old, values[0], index);
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = index;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:     index = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = values[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 25;
// LOWERING-DAG:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-DAG:     tmp = {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: let {{__v[0-9]+}}: i64 = tmp;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%ld %ld %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%ld %ld %d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG: let {{__v[0-9]+}}: i64 = values[({{__v[0-9]+}} as usize)];
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = index;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = index;
// REWRITES-DAG: index = {{__v[0-9]+}} + 1;
// REWRITES-DAG: let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// REWRITES-DAG: let {{__v[0-9]+}}: i64 = values[({{__v[0-9]+}} as usize)];
// REWRITES-DAG: values[({{__v[0-9]+}} as usize)] = 25;
// REWRITES-DAG: tmp = {{__v[0-9]+}};
// REWRITES-DAG: unsafe { printf(c"%ld %ld %d\n".as_ptr(), tmp, values[0], index) };
// SLATE-FILECHECK-END rewrites
