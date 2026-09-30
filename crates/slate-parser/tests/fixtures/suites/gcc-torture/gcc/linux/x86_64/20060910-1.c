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
// DEFAULT-NEXT:     type @type[[TYPE_input_ty:[0-9]+]] input_ty = struct {
// DEFAULT-NEXT:         field0 buffer_position: ptr<u8>;
// DEFAULT-NEXT:         field1 buffer_end: ptr<u8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_input_ty]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: array<u8, 6> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_input_getc_complicated:[0-9]+]] @input_getc_complicated(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_input_ty]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_header:[0-9]+]] @check_header(%[[VALUE_deeper:[0-9]+]] deeper: ptr<@type[[TYPE_input_ty]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: u32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_len]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_len]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_len]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_len]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:                 if lt<ptr<u8>>(read<ptr<u8>>(field0(deref(read<ptr<@type[[TYPE_input_ty]]>>(%[[VALUE_deeper]])))), read<ptr<u8>>(field1(deref(read<ptr<@type[[TYPE_input_ty]]>>(%[[VALUE_deeper]])))))
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: ptr<@type[[TYPE_input_ty]]> [synthetic] = read<ptr<@type[[TYPE_input_ty]]>>(%[[VALUE_deeper]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(field0(deref(read<ptr<@type[[TYPE_input_ty]]>>(%[[VALUE4]]))));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(field0(deref(read<ptr<@type[[TYPE_input_ty]]>>(%[[VALUE4]]))), read<ptr<u8>>(%[[VALUE6]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE3]], reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%[[VALUE5]]))))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<i32>(%[[VALUE3]], call<i32, signature=fn(ptr<@type[[TYPE_input_ty]]>) -> i32>(%[[VALUE_input_getc_complicated]], read<ptr<@type[[TYPE_input_ty]]>>(%[[VALUE_deeper]])));
// DEFAULT-NEXT:                 if lt<i32>(read<i32>(%[[VALUE3]]), const<i32>(0))
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<u8>>(field0(%[[VALUE_s]]), array_decay<ptr<u8>, length=Some(6)>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<ptr<u8>>(field1(%[[VALUE_s]]), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(%[[VALUE_b]]), const<u64>(6)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_input_ty]]>) -> i32>(%[[VALUE_check_header]], addr_of<ptr<@type[[TYPE_input_ty]]>>(%[[VALUE_s]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<ptr<u8>>(read<ptr<u8>>(field0(%[[VALUE_s]])), read<ptr<u8>>(field1(%[[VALUE_s]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
