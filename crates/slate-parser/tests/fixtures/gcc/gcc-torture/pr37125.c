extern void abort(void);

static inline unsigned int mod_rhs(int rhs) {
  if (rhs == 0)
    return 1;
  return rhs;
}

void func_44(unsigned int p_45);
void func_44(unsigned int p_45) {
  if (!((p_45 * -9) % mod_rhs(-9))) {
    abort();
  }
}

int main(void) {
  func_44(2);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @mod_rhs(%2 rhs: i32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             return reinterpret<u32, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(read<i32>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @func_44(%4 p_45: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<u32>(rem<u32, by_zero=ub>(mul<u32, overflow=wrap>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=unknown>(neg<i32, overflow=ub>(const<i32>(9)))), call<u32, signature=fn(i32) -> u32>(%1, neg<i32, overflow=ub>(const<i32>(9)))), const<u32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%3, reinterpret<u32, reason=arg, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
