#include <stdio.h>

struct Slice {
  const char   *data;
  unsigned long length;
};

static struct Slice *make_slice(struct Slice *result, const char *data,
                                unsigned long length) {
  result->data   = data;
  result->length = length;
  return result;
}

#define SLICE(text) (*make_slice(&(struct Slice){}, text, sizeof(text) - 1))

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  struct Slice first  = SLICE("slate");
  struct Slice second = SLICE("translation");
  printf("%c %lu %c %lu\n", first.data[0], first.length, second.data[0],
         second.length);
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: _compoundliteral.data = {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-DAG: _compoundliteral.length = {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = 6;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut Slice = make_slice(std::ptr::addr_of_mut!(_compoundliteral), {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-DAG: unsafe { std::ptr::copy({{__v[0-9]+}}, std::ptr::addr_of_mut!(first), 1) };
// COMMON-LOWERING-DAG: _compoundliteral2.data = {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-DAG: _compoundliteral2.length = {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = 12;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut Slice = make_slice(std::ptr::addr_of_mut!(_compoundliteral2), {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-DAG: unsafe { std::ptr::copy({{__v[0-9]+}}, std::ptr::addr_of_mut!(second), 1) };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = first.length;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u64 = second.length;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:     printf(
// COMMON-LOWERING-DAG:         {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:     )
// COMMON-LOWERING-DAG: };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"translation\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%c %lu %c %lu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = first.data;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = second.data;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"slate\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"translation\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%c %lu %c %lu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = first.data;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = second.data;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: _compoundliteral.data = std::ptr::null_mut();
// COMMON-REWRITES-DAG: _compoundliteral.length = 0;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: u64 = 1;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: u64 = 6 - {{__v[0-9]+}};
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: *mut Slice = make_slice(
// COMMON-REWRITES-DAG:     unsafe { &mut (*std::ptr::addr_of_mut!(_compoundliteral)) },
// COMMON-REWRITES-DAG:     {{__v[0-9]+}},
// COMMON-REWRITES-DAG: );
// COMMON-REWRITES-DAG: unsafe { std::ptr::copy({{__v[0-9]+}}, std::ptr::addr_of_mut!(first), 1) };
// COMMON-REWRITES-DAG: _compoundliteral2.data = std::ptr::null_mut();
// COMMON-REWRITES-DAG: _compoundliteral2.length = 0;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: u64 = 1;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: u64 = 12 - {{__v[0-9]+}};
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: *mut Slice = make_slice(
// COMMON-REWRITES-DAG:     unsafe { &mut (*std::ptr::addr_of_mut!(_compoundliteral2)) },
// COMMON-REWRITES-DAG:     {{__v[0-9]+}},
// COMMON-REWRITES-DAG: );
// COMMON-REWRITES-DAG: unsafe { std::ptr::copy({{__v[0-9]+}}, std::ptr::addr_of_mut!(second), 1) };
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: u64 = first.length;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: u64 = second.length;
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-DAG:         {{__v[0-9]+}},
// COMMON-REWRITES-DAG:         {{__v[0-9]+}},
// COMMON-REWRITES-DAG:         (unsafe { *{{__v[0-9]+}} }) as i32,
// COMMON-REWRITES-DAG:         {{__v[0-9]+}},
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     c"slate".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:     c"translation".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = c"%c %lu %c %lu\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = first.data;
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = second.data;
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     c"slate".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:     c"translation".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = c"%c %lu %c %lu\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = first.data;
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = second.data;
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
