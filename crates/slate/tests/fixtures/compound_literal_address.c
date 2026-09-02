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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-DAG: _compoundliteral.data = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 0;
// LOWERING-DAG: _compoundliteral.length = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 6;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut Slice = make_slice(std::ptr::addr_of_mut!(_compoundliteral), {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG: unsafe { std::ptr::copy({{_v[0-9]+}}, std::ptr::addr_of_mut!(first), 1) };
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-DAG: _compoundliteral2.data = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 0;
// LOWERING-DAG: _compoundliteral2.length = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"translation\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 12;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut Slice = make_slice(std::ptr::addr_of_mut!(_compoundliteral2), {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG: unsafe { std::ptr::copy({{_v[0-9]+}}, std::ptr::addr_of_mut!(second), 1) };
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%c %lu %c %lu\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = first.data;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-DAG: let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = first.length;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = second.data;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-DAG: let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = second.length;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: _compoundliteral.data = std::ptr::null_mut();
// REWRITES-DAG: _compoundliteral.length = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 6 - {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: *mut Slice = make_slice(
// REWRITES-DAG:     unsafe { &mut (*std::ptr::addr_of_mut!(_compoundliteral)) },
// REWRITES-DAG:     c"slate".as_ptr() as *mut i8,
// REWRITES-DAG:     {{_v[0-9]+}},
// REWRITES-DAG: );
// REWRITES-DAG: unsafe { std::ptr::copy({{_v[0-9]+}}, std::ptr::addr_of_mut!(first), 1) };
// REWRITES-DAG: _compoundliteral2.data = std::ptr::null_mut();
// REWRITES-DAG: _compoundliteral2.length = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 12 - {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: *mut Slice = make_slice(
// REWRITES-DAG:     unsafe { &mut (*std::ptr::addr_of_mut!(_compoundliteral2)) },
// REWRITES-DAG:     c"translation".as_ptr() as *mut i8,
// REWRITES-DAG:     {{_v[0-9]+}},
// REWRITES-DAG: );
// REWRITES-DAG: unsafe { std::ptr::copy({{_v[0-9]+}}, std::ptr::addr_of_mut!(second), 1) };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = c"%c %lu %c %lu\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = first.data;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (unsafe { *{{_v[0-9]+}} }) as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = first.length;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = second.data;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = second.length;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:         {{_v[0-9]+}},
// REWRITES-DAG:         {{_v[0-9]+}},
// REWRITES-DAG:         (unsafe { *{{_v[0-9]+}} }) as i32,
// REWRITES-DAG:         {{_v[0-9]+}},
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
