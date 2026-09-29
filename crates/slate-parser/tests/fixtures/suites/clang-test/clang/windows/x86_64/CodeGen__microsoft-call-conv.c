
void __fastcall f1(void);
void __stdcall f2(void);
void __fastcall f4(void) {
  f1();
}
void __stdcall f5(void) {
  f2();
}

// PR5280
void (__fastcall *pf1)(void) = f1;
void (__stdcall *pf2)(void) = f2;
void (__fastcall *pf4)(void) = f4;
void (__stdcall *pf5)(void) = f5;

int main(void) {
    f4(); f5();
    pf1(); pf2(); pf4(); pf5();
    return 0;
}

// PR7117
void __stdcall f7(foo) int foo; {}
void f8(void) {
  f7(0);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wno-strict-prototypes

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
// DEFAULT-NEXT:     global %[[VALUE_pf1:[0-9]+]] pf1: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%[[VALUE_f1:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pf2:[0-9]+]] pf2: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%[[VALUE_f2:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pf4:[0-9]+]] pf4: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%[[VALUE_f4:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pf5:[0-9]+]] pf5: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%[[VALUE_f5:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1]] @f1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2]] @f2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f4]] @f4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5]] @f5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f4]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f5]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>>(%[[VALUE_pf1]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>>(%[[VALUE_pf2]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>>(%[[VALUE_pf4]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>>(%[[VALUE_pf5]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_foo:[0-9]+]] foo: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_f7]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
