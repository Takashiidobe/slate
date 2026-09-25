/* PR tree-optimization/86231 */

#define ONE ((void *)1)
#define TWO ((void *)2)

__attribute__((noipa)) int foo(void *p, int x) {
  if (p == ONE)
    return 0;
  if (!p)
    p = x ? TWO : ONE;
  return p == ONE ? 0 : 1;
}

int v[8];

int main() {
  if (foo((void *)0, 0) != 0 || foo((void *)0, 1) != 1 || foo(ONE, 0) != 0 ||
      foo(ONE, 1) != 0 || foo(TWO, 0) != 1 || foo(TWO, 1) != 1 ||
      foo(&v[7], 0) != 1 || foo(&v[7], 1) != 1)
    __builtin_abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
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
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %3 v: array<i32, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo(%1 p: ptr<void>, %2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(%1), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(read<ptr<void>>(%1), null<ptr<void>>))
// DEFAULT-NEXT:             write<ptr<void>>(%1, conditional<ptr<void>>(ne<i32>(read<i32>(%2), const<i32>(0)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(2)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:         return conditional<i32>(eq<ptr<void>>(read<ptr<void>>(%1), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1))), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%0, null<ptr<void>>, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%5, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%5, ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%0, null<ptr<void>>, const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         let %6: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%5)
// DEFAULT-NEXT:             write<bool>(%6, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%6, ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%0, int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1)), const<i32>(0)), const<i32>(0)));
// DEFAULT-NEXT:         let %7: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%6)
// DEFAULT-NEXT:             write<bool>(%7, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%7, ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%0, int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1)), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %8: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%7)
// DEFAULT-NEXT:             write<bool>(%8, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%8, ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%0, int_to_ptr<ptr<void>, reason=explicit>(const<i32>(2)), const<i32>(0)), const<i32>(1)));
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%0, int_to_ptr<ptr<void>, reason=explicit>(const<i32>(2)), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%0, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%3), const<i32>(7))))), const<i32>(0)), const<i32>(1)));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%0, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%3), const<i32>(7))))), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
