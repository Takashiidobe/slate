extern void abort(void);
static void fixnum_neg(signed char x, signed char *py, int *pv) {
  unsigned char ux, uy;

  ux  = (unsigned char)x;
  uy  = -ux;
  *py = (uy <= 127) ? (signed char)uy : (-(signed char)(255 - uy) - 1);
  *pv = (x == -128) ? 1 : 0;
}

void __attribute__((noinline)) foo(int x, int y, int v) {
  if (y < -128 || y > 127)
    abort();
}

int test_neg(void) {
  signed char x, y;
  int         v, err;

  err = 0;
  x   = -128;
  for (;;) {
    fixnum_neg(x, &y, &v);
    foo((int)x, (int)y, v);
    if ((v && x != -128) || (!v && x == -128))
      ++err;
    if (x == 127)
      break;
    ++x;
  }
  return err;
}

int main(void) {
  if (sizeof(char) != 1)
    return 0;
  if (test_neg() != 0)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @fixnum_neg(%2 x: i8, %3 py: ptr<i8>, %4 pv: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 ux: u8 [storage=automatic];
// DEFAULT-NEXT:         let %6 uy: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%5, reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(%2)));
// DEFAULT-NEXT:         write<u8>(%6, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%5)))))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%3)), truncate<i8, reason=assign, fits=unknown>(conditional<i32>(le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))), const<i32>(127)), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(read<u8>(%6))), sub<i32, overflow=ub>(neg<i32, overflow=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(sub<i32, overflow=ub>(const<i32>(255), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%6))))))), const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%4)), conditional<i32>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%2)), neg<i32, overflow=ub>(const<i32>(128))), const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @foo(%8 x: i32, %9 y: i32, %10 v: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%9), neg<i32, overflow=ub>(const<i32>(128))), gt<i32>(read<i32>(%9), const<i32>(127)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_neg() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 x: i8 [storage=automatic];
// DEFAULT-NEXT:         let %13 y: i8 [storage=automatic];
// DEFAULT-NEXT:         let %14 v: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 err: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%15, const<i32>(0));
// DEFAULT-NEXT:         write<i8>(%12, truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(128))));
// DEFAULT-NEXT:         for %17
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(i8, ptr<i8>, ptr<i32>) -> void>(%1, read<i8>(%12), addr_of<ptr<i8>>(%13), addr_of<ptr<i32>>(%14));
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%7, widen<i32, reason=explicit>(read<i8>(%12)), widen<i32, reason=explicit>(read<i8>(%13)), read<i32>(%14));
// DEFAULT-NEXT:                     if logical_or<bool>(logical_and<bool>(ne<i32>(read<i32>(%14), const<i32>(0)), ne<i32>(widen<i32, reason=promotion>(read<i8>(%12)), neg<i32, overflow=ub>(const<i32>(128)))), logical_and<bool>(not<bool>(ne<i32>(read<i32>(%14), const<i32>(0))), eq<i32>(widen<i32, reason=promotion>(read<i8>(%12)), neg<i32, overflow=ub>(const<i32>(128)))))
// DEFAULT-NEXT:                         let %18: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                         let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%15, read<i32>(%19));
// DEFAULT-NEXT:                     if eq<i32>(widen<i32, reason=promotion>(read<i8>(%12)), const<i32>(127))
// DEFAULT-NEXT:                         break %17;
// DEFAULT-NEXT:                     let %20: i8 [synthetic] = read<i8>(%12);
// DEFAULT-NEXT:                     let %21: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%20)), const<i32>(1)));
// DEFAULT-NEXT:                     write<i8>(%12, read<i8>(%21));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%11), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
