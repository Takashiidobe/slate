extern void abort(void);
extern void exit(int);

int   a[6], c = 1, d;
short e;

int __attribute__((noinline)) fn1(int p) { return a[p]; }

int main() {
  if (sizeof(long long) != 8)
    exit(0);

  a[0] = 1;
  if (c)
    e--;
  d           = e;
  long long f = e;
  if (fn1((f >> 56) & 1) != 0)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     global %2 a: array<i32, 6> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 e: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @fn1(%7 p: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(%2), read<i32>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(%2), const<i32>(0))), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             let %11: i16 [synthetic] = read<i16>(%5);
// DEFAULT-NEXT:             let %12: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%11)), const<i32>(1)));
// DEFAULT-NEXT:             write<i16>(%5, read<i16>(%12));
// DEFAULT-NEXT:         write<i32>(%4, widen<i32, reason=assign>(read<i16>(%5)));
// DEFAULT-NEXT:         let %9 f: i64 [storage=automatic] = widen<i64, reason=assign>(read<i16>(%5));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%6, truncate<i32, reason=arg, fits=unknown>(and<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%9), const<i32>(56)), widen<i64, reason=usual_arith>(const<i32>(1))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
