/* PR rtl-optimization/57875 */

extern void abort(void);
int         a[1], b, c, d, f, i;
char        e[1];

int main() {
  for (; i < 1; i++)
    if (!d) {
      if (!c)
        f = 2;
      e[0] &= f ^= 0;
    }
  b = a[e[0] >> 1 & 1];
  if (b != 0)
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
// DEFAULT-NEXT:     global %1 a: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 e: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if not<bool>(ne<i32>(read<i32>(%3), const<i32>(0)))
// DEFAULT-NEXT:                             write<i32>(%5, const<i32>(2));
// DEFAULT-NEXT:                         let %12: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%7), const<i32>(0));
// DEFAULT-NEXT:                         let %13: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%12)));
// DEFAULT-NEXT:                         let %14: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                         let %15: i32 [synthetic] = xor<i32>(read<i32>(%14), const<i32>(0));
// DEFAULT-NEXT:                         write<i32>(%5, read<i32>(%15));
// DEFAULT-NEXT:                         let %16: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(and<i32>(widen<i32, reason=promotion>(read<i8>(%13)), read<i32>(%15)));
// DEFAULT-NEXT:                         write<i8>(deref(read<ptr<i8>>(%12)), read<i8>(%16));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%7), const<i32>(0))))), const<i32>(1)), const<i32>(1))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
