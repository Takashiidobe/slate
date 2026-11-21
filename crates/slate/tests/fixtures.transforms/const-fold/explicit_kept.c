int narrowed(void) { return (int)7L; }

unsigned char wrapped(void) { return (unsigned char)300; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @narrowed() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return truncate<i32, reason=explicit, fits=always>(const<i64>(7));
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @wrapped() -> u8 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(300)));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
