/* PR target/14209.  Bug in cris.md, shrinking access size of
   postincrement.
   Origin: <hp@axis.com>.  */

void abort(void);
void exit(int);

long int  xb(long int *y) __attribute__((__noinline__));
long int  xw(long int *y) __attribute__((__noinline__));
short int yb(short int *y) __attribute__((__noinline__));

long int xb(long int *y) {
  long int xx = *y & 255;
  return xx + y[1];
}

long int xw(long int *y) {
  long int xx = *y & 65535;
  return xx + y[1];
}

short int yb(short int *y) {
  short int xx = *y & 255;
  return xx + y[1];
}

int main(void) {
  long int  y[]  = {-1, 16000};
  short int yw[] = {-1, 16000};

  if (xb(y) != 16255 || xw(y) != 81535 || yb(yw) != 16255)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%14 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @xb(%5 y: ptr<i64>) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 xx: i64 [storage=automatic] = and<i64>(read<i64>(deref(read<ptr<i64>>(%5))), widen<i64, reason=usual_arith>(const<i32>(255)));
// DEFAULT-NEXT:         return add<i64, overflow=ub>(read<i64>(%6), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%5), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @xw(%7 y: ptr<i64>) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 xx: i64 [storage=automatic] = and<i64>(read<i64>(deref(read<ptr<i64>>(%7))), widen<i64, reason=usual_arith>(const<i32>(65535)));
// DEFAULT-NEXT:         return add<i64, overflow=ub>(read<i64>(%8), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%7), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @yb(%9 y: ptr<i16>) -> i16 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 xx: i16 [storage=automatic] = truncate<i16, reason=assign, fits=unknown>(and<i32>(widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%9)))), const<i32>(255)));
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%10)), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%9), const<i32>(1)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 y: array<i64, 2> [storage=automatic] [align=16] = aggregate<array<i64, 2>, zero_fill=false>(index0 = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))), index1 = widen<i64, reason=assign>(const<i32>(16000)));
// DEFAULT-NEXT:         let %13 yw: array<i16, 2> [storage=automatic] = aggregate<array<i16, 2>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(16000)));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(ptr<i64>) -> i64>(%2, array_decay<ptr<i64>, length=Some(2)>(%12)), widen<i64, reason=usual_arith>(const<i32>(16255)))
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<i64>(call<i64, signature=fn(ptr<i64>) -> i64>(%3, array_decay<ptr<i64>, length=Some(2)>(%12)), widen<i64, reason=usual_arith>(const<i32>(81535))));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(ptr<i16>) -> i16>(%4, array_decay<ptr<i16>, length=Some(2)>(%13))), const<i32>(16255)));
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
