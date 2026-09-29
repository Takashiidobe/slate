
unsigned char a = 6;
int           b, c;

static void fn1() {
  int           i = a > 1 ? 1 : a, j = 6 & (c = a && (b = a));
  int           d = 0, e = a, f = ~c, g = b || a;
  unsigned char h = ~a;
  if (a)
    f = j;
  if (h && g)
    d = a;
  i = -~(f * d * h) + c && (e || i) ^ f;
  if (i != 1)
    __builtin_abort();
}

int main() {
  fn1();
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = conditional<i32>(gt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a]]))), const<i32>(1)), const<i32>(1), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u8>(read<u8>(%[[VALUE_a]]), const<u8>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%[[VALUE_a]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%[[VALUE_a]]))), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(false));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE0]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], and<i32>(const<i32>(6), from_bool<i32, reason=assign>(read<bool>(%[[VALUE0]]))));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%[[VALUE_a]])));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic] = not<i32>(read<i32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0)), ne<u8>(read<u8>(%[[VALUE_a]]), const<u8>(0))));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a]]))))));
// DEFAULT-NEXT:         if ne<u8>(read<u8>(%[[VALUE_a]]), const<u8>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_f]], read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:         if logical_and<bool>(ne<u8>(read<u8>(%[[VALUE_h]]), const<u8>(0)), ne<i32>(read<i32>(%[[VALUE_g]]), const<i32>(0)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_d]], reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(add<i32, overflow=ub>(neg<i32, overflow=ub>(not<i32>(mul<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_f]]), read<i32>(%[[VALUE_d]])), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_h]])))))), read<i32>(%[[VALUE_c]])), const<i32>(0)), ne<i32>(xor<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))), read<i32>(%[[VALUE_f]])), const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fn1]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
