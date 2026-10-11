
struct foo {
  int x;
  float y;
  char z;
};

void __attribute__((ms_abi)) f1(void);
void __attribute__((sysv_abi)) f2(void);
void f3(void) {
  f1();
  f2();
}

// Win64 ABI varargs
void __attribute__((ms_abi)) f4(int a, ...) {
  __builtin_ms_va_list ap;
  __builtin_ms_va_start(ap, a);
  int b = __builtin_va_arg(ap, int);
  double _Complex c = __builtin_va_arg(ap, double _Complex);
  struct foo d = __builtin_va_arg(ap, struct foo);
  __builtin_ms_va_list ap2;
  __builtin_ms_va_copy(ap2, ap);
  __builtin_ms_va_end(ap);
}

// Let's verify that normal va_lists work right on Win64, too.
void f5(int a, ...) {
  __builtin_va_list ap;
  __builtin_va_start(ap, a);
  int b = __builtin_va_arg(ap, int);
  double _Complex c = __builtin_va_arg(ap, double _Complex);
  struct foo d = __builtin_va_arg(ap, struct foo);
  __builtin_va_list ap2;
  __builtin_va_copy(ap2, ap);
  __builtin_va_end(ap);
}

// Verify that using a Win64 va_list from a System V function works.
void __attribute__((sysv_abi)) f6(__builtin_ms_va_list ap) {
  int b = __builtin_va_arg(ap, int);
  double _Complex c = __builtin_va_arg(ap, double _Complex);
  struct foo d = __builtin_va_arg(ap, struct foo);
  __builtin_ms_va_list ap2;
  __builtin_ms_va_copy(ap2, ap);
}

// This test checks if structs are passed according to Win64 calling convention
// when it's enforced by __attribute((ms_abi)).
struct i128 {
  unsigned long long a;
  unsigned long long b;
};

__attribute__((ms_abi)) struct i128 f7(struct i128 a) {
  return a;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: f32;
// DEFAULT-NEXT:         field2 z: i8;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_i128:[0-9]+]] i128 = struct {
// DEFAULT-NEXT:         field0 a: u64;
// DEFAULT-NEXT:         field1 b: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external] [abi=sysv64 sysv_abi() -> void];
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f1]]);
// DEFAULT-NEXT:         call<void, signature=fn sysv_abi() -> void, abi=sysv64 sysv_abi() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_a:[0-9]+]] a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: complex<f64> [storage=automatic] = va_arg<complex<f64>>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE_foo]] [storage=automatic] = copy<@type[[TYPE_foo]], reason=assign>(va_arg<@type[[TYPE_foo]]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         let %[[VALUE_ap2:[0-9]+]] ap2: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_copy(%[[VALUE_ap2]], %[[VALUE_ap]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: i32 [storage=automatic] = va_arg<i32>(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: complex<f64> [storage=automatic] = va_arg<complex<f64>>(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: @type[[TYPE_foo]] [storage=automatic] = copy<@type[[TYPE_foo]], reason=assign>(va_arg<@type[[TYPE_foo]]>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:         let %[[VALUE_ap2_2:[0-9]+]] ap2: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_copy(%[[VALUE_ap2_2]], %[[VALUE_ap_2]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_ap_3:[0-9]+]] ap: ptr<i8>) -> void [linkage=external] [abi=sysv64 sysv_abi(scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_b_3:[0-9]+]] b: i32 [storage=automatic] = va_arg<i32>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         let %[[VALUE_c_3:[0-9]+]] c: complex<f64> [storage=automatic] = va_arg<complex<f64>>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         let %[[VALUE_d_3:[0-9]+]] d: @type[[TYPE_foo]] [storage=automatic] = copy<@type[[TYPE_foo]], reason=assign>(va_arg<@type[[TYPE_foo]]>(%[[VALUE_ap_3]]));
// DEFAULT-NEXT:         let %[[VALUE_ap2_3:[0-9]+]] ap2: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_copy(%[[VALUE_ap2_3]], %[[VALUE_ap_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_a_3:[0-9]+]] a: @type[[TYPE_i128]]) -> @type[[TYPE_i128]] [linkage=external] [abi=win64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_i128]], reason=return>(read<@type[[TYPE_i128]]>(%[[VALUE_a_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
