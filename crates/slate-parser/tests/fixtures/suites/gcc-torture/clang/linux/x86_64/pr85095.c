/* PR target/85095 */

__attribute__((noipa)) unsigned long f1(unsigned long a, unsigned long b) {
  unsigned long i = __builtin_add_overflow(a, b, &a);
  return a + i;
}

__attribute__((noipa)) unsigned long f2(unsigned long a, unsigned long b) {
  unsigned long i = __builtin_add_overflow(a, b, &a);
  return a - i;
}

__attribute__((noipa)) unsigned long f3(unsigned int a, unsigned int b) {
  unsigned int i = __builtin_add_overflow(a, b, &a);
  return a + i;
}

__attribute__((noipa)) unsigned long f4(unsigned int a, unsigned int b) {
  unsigned int i = __builtin_add_overflow(a, b, &a);
  return a - i;
}

int main() {
  if (f1(16UL, -18UL) != -2UL || f1(16UL, -17UL) != -1UL ||
      f1(16UL, -16UL) != 1UL || f1(16UL, -15UL) != 2UL ||
      f2(24UL, -26UL) != -2UL || f2(24UL, -25UL) != -1UL ||
      f2(24UL, -24UL) != -1UL || f2(24UL, -23UL) != 0UL ||
      f3(32U, -34U) != -2U || f3(32U, -33U) != -1U || f3(32U, -32U) != 1U ||
      f3(32U, -31U) != 2U || f4(35U, -37U) != -2U || f4(35U, -36U) != -1U ||
      f4(35U, -35U) != -1U || f4(35U, -34U) != 0U)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_a:[0-9]+]] a: u64, %[[VALUE_b:[0-9]+]] b: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u64 [storage=automatic] = from_bool<u64, reason=assign>(overflow_add<bool>(read<u64>(%[[VALUE_a]]), read<u64>(%[[VALUE_b]]), deref(addr_of<ptr<u64>>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(read<u64>(%[[VALUE_a]]), read<u64>(%[[VALUE_i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_a_2:[0-9]+]] a: u64, %[[VALUE_b_2:[0-9]+]] b: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: u64 [storage=automatic] = from_bool<u64, reason=assign>(overflow_add<bool>(read<u64>(%[[VALUE_a_2]]), read<u64>(%[[VALUE_b_2]]), deref(addr_of<ptr<u64>>(%[[VALUE_a_2]]))));
// DEFAULT-NEXT:         return sub<u64, overflow=wrap>(read<u64>(%[[VALUE_a_2]]), read<u64>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_a_3:[0-9]+]] a: u32, %[[VALUE_b_3:[0-9]+]] b: u32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: u32 [storage=automatic] = from_bool<u32, reason=assign>(overflow_add<bool>(read<u32>(%[[VALUE_a_3]]), read<u32>(%[[VALUE_b_3]]), deref(addr_of<ptr<u32>>(%[[VALUE_a_3]]))));
// DEFAULT-NEXT:         return widen<u64, reason=return>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_a_3]]), read<u32>(%[[VALUE_i_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_a_4:[0-9]+]] a: u32, %[[VALUE_b_4:[0-9]+]] b: u32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_4:[0-9]+]] i: u32 [storage=automatic] = from_bool<u32, reason=assign>(overflow_add<bool>(read<u32>(%[[VALUE_a_4]]), read<u32>(%[[VALUE_b_4]]), deref(addr_of<ptr<u32>>(%[[VALUE_a_4]]))));
// DEFAULT-NEXT:         return widen<u64, reason=return>(sub<u32, overflow=wrap>(read<u32>(%[[VALUE_a_4]]), read<u32>(%[[VALUE_i_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_f1]], const<u64>(16), neg<u64, overflow=wrap>(const<u64>(18))), neg<u64, overflow=wrap>(const<u64>(2)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_f1]], const<u64>(16), neg<u64, overflow=wrap>(const<u64>(17))), neg<u64, overflow=wrap>(const<u64>(1))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_f1]], const<u64>(16), neg<u64, overflow=wrap>(const<u64>(16))), const<u64>(1)));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_f1]], const<u64>(16), neg<u64, overflow=wrap>(const<u64>(15))), const<u64>(2)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_f2]], const<u64>(24), neg<u64, overflow=wrap>(const<u64>(26))), neg<u64, overflow=wrap>(const<u64>(2))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_f2]], const<u64>(24), neg<u64, overflow=wrap>(const<u64>(25))), neg<u64, overflow=wrap>(const<u64>(1))));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_f2]], const<u64>(24), neg<u64, overflow=wrap>(const<u64>(24))), neg<u64, overflow=wrap>(const<u64>(1))));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_f2]], const<u64>(24), neg<u64, overflow=wrap>(const<u64>(23))), const<u64>(0)));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%[[VALUE_f3]], const<u32>(32), neg<u32, overflow=wrap>(const<u32>(34))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(2)))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%[[VALUE_f3]], const<u32>(32), neg<u32, overflow=wrap>(const<u32>(33))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%[[VALUE_f3]], const<u32>(32), neg<u32, overflow=wrap>(const<u32>(32))), widen<u64, reason=usual_arith>(const<u32>(1))));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%[[VALUE_f3]], const<u32>(32), neg<u32, overflow=wrap>(const<u32>(31))), widen<u64, reason=usual_arith>(const<u32>(2))));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%[[VALUE_f4]], const<u32>(35), neg<u32, overflow=wrap>(const<u32>(37))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(2)))));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE11]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%[[VALUE_f4]], const<u32>(35), neg<u32, overflow=wrap>(const<u32>(36))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%[[VALUE_f4]], const<u32>(35), neg<u32, overflow=wrap>(const<u32>(35))), widen<u64, reason=usual_arith>(neg<u32, overflow=wrap>(const<u32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE13]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], ne<u64>(call<u64, signature=fn(u32, u32) -> u64>(%[[VALUE_f4]], const<u32>(35), neg<u32, overflow=wrap>(const<u32>(34))), widen<u64, reason=usual_arith>(const<u32>(0))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE14]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
