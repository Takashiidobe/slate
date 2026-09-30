
__declspec(selectany) int x1 = 1;
const __declspec(selectany) int x2 = 2;

// selectany turns extern variable declarations into definitions.
__declspec(selectany) int x3;
extern __declspec(selectany) int x4;

struct __declspec(align(16)) S {
  char x;
};
union { struct S s; } u;



__declspec(naked) void t3(void) {}

void __declspec(nothrow) t22(void);
void t22(void) {}

__declspec(noinline) void t2(void) {}

__declspec(noreturn) void f20_t(void);
void f20(void) { f20_t(); }

__declspec(noalias) void noalias_callee(int *x);
void noalias_caller(int *x) { noalias_callee(x); }

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 s: @type[[TYPE_S]];
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_x1:[0-9]+]] x1: i32 [storage=static] = const<i32>(1) [linkage=external] [selectany];
// DEFAULT-NEXT:     global %[[VALUE_x2:[0-9]+]] x2: i32 [storage=static] [const] = const<i32>(2) [linkage=external] [selectany];
// DEFAULT-NEXT:     global %[[VALUE_x3:[0-9]+]] x3: i32 [storage=static] [linkage=external] [selectany];
// DEFAULT-NEXT:     extern %[[VALUE_x4:[0-9]+]] x4: i32 [storage=static] [linkage=external] [selectany];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_t3:[0-9]+]] @t3() -> void [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t22:[0-9]+]] @t22() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t2:[0-9]+]] @t2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f20_t:[0-9]+]] @f20_t() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f20:[0-9]+]] @f20() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f20_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_noalias_callee:[0-9]+]] @noalias_callee(%[[VALUE_x:[0-9]+]] x: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_noalias_caller:[0-9]+]] @noalias_caller(%[[VALUE_x_2:[0-9]+]] x: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_noalias_callee]], read<ptr<i32>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
