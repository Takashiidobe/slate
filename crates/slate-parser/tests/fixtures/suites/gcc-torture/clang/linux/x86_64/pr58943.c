/* PR c/58943 */

unsigned int x[1] = {2};

unsigned int foo(void) {
  x[0] |= 128;
  return 1;
}

int main() {
  x[0] |= foo();
  if (x[0] != 131)
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
// DEFAULT-NEXT:     global %0 x: array<u32, 1> [storage=static] = aggregate<array<u32, 1>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4: ptr<u32> [synthetic] = ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1)>(%0), const<i32>(0));
// DEFAULT-NEXT:         let %5: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%4)));
// DEFAULT-NEXT:         let %6: u32 [synthetic] = or<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(128)));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%4)), read<u32>(%6));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7: ptr<u32> [synthetic] = ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1)>(%0), const<i32>(0));
// DEFAULT-NEXT:         let %8: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%7)));
// DEFAULT-NEXT:         let %9: u32 [synthetic] = or<u32>(read<u32>(%8), call<u32, signature=fn() -> u32>(%1));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%7)), read<u32>(%9));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(1)>(%0), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(131)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
