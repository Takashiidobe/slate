typedef unsigned int U32;

int cpuid_leaf7_ebx_nonzero(void) {
  U32 f7a, f7b, f7c;
  // @lowering-begin
  // @rewrite-begin
  __asm__("cpuid"
          : "=a"(f7a), "=b"(f7b), "=c"(f7c)
          : "a"(7), "c"(0)
          : "edx");
  // @rewrite-end
  // @lowering-end
  return (int)(f7b != 0 || f7a != 0 || f7c != 0);
}

int main(void) {
  return cpuid_leaf7_ebx_nonzero();
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 7;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: u32;
// LOWERING-DAG: let {{_v[0-9]+}}: u32;
// LOWERING-DAG: let {{_v[0-9]+}}: u32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: core::arch::asm!("push %rbx\n\tcpuid\n\tmov %ebx, %edi\n\tpop %rbx", lateout("eax") {{_v[0-9]+}}, lateout("edi") {{_v[0-9]+}}, lateout("ecx") {{_v[0-9]+}}, in("eax") {{_v[0-9]+}}, in("ecx") {{_v[0-9]+}}, options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: f7a = {{_v[0-9]+}};
// LOWERING-DAG: f7b = {{_v[0-9]+}};
// LOWERING-DAG: f7c = {{_v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 7;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: u32;
// REWRITES-DAG: let {{_v[0-9]+}}: u32;
// REWRITES-DAG: let {{_v[0-9]+}}: u32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG: core::arch::asm!("push %rbx\n\tcpuid\n\tmov %ebx, %edi\n\tpop %rbx", lateout("eax") {{_v[0-9]+}}, lateout("edi") {{_v[0-9]+}}, lateout("ecx") {{_v[0-9]+}}, in("eax") {{_v[0-9]+}}, in("ecx") {{_v[0-9]+}}, options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: f7a = {{_v[0-9]+}};
// REWRITES-DAG: f7b = {{_v[0-9]+}};
// REWRITES-DAG: f7c = {{_v[0-9]+}};
// SLATE-FILECHECK-END rewrites
