/* PR 6534 */
/* GCSE unified the two i<0 tests, but if-conversion to ui=abs(i)
   insertted the code at the wrong place corrupting the i<0 test.  */

void         abort(void);
static char *inttostr(long i, char buf[128]) {
  unsigned long ui = i;
  char         *p  = buf + 127;
  *p               = '\0';
  if (i < 0)
    ui = -ui;
  do
    *--p = '0' + ui % 10;
  while ((ui /= 10) != 0);
  if (i < 0)
    *--p = '-';
  return p;
}

int main() {
  char buf[128], *p;

  p = inttostr(-1, buf);
  if (*p != '-')
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
// DEFAULT-NEXT:     fn %1 @inttostr(%2 i: i64, %3 buf: ptr<i8> [array=128]) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 ui: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(read<i64>(%2));
// DEFAULT-NEXT:         let %5 p: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), const<i32>(127));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%5)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<u64>(%4, neg<u64, overflow=wrap>(read<u64>(%4)));
// DEFAULT-NEXT:         do %9
// DEFAULT-NEXT:             let %10: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:             let %11: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%10), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%5, read<ptr<i8>>(%11));
// DEFAULT-NEXT:             write<i8>(deref(read<ptr<i8>>(%11)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(48))), rem<u64, by_zero=ub>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))))))));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %12: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:             let %13: u64 [synthetic] = div<u64, by_zero=ub>(read<u64>(%12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))));
// DEFAULT-NEXT:             write<u64>(%4, read<u64>(%13));
// DEFAULT-NEXT:             yield ne<u64>(read<u64>(%13), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             let %14: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:             let %15: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%14), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%5, read<ptr<i8>>(%15));
// DEFAULT-NEXT:             write<i8>(deref(read<ptr<i8>>(%15)), truncate<i8, reason=assign, fits=always>(const<i32>(45)));
// DEFAULT-NEXT:         return read<ptr<i8>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 buf: array<i8, 128> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %8 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%8, call<ptr<i8>, signature=fn(i64, ptr<i8>) -> ptr<i8>>(%1, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), array_decay<ptr<i8>, length=Some(128)>(%7)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(i64, ptr<i8>) -> ptr<i8>>(%1, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), array_decay<ptr<i8>, length=Some(128)>(%7));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%8)))), const<i32>(45))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
