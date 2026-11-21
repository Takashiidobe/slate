short take(short s);

signed char narrow(void) {
  signed char c = 200;
  return c;
}

unsigned all_ones(void) {
  unsigned u = -1;
  return u;
}

long long widen(void) {
  long long l = -5;
  return l;
}

char letter(void) { return 'a'; }

int argument(void) { return take(70000); }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @narrow() -> i8 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_C:[0-9]+]] c: i8 [storage=automatic] = const<i8>(-56);
// CHECK-NEXT:     return read<i8>(%[[V_C]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @all_ones() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_U:[0-9]+]] u: u32 [storage=automatic] = const<u32>(4294967295);
// CHECK-NEXT:     return read<u32>(%[[V_U]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @widen() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     let %[[V_L:[0-9]+]] l: i64 [storage=automatic] = const<i64>(-5);
// CHECK-NEXT:     return read<i64>(%[[V_L]]);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @letter() -> i8 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return const<i8>(97);
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn %{{[0-9]+}} @argument() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CHECK-NEXT:     return widen<i32, reason=return>(call<i16, signature=fn(i16) -> i16>(%[[V0:[0-9]+]], const<i16>(4464)));
// CHECK: {{^}}}
// SLATE-FILECHECK-END
