void abort(void);
void exit(int);

unsigned int f1(int diff) { return ((unsigned int)(diff < 0 ? -diff : diff)); }

unsigned int f2(unsigned int diff) {
  return ((unsigned int)((signed int)diff < 0 ? -diff : diff));
}

unsigned long long f3(long long diff) {
  return ((unsigned long long)(diff < 0 ? -diff : diff));
}

unsigned long long f4(unsigned long long diff) {
  return ((unsigned long long)((signed long long)diff < 0 ? -diff : diff));
}

int main(void) {
  int i;
  for (i = 0; i <= 10; i++) {
    if (f1(i) != i)
      abort();
    if (f1(-i) != i)
      abort();
    if (f2(i) != i)
      abort();
    if (f2(-i) != i)
      abort();
    if (f3((long long)i) != i)
      abort();
    if (f3((long long)-i) != i)
      abort();
    if (f4((long long)i) != i)
      abort();
    if (f4((long long)-i) != i)
      abort();
  }
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f1(%3 diff: i32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=explicit, fits=unknown>(conditional<i32>(lt<i32>(read<i32>(%3), const<i32>(0)), neg<i32, overflow=ub>(read<i32>(%3)), read<i32>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2(%5 diff: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(lt<i32>(reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%5)), const<i32>(0)), neg<u32, overflow=wrap>(read<u32>(%5)), read<u32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f3(%7 diff: i64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=explicit, fits=unknown>(conditional<i64>(lt<i64>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(0))), neg<i64, overflow=ub>(read<i64>(%7)), read<i64>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f4(%9 diff: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u64>(lt<i64>(reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%9)), widen<i64, reason=usual_arith>(const<i32>(0))), neg<u64, overflow=wrap>(read<u64>(%9)), read<u64>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%11), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<u32>(call<u32, signature=fn(i32) -> u32>(%2, read<i32>(%11)), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%11)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     if ne<u32>(call<u32, signature=fn(i32) -> u32>(%2, neg<i32, overflow=ub>(read<i32>(%11))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%11)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     if ne<u32>(call<u32, signature=fn(u32) -> u32>(%4, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%11))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%11)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     if ne<u32>(call<u32, signature=fn(u32) -> u32>(%4, reinterpret<u32, reason=arg, fits=unknown>(neg<i32, overflow=ub>(read<i32>(%11)))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%11)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     if ne<u64>(call<u64, signature=fn(i64) -> u64>(%6, widen<i64, reason=explicit>(read<i32>(%11))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%11))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     if ne<u64>(call<u64, signature=fn(i64) -> u64>(%6, widen<i64, reason=explicit>(neg<i32, overflow=ub>(read<i32>(%11)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%11))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     if ne<u64>(call<u64, signature=fn(u64) -> u64>(%8, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%11)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%11))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     if ne<u64>(call<u64, signature=fn(u64) -> u64>(%8, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(read<i32>(%11))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%11))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
