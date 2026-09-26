/* { dg-add-options stack_size } */

/* Origin: hp@bitrange.com
   Test that return values come out right from a 1000-level call chain to
   functions without parameters that each need at least one "long"
   preserved.  Exposed problems related to the MMIX port.  */

void abort(void);
void exit(int);

long        level = 0;
extern long foo(void);
extern long bar(void);

#ifdef STACK_SIZE
#define DEPTH ((STACK_SIZE) / 512 + 1)
#else
#define DEPTH 500
#endif

int main(void) {
  if (foo() == -42)
    exit(0);

  abort();
}

long foo(void) {
  long tmp = ++level;
  return bar() + tmp;
}

long bar(void) {
  long tmp = level;
  return tmp > DEPTH - 1 ? -42 - tmp : foo() - tmp;
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
// DEFAULT-NEXT:     global %2 level: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 tmp: i64 [storage=automatic];
// DEFAULT-NEXT:         let %9: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %10: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%9), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%10));
// DEFAULT-NEXT:         write<i64>(%6, read<i64>(%10));
// DEFAULT-NEXT:         return add<i64, overflow=ub>(call<i64, signature=fn() -> i64>(%4), read<i64>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 tmp: i64 [storage=automatic] = read<i64>(%2);
// DEFAULT-NEXT:         let %11: i64 [synthetic];
// DEFAULT-NEXT:         if gt<i64>(read<i64>(%7), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(const<i32>(500), const<i32>(1))))
// DEFAULT-NEXT:             write<i64>(%11, sub<i64, overflow=ub>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(42))), read<i64>(%7)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i64>(%11, sub<i64, overflow=ub>(call<i64, signature=fn() -> i64>(%3), read<i64>(%7)));
// DEFAULT-NEXT:         return read<i64>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<i64>(call<i64, signature=fn() -> i64>(%3), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(42))))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
