typedef unsigned int U32;

static int cpuid_leaf1_nonzero(void) {
  U32 f1a, f1c, f1d;
  // @lowering-begin
  // @rewrite-begin
  __asm__(
      "cpuid\n\t"
      : "=a"(f1a), "=c"(f1c), "=d"(f1d)
      : "a"(1)
      : "ebx");
  // @rewrite-end
  // @lowering-end
  return (int)((f1c | f1d) != 0);
}

int main(void) {
  return cpuid_leaf1_nonzero();
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: u32;
// LOWERING-DAG: let {{_v[0-9]+}}: u32;
// LOWERING-DAG: let {{_v[0-9]+}}: u32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: core::arch::asm!("cpuid\n\t", lateout("eax") {{_v[0-9]+}}, lateout("ecx") {{_v[0-9]+}}, lateout("edx") {{_v[0-9]+}}, in("eax") {{_v[0-9]+}}, options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: f1a = {{_v[0-9]+}};
// LOWERING-DAG: f1c = {{_v[0-9]+}};
// LOWERING-DAG: f1d = {{_v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: u32;
// REWRITES-DAG: let {{_v[0-9]+}}: u32;
// REWRITES-DAG: let {{_v[0-9]+}}: u32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG: core::arch::asm!("cpuid\n\t", lateout("eax") {{_v[0-9]+}}, lateout("ecx") {{_v[0-9]+}}, lateout("edx") {{_v[0-9]+}}, in("eax") {{_v[0-9]+}}, options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: f1a = {{_v[0-9]+}};
// REWRITES-DAG: f1c = {{_v[0-9]+}};
// REWRITES-DAG: f1d = {{_v[0-9]+}};
// SLATE-FILECHECK-END rewrites
