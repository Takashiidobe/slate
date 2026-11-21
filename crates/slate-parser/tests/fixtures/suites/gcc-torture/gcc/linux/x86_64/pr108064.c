/* PR tree-optimization/108064 */

static inline short foo(short value) {
  return ((value >> 8) & 0xff) | ((value & 0xff) << 8);
}

__attribute__((noipa)) void bar(short *d, const short *s) {
  for (unsigned long i = 0; i < 4; i++)
    d[i] = foo(s[i]);
}

int main() {
  short a[4] __attribute__((aligned(16))) = {0xff, 0, 0, 0};
  short b[4] __attribute__((aligned(16)));
  short c[4] __attribute__((aligned(16)));

  bar(b, a);
  bar(c, b);
  if (a[0] != c[0])
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_value:[0-9]+]] value: i16) -> i16 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(or<i32>(and<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_value]])), const<i32>(8)), const<i32>(255)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(and<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_value]])), const<i32>(255)), const<i32>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_d:[0-9]+]] d: ptr<i16>, %[[VALUE_s:[0-9]+]] s: ptr<const i16>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%[[VALUE_i]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%[[VALUE_d]]), read<u64>(%[[VALUE_i]]))), call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo]], read<i16>(deref(ptr_offset<ptr<const i16>, subtract=false, element=i16, overflow=ub>(read<ptr<const i16>>(%[[VALUE_s]]), read<u64>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i16, 4> [storage=automatic] [align=16] = aggregate<array<i16, 4>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(255)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), index2 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), index3 = truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<i16, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: array<i16, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i16>, ptr<const i16>) -> void>(%[[VALUE_bar]], array_decay<ptr<i16>, length=Some(4)>(%[[VALUE_b]]), pointer_cast<ptr<const i16>, reason=arg>(array_decay<ptr<i16>, length=Some(4)>(%[[VALUE_a]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i16>, ptr<const i16>) -> void>(%[[VALUE_bar]], array_decay<ptr<i16>, length=Some(4)>(%[[VALUE_c]]), pointer_cast<ptr<const i16>, reason=arg>(array_decay<ptr<i16>, length=Some(4)>(%[[VALUE_b]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(%[[VALUE_a]]), const<i32>(0))))), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(4)>(%[[VALUE_c]]), const<i32>(0))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
