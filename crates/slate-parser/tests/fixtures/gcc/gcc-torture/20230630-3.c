struct S {
  __INT32_TYPE__ i  : 24;
  char           c1 : 1;
  char           c2 : 1;
  char           c3 : 1;
  char           c4 : 1;
  char           c5 : 1;
  char           c6 : 1;
  char           c7 : 1;
  char           c8 : 1;
};

int main(void) {
  struct S s0 = {1193046, 1, 1, 1, 1, 1, 1, 1, 1};
  char    *p  = (char *)&s0;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  if (*p != 86)
    __builtin_abort();
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  if (*p != 18)
    __builtin_abort();
#endif

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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 i: i32 : 24;
// DEFAULT-NEXT:         field1 c1: i8 : 1;
// DEFAULT-NEXT:         field2 c2: i8 : 1;
// DEFAULT-NEXT:         field3 c3: i8 : 1;
// DEFAULT-NEXT:         field4 c4: i8 : 1;
// DEFAULT-NEXT:         field5 c5: i8 : 1;
// DEFAULT-NEXT:         field6 c6: i8 : 1;
// DEFAULT-NEXT:         field7 c7: i8 : 1;
// DEFAULT-NEXT:         field8 c8: i8 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 3, 3, 3, 3, 3, 3, 3, 3], bit_offsets=[Some(0), Some(24), Some(25), Some(26), Some(27), Some(28), Some(29), Some(30), Some(31)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %4 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 s0: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1193046), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field2 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field3 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field4 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field5 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field6 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field7 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field8 = truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %3 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%2));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%3)))), const<i32>(86))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
