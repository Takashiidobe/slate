typedef unsigned int U32;

static int cpuid_leaf1_nonzero(void) {
  U32 f1a, f1c, f1d;
  // @lowering-begin
  // @rewrite-begin
  __asm__("cpuid\n\t" : "=a"(f1a), "=c"(f1c), "=d"(f1d) : "a"(1) : "ebx");
  // @rewrite-end
  // @lowering-end
  return (int)((f1c | f1d) != 0);
}

int main(void) { return cpuid_leaf1_nonzero(); }

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("cpuid\n\t", lateout("eax") {{__v[0-9]+}}, lateout("ecx") {{__v[0-9]+}}, lateout("edx") {{__v[0-9]+}}, in("eax") {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("cpuid\n\t", lateout("eax") {{__v[0-9]+}}, lateout("ecx") {{__v[0-9]+}}, lateout("edx") {{__v[0-9]+}}, in("eax") 1, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
