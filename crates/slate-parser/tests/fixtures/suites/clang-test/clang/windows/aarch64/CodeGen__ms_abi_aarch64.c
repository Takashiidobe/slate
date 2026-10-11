
struct small_odd {
  char a, b, c;
};

struct larger {
  int a, b, c, d, e;
};

void __attribute__((ms_abi)) f1(void);
void f2(void);
void f3(void) {
  f1();
  f2();
}

// Win64 ABI varargs
void __attribute__((ms_abi)) f4(int a, ...) {
  __builtin_ms_va_list ap;
  __builtin_ms_va_start(ap, a);
  int b = __builtin_va_arg(ap, int);
  __builtin_ms_va_list ap2;
  __builtin_ms_va_copy(ap2, ap);
  __builtin_ms_va_end(ap);
}

void __attribute__((ms_abi)) f4_2(int a, ...) {
  __builtin_ms_va_list ap;
  __builtin_ms_va_start(ap, a);
  struct small_odd s1 = __builtin_va_arg(ap, struct small_odd);
  struct larger s2 = __builtin_va_arg(ap, struct larger);
  __builtin_ms_va_end(ap);
}

// Let's verify that normal va_lists work right on Win64, too.
void f5(int a, ...) {
  __builtin_va_list ap;
  __builtin_va_start(ap, a);
  int b = __builtin_va_arg(ap, int);
  __builtin_va_list ap2;
  __builtin_va_copy(ap2, ap);
  __builtin_va_end(ap);
}

struct HFA {
  float a, b, c;
};

__attribute__((ms_abi)) void msabi_hfa(struct HFA a);
__attribute__((ms_abi)) void msabi_hfa_vararg(struct HFA a, int b, ...);

void call_msabi_hfa(void) {
  msabi_hfa((struct HFA){1.0f, 2.0f, 3.0f});
}

void call_msabi_hfa_vararg(void) {
  msabi_hfa_vararg((struct HFA){1.0f, 2.0f, 3.0f}, 4,
                   (struct HFA){5.0f, 6.0f, 7.0f});
}

__attribute__((ms_abi)) void get_msabi_hfa_vararg(int a, ...) {
  __builtin_ms_va_list ap;
  __builtin_ms_va_start(ap, a);
  struct HFA b = __builtin_va_arg(ap, struct HFA);
  __builtin_ms_va_end(ap);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_small_odd:[0-9]+]] small_odd = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_larger:[0-9]+]] larger = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:         field4 e: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_HFA:[0-9]+]] HFA = struct {
// DEFAULT-NEXT:         field0 a: f32;
// DEFAULT-NEXT:         field1 b: f32;
// DEFAULT-NEXT:         field2 c: f32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_a:[0-9]+]] a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE_ap2:[0-9]+]] ap2: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_copy(%[[VALUE_ap2]], %[[VALUE_ap]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4_2:[0-9]+]] @f4_2(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         let %[[VALUE_s1:[0-9]+]] s1: @type[[TYPE_small_odd]] [storage=automatic] = copy<@type[[TYPE_small_odd]], reason=assign>(va_arg<@type[[TYPE_small_odd]]>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:         let %[[VALUE_s2:[0-9]+]] s2: @type[[TYPE_larger]] [storage=automatic] = copy<@type[[TYPE_larger]], reason=assign>(va_arg<@type[[TYPE_larger]]>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_a_3:[0-9]+]] a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: i32 [storage=automatic] = va_arg<i32>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         let %[[VALUE_ap2_2:[0-9]+]] ap2: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_copy(%[[VALUE_ap2_2]], %[[VALUE_ap_3]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_msabi_hfa:[0-9]+]] @msabi_hfa(%[[VALUE_a_4:[0-9]+]] a: @type[[TYPE_HFA]]) -> void [linkage=external] [abi=win_arm64(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_msabi_hfa_vararg:[0-9]+]] @msabi_hfa_vararg(%[[VALUE_a_5:[0-9]+]] a: @type[[TYPE_HFA]], %[[VALUE_b_3:[0-9]+]] b: i32, ...) -> void [linkage=external] [abi=win_arm64(native_c, scalar) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_call_msabi_hfa:[0-9]+]] @call_msabi_hfa() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_HFA]]) -> void, abi=win_arm64(native_c) -> void>(%[[VALUE_msabi_hfa]], copy<@type[[TYPE_HFA]], reason=arg>(read<@type[[TYPE_HFA]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_HFA]], zero_fill=false>(field0 = const<f32>(1.0), field1 = const<f32>(2.0), field2 = const<f32>(3.0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_call_msabi_hfa_vararg:[0-9]+]] @call_msabi_hfa_vararg() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_HFA]], i32, ...) -> void, abi=win_arm64(native_c, scalar, coerce<i64, i64>) -> void>(%[[VALUE_msabi_hfa_vararg]], copy<@type[[TYPE_HFA]], reason=arg>(read<@type[[TYPE_HFA]]>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_HFA]], zero_fill=false>(field0 = const<f32>(1.0), field1 = const<f32>(2.0), field2 = const<f32>(3.0)))), const<i32>(4), copy<@type[[TYPE_HFA]], reason=vararg>(read<@type[[TYPE_HFA]]>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_HFA]], zero_fill=false>(field0 = const<f32>(5.0), field1 = const<f32>(6.0), field2 = const<f32>(7.0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_get_msabi_hfa_vararg:[0-9]+]] @get_msabi_hfa_vararg(%[[VALUE_a_6:[0-9]+]] a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         let %[[VALUE_b_4:[0-9]+]] b: @type[[TYPE_HFA]] [storage=automatic] = copy<@type[[TYPE_HFA]], reason=assign>(va_arg<@type[[TYPE_HFA]]>(%[[VALUE_ap_4]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
