// Make sure -finline-functions family flags are behaving correctly.
//
//

inline int inline_hint(int a, int b) { return(a+b); }

int inline_no_hint(int a, int b) { return (a/b); }

inline __attribute__ ((__always_inline__)) int inline_always(int a, int b) { return(a*b); }

volatile int *pa = (int*) 0x1000;
void foo(void) {
    pa[0] = inline_hint(pa[1],pa[2]);
    pa[3] = inline_always(pa[4],pa[5]);
    pa[6] = inline_no_hint(pa[7], pa[8]);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %9 pa: ptr<volatile i32> [storage=static] = pointer_cast<ptr<volatile i32>, reason=assign>(int_to_ptr<ptr<i32>, reason=explicit>(const<i32>(4096))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @inline_hint(%1 a: i32, %2 b: i32) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%1), read<i32>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @inline_no_hint(%4 a: i32, %5 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), read<i32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @inline_always(%7 a: i32, %8 b: i32) -> i32 [linkage=external] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%7), read<i32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(0))), call<i32, signature=fn(i32, i32) -> i32>(%0, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(1)))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(2))))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32) -> i32>(%0, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(1)))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(2)))));
// DEFAULT-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(3))), call<i32, signature=fn(i32, i32) -> i32>(%6, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(4)))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(5))))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32) -> i32>(%6, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(4)))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(5)))));
// DEFAULT-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(6))), call<i32, signature=fn(i32, i32) -> i32>(%3, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(7)))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(8))))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32) -> i32>(%3, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(7)))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%9), const<i32>(8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
