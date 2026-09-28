/* PR rtl-optimization/28636 */
/* Origin: Andreas Schwab <schwab@suse.de> */

extern void abort(void);

struct input_ty {
  unsigned char *buffer_position;
  unsigned char *buffer_end;
};

int input_getc_complicated(struct input_ty *x) { return 0; }

int check_header(struct input_ty *deeper) {
  unsigned len;
  for (len = 0; len < 6; len++)
    if (((deeper)->buffer_position < (deeper)->buffer_end
             ? *((deeper)->buffer_position)++
             : input_getc_complicated((deeper))) < 0)
      return 0;
  return 1;
}

struct input_ty s;
unsigned char   b[6];

int main(void) {
  s.buffer_position = b;
  s.buffer_end      = b + sizeof b;
  if (!check_header(&s))
    abort();
  if (s.buffer_position != s.buffer_end)
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
// DEFAULT-NEXT:     type @type0 input_ty = struct {
// DEFAULT-NEXT:         field0 buffer_position: ptr<u8>;
// DEFAULT-NEXT:         field1 buffer_end: ptr<u8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %7 s: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 b: array<u8, 6> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @input_getc_complicated(%3 x: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @check_header(%5 deeper: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 len: u32 [storage=automatic];
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%6, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %11: u32 [synthetic] = read<u32>(%6);
// DEFAULT-NEXT:                 let %12: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%6, read<u32>(%12));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %13: i32 [synthetic];
// DEFAULT-NEXT:                 if lt<ptr<u8>>(read<ptr<u8>>(field0(deref(read<ptr<@type0>>(%5)))), read<ptr<u8>>(field1(deref(read<ptr<@type0>>(%5)))))
// DEFAULT-NEXT:                     let %14: ptr<@type0> [synthetic] = read<ptr<@type0>>(%5);
// DEFAULT-NEXT:                     let %15: ptr<u8> [synthetic] = read<ptr<u8>>(field0(deref(read<ptr<@type0>>(%14))));
// DEFAULT-NEXT:                     let %16: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%15), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(field0(deref(read<ptr<@type0>>(%14))), read<ptr<u8>>(%16));
// DEFAULT-NEXT:                     write<i32>(%13, reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%15))))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<i32>(%13, call<i32, signature=fn(ptr<@type0>) -> i32>(%2, read<ptr<@type0>>(%5)));
// DEFAULT-NEXT:                 if lt<i32>(read<i32>(%13), const<i32>(0))
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<u8>>(field0(%7), array_decay<ptr<u8>, length=Some(6)>(%8));
// DEFAULT-NEXT:         write<ptr<u8>>(field1(%7), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(%8), const<u64>(6)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%4, addr_of<ptr<@type0>>(%7)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<ptr<u8>>(read<ptr<u8>>(field0(%7)), read<ptr<u8>>(field1(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
