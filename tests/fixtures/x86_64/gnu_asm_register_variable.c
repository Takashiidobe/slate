// { dg-options "-std=gnu23" }
int main(void) {
  register int x asm("eax") = 5;
  __asm__ __volatile__("incl %0" : "+r"(x));
  return x;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU: #![allow(
// LOWERING-X86_64-GNU-NEXT:     dead_code,
// LOWERING-X86_64-GNU-NEXT:     unused,
// LOWERING-X86_64-GNU-NEXT:     non_camel_case_types,
// LOWERING-X86_64-GNU-NEXT:     non_snake_case,
// LOWERING-X86_64-GNU-NEXT:     non_upper_case_globals,
// LOWERING-X86_64-GNU-NEXT:     arithmetic_overflow,
// LOWERING-X86_64-GNU-NEXT:     unconditional_panic,
// LOWERING-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-X86_64-GNU-NEXT:     unused_comparisons
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: /// { dg-options "-std=gnu23" }
// LOWERING-X86_64-GNU-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("incl %eax", lateout("eax") {{__v[0-9]+}}, in("eax") {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU: #![allow(
// REWRITES-X86_64-GNU-NEXT:     dead_code,
// REWRITES-X86_64-GNU-NEXT:     unused,
// REWRITES-X86_64-GNU-NEXT:     non_camel_case_types,
// REWRITES-X86_64-GNU-NEXT:     non_snake_case,
// REWRITES-X86_64-GNU-NEXT:     non_upper_case_globals,
// REWRITES-X86_64-GNU-NEXT:     arithmetic_overflow,
// REWRITES-X86_64-GNU-NEXT:     unconditional_panic,
// REWRITES-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-X86_64-GNU-NEXT:     unused_comparisons
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: /// { dg-options "-std=gnu23" }
// REWRITES-X86_64-GNU-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("incl %eax", lateout("eax") {{__v[0-9]+}}, in("eax") 5, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites
