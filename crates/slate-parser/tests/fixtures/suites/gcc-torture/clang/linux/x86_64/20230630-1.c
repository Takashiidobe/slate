struct S {
  short int i  : 12;
  char      c1 : 1;
  char      c2 : 1;
  char      c3 : 1;
  char      c4 : 1;
};

int main(void) {
  struct S s0 = {341, 1, 1, 1, 1};
  char    *p  = (char *)&s0;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  if (*p != 85)
    __builtin_abort();
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  if (*p != 21)
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 i: i16 : 12;
// DEFAULT-NEXT:         field1 c1: i8 : 1;
// DEFAULT-NEXT:         field2 c2: i8 : 1;
// DEFAULT-NEXT:         field3 c3: i8 : 1;
// DEFAULT-NEXT:         field4 c4: i8 : 1;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0, 1, 1, 1, 1], bit_offsets=[Some(0), Some(12), Some(13), Some(14), Some(15)], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s0:[0-9]+]] s0: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(341)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field2 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field3 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field4 = truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s0]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_p]])))), const<i32>(85))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
