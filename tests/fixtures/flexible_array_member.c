#include <stddef.h>
#include <stdlib.h>

struct FlexibleArray {
  size_t count;
  int    values[];
};

int main(void) {
  if (sizeof(struct FlexibleArray) != sizeof(size_t)) {
    return 1;
  }

  // @lowering-begin
  // @rewrites-begin
  struct FlexibleArray *flexible =
      malloc(sizeof(*flexible) + 3 * sizeof(flexible->values[0]));
  if (flexible == NULL) {
    return 2;
  }

  flexible->count = 3;
  for (size_t index = 0; index < flexible->count; ++index) {
    flexible->values[index] = (int)index + 1;
  }

  int total = 0;
  for (size_t index = 0; index < flexible->count; ++index) {
    total += flexible->values[index];
  }
  // @rewrites-end
  // @lowering-end

  free(flexible);
  return total == 6 ? 0 : 3;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = 8;
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = 3;
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// LOWERING-DAG: let {{__v[0-9]+}}: *mut FlexibleArray = {{__v[0-9]+}} as *mut FlexibleArray;
// LOWERING-DAG: flexible = {{__v[0-9]+}};
// LOWERING-DAG: {
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut FlexibleArray = std::ptr::null_mut();
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     if {{__v[0-9]+}} {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:         __retval = {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:         return std::process::ExitCode::from({{__v[0-9]+}} as u8);
// LOWERING-DAG:     }
// LOWERING-DAG: }
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = 3;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     (*{{__v[0-9]+}}).count = {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: {
// LOWERING-DAG:     let mut index: u64 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-DAG:     index = {{__v[0-9]+}};
// LOWERING-DAG:     loop {
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = index;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).count };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:         if !{{__v[0-9]+}} {
// LOWERING-DAG:             break;
// LOWERING-DAG:         }
// LOWERING-DAG:         {
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = index;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = index;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 std::ptr::write_unaligned(
// LOWERING-DAG:                     unsafe {
// LOWERING-DAG:                         std::ptr::addr_of_mut!(
// LOWERING-DAG:                             *std::ptr::addr_of_mut!((*{{__v[0-9]+}}).values)
// LOWERING-DAG:                                 .cast::<i32>()
// LOWERING-DAG:                                 .add({{__v[0-9]+}} as usize)
// LOWERING-DAG:                         )
// LOWERING-DAG:                     },
// LOWERING-DAG:                     {{__v[0-9]+}},
// LOWERING-DAG:                 )
// LOWERING-DAG:             };
// LOWERING-DAG:         }
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = index;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:         index = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG: }
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG: total = {{__v[0-9]+}};
// LOWERING-DAG: {
// LOWERING-DAG:     let mut index2: u64 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-DAG:     index2 = {{__v[0-9]+}};
// LOWERING-DAG:     loop {
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = index2;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).count };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:         if !{{__v[0-9]+}} {
// LOWERING-DAG:             break;
// LOWERING-DAG:         }
// LOWERING-DAG:         {
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = index2;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut FlexibleArray = flexible;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:                 std::ptr::read_unaligned(unsafe {
// LOWERING-DAG:                     std::ptr::addr_of!(
// LOWERING-DAG:                         *std::ptr::addr_of_mut!((*{{__v[0-9]+}}).values)
// LOWERING-DAG:                             .cast::<i32>()
// LOWERING-DAG:                             .add({{__v[0-9]+}} as usize)
// LOWERING-DAG:                     )
// LOWERING-DAG:                 })
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = total;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:             total = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = index2;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:         index2 = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering
