typedef unsigned int U32;

int cpuid_leaf7_ebx_nonzero(void) {
  U32 f7a, f7b, f7c;
  // @lowering-begin
  // @rewrite-begin
  __asm__("cpuid" : "=a"(f7a), "=b"(f7b), "=c"(f7c) : "a"(7), "c"(0) : "edx");
  // @rewrite-end
  // @lowering-end
  return (int)(f7b != 0 || f7a != 0 || f7c != 0);
}

int main(void) { return cpuid_leaf7_ebx_nonzero(); }

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32 = 7;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("push %rbx\n\tcpuid\n\tmov %ebx, %edi\n\tpop %rbx", lateout("eax") {{__v[0-9]+}}, lateout("edi") {{__v[0-9]+}}, lateout("ecx") {{__v[0-9]+}}, in("eax") {{__v[0-9]+}}, in("ecx") {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: f7a = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: f7c = {{__v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("push %rbx\n\tcpuid\n\tmov %ebx, %edi\n\tpop %rbx", lateout("eax") {{__v[0-9]+}}, lateout("edi") {{__v[0-9]+}}, lateout("ecx") {{__v[0-9]+}}, in("eax") 7, in("ecx") 0, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: f7a = {{__v[0-9]+}};
// REWRITES-X86_64-GNU-DAG: f7c = {{__v[0-9]+}};
// SLATE-FILECHECK-END rewrites
