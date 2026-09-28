/* PR c/19606
   The C front end used to shorten the type of a division to a type
   that does not preserve the semantics of the original computation.
   Make sure that won't happen.  */

void abort(void);
void exit(int);

signed char a = -4;

int foo(void) { return ((unsigned int)(signed int)a) / 2LL; }

int bar(void) { return ((unsigned int)(signed int)a) % 5LL; }

int main(void) {
  int r;

  r = foo();
  if (r != ((unsigned int)(signed int)(signed char)-4) / 2LL)
    abort();

  r = bar();
  if (r != ((unsigned int)(signed int)(signed char)-4) % 5LL)
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
// DEFAULT-NEXT:     global %2 a: i8 [storage=static] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(4))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(div<i64, by_zero=ub, min_by_neg_one=ub>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(%2))))), const<i64>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(rem<i64, by_zero=ub, min_by_neg_one=ub>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(%2))))), const<i64>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%6, call<i32, signature=fn() -> i32>(%3));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%3);
// DEFAULT-NEXT:         if ne<i64>(widen<i64, reason=usual_arith>(read<i32>(%6)), div<i64, by_zero=ub, min_by_neg_one=ub>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(4))))))), const<i64>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%6, call<i32, signature=fn() -> i32>(%4));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%4);
// DEFAULT-NEXT:         if ne<i64>(widen<i64, reason=usual_arith>(read<i32>(%6)), rem<i64, by_zero=ub, min_by_neg_one=ub>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(4))))))), const<i64>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
