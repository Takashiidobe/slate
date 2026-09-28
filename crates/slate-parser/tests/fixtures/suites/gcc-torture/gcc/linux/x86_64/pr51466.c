/* PR tree-optimization/51466 */

extern void abort(void);

__attribute__((noinline, noclone)) int foo(int i) {
  volatile int v[4];
  int         *p;
  v[i] = 6;
  p    = (int *)&v[i];
  return *p;
}

__attribute__((noinline, noclone)) int bar(int i) {
  volatile int v[4];
  int         *p;
  v[i] = 6;
  p    = (int *)&v[i];
  *p   = 8;
  return v[i];
}

__attribute__((noinline, noclone)) int baz(int i) {
  volatile int v[4];
  int         *p;
  v[i] = 6;
  p    = (int *)&v[0];
  *p   = 8;
  return v[i];
}

int main() {
  if (foo(3) != 6 || bar(2) != 8 || baz(0) != 8 || baz(1) != 6)
    abort();
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @foo(%2 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 v: volatile array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %4 p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%3), read<i32>(%2))), const<i32>(6));
// DEFAULT-NEXT:         write<ptr<i32>>(%4, pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<volatile i32>>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%3), read<i32>(%2))))));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 v: volatile array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %8 p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%7), read<i32>(%6))), const<i32>(6));
// DEFAULT-NEXT:         write<ptr<i32>>(%8, pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<volatile i32>>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%7), read<i32>(%6))))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%8)), const<i32>(8));
// DEFAULT-NEXT:         return read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%7), read<i32>(%6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @baz(%10 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 v: volatile array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %12 p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%11), read<i32>(%10))), const<i32>(6));
// DEFAULT-NEXT:         write<ptr<i32>>(%12, pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<volatile i32>>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%11), const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%12)), const<i32>(8));
// DEFAULT-NEXT:         return read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%11), read<i32>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(3)), const<i32>(6))
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, ne<i32>(call<i32, signature=fn(i32) -> i32>(%5, const<i32>(2)), const<i32>(8)));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, const<i32>(0)), const<i32>(8)));
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, ne<i32>(call<i32, signature=fn(i32) -> i32>(%9, const<i32>(1)), const<i32>(6)));
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
