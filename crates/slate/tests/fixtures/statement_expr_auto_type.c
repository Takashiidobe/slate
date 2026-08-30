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
// LOWERING-DAG: let mut exchange_pointer: *mut i64 = std::ptr::null_mut();
// LOWERING-DAG: let mut exchange_value: i64 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = index;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG: index = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG: exchange_pointer = std::ptr::addr_of_mut!(values[({{_v[0-9]+}} as usize)]);
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i64 = exchange_pointer;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: exchange_value = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 25;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i64 = exchange_pointer;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = exchange_value;
// LOWERING-DAG: tmp = {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = tmp;
// LOWERING-DAG: old = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%ld %ld %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = old;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = values[({{_v[0-9]+}} as usize)];
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = index;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: {
// REWRITES-DAG: let mut exchange_pointer: *mut i64 = std::ptr::null_mut();
// REWRITES-DAG: let mut exchange_value: i64 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = index;
// REWRITES-DAG: index = {{_v[0-9]+}} + 1;
// REWRITES-DAG: exchange_pointer = std::ptr::addr_of_mut!(values[(({{_v[0-9]+}} as i64) as usize)]);
// REWRITES-DAG: exchange_value = unsafe { *exchange_pointer };
// REWRITES-DAG: unsafe {
// REWRITES-DAG: *exchange_pointer = 25;
// REWRITES-DAG: }
// REWRITES-DAG: tmp = exchange_value;
// REWRITES-DAG: }
// REWRITES-DAG: old = tmp;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%ld %ld %d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, old, values[({{_v[0-9]+}} as usize)], index) };
// SLATE-FILECHECK-END rewrites
