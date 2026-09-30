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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_inttostr:[0-9]+]] @inttostr(%[[VALUE_i:[0-9]+]] i: i64, %[[VALUE_buf:[0-9]+]] buf: ptr<i8> [array=128]) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ui:[0-9]+]] ui: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(read<i64>(%[[VALUE_i]]));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_buf]]), const<i32>(127));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_p]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_i]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_ui]], neg<u64, overflow=wrap>(read<u64>(%[[VALUE_ui]])));
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE2]]));
// DEFAULT-NEXT:             write<i8>(deref(read<ptr<i8>>(%[[VALUE2]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(48))), rem<u64, by_zero=ub>(read<u64>(%[[VALUE_ui]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))))))));
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_ui]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: u64 [synthetic] = div<u64, by_zero=ub>(read<u64>(%[[VALUE3]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))));
// DEFAULT-NEXT:             write<u64>(%[[VALUE_ui]], read<u64>(%[[VALUE4]]));
// DEFAULT-NEXT:             yield ne<u64>(read<u64>(%[[VALUE4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_i]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE6]]));
// DEFAULT-NEXT:             write<i8>(deref(read<ptr<i8>>(%[[VALUE6]])), truncate<i8, reason=assign, fits=always>(const<i32>(45)));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_buf_2:[0-9]+]] buf: array<i8, 128> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_p_2]], call<ptr<i8>, signature=fn(i64, ptr<i8>) -> ptr<i8>>(%[[VALUE_inttostr]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), array_decay<ptr<i8>, length=Some(128)>(%[[VALUE_buf_2]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_p_2]])))), const<i32>(45))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
