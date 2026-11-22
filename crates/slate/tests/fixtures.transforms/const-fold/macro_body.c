#define SET(x) \
  do {          \
    (x) = 300;  \
  } while (0)

unsigned char set(void) {
  unsigned char c;
  SET(c);
  return c;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @set() -> u8 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_C:[0-9]+]] c: u8 [storage=automatic];
// CHECK-NEXT:     macro_body #0 {
// CHECK-NEXT:         do %[[V0:[0-9]+]]
// CHECK-NEXT:             {
// CHECK-NEXT:                 write<u8>(%[[V_C]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(300))));
// CHECK-NEXT:             }
// CHECK-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// CHECK-NEXT:     }
// CHECK-NEXT:     return read<u8>(%[[V_C]]);
// CHECK: {{^}}}
// SLATE-FILECHECK-END
