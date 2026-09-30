/* PR rtl-optimization/119291 */

int  a;
long c;

__attribute__((noipa)) void foo(int x) {
  if (x != 0)
    __builtin_abort();
  a = 42;
}

int main() {
  int e = 1;
lab:
  if (a < 2) {
    int           b = e;
    _Bool         d = a != 0;
    _Bool         f = b != 0;
    unsigned long g = -(d & f);
    unsigned long h = c & g;
    unsigned long i = ~c;
    e               = -(i & h);
    c               = e != 0;
    a               = ~e + b;
    foo(e);
    goto lab;
  }
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         label %[[VALUE_lab:[0-9]+]] lab:
// DEFAULT-NEXT:             if lt<i32>(read<i32>(%[[VALUE_a]]), const<i32>(2))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                     let %[[VALUE_d:[0-9]+]] d: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0));
// DEFAULT-NEXT:                     let %[[VALUE_f:[0-9]+]] f: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0));
// DEFAULT-NEXT:                     let %[[VALUE_g:[0-9]+]] g: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_d]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_f]]))))));
// DEFAULT-NEXT:                     let %[[VALUE_h:[0-9]+]] h: u64 [storage=automatic] = and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_c]])), read<u64>(%[[VALUE_g]]));
// DEFAULT-NEXT:                     let %[[VALUE_i:[0-9]+]] i: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(not<i64>(read<i64>(%[[VALUE_c]])));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_e]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(neg<u64, overflow=wrap>(and<u64>(read<u64>(%[[VALUE_i]]), read<u64>(%[[VALUE_h]]))))));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_c]], from_bool<i64, reason=assign>(ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], add<i32, overflow=ub>(not<i32>(read<i32>(%[[VALUE_e]])), read<i32>(%[[VALUE_b]])));
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_e]]));
// DEFAULT-NEXT:                     goto %[[VALUE_lab]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
