/* PR c/56724 */
/* { dg-do compile } */
/* { dg-options "-fpermissive -Wc++-compat -Wpedantic" } */

enum E1 { A };
enum E2 { B };
extern void foo_E (enum E1);
extern void foo_v (void *p);
extern void foo_sc (int, int, signed char *);
extern unsigned char *uc;
extern signed char sc;
extern const signed char *csc;
extern float *f;

void
foo (void)
{
  void (*fp)(void);
  const void (*ffp)(void);
  foo_v (fp); /* { dg-warning "10:ISO C forbids passing argument" } */
  foo_E (B); /* { dg-warning "10:enum conversion when passing argument" } */
  foo_sc (1, 2, uc); /* { dg-warning "17:pointer targets in passing argument" } */
  foo_sc (1, 2, f); /* { dg-warning "17:passing argument" } */
  foo_sc (1, 2, sc); /* { dg-warning "17:passing argument" } */
  foo_sc (uc, 2, &sc); /* { dg-warning "11:passing argument" } */
  foo_sc (1, 2, csc); /* { dg-warning "17:passing argument" } */
}

typedef void (*fp)(void);
typedef void (*nrfp)(void) __attribute__((noreturn));
void f1 (nrfp); void f2 (fp x) { f1 (x); } extern int e; /* { dg-warning "38:passing argument" } */

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_E1:[0-9]+]] E1 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_E2:[0-9]+]] E2 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] B = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_fp:[0-9]+]] fp = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_nrfp:[0-9]+]] nrfp = ptr<fn() -> void>;
// DEFAULT-NEXT:     extern %[[VALUE_uc:[0-9]+]] uc: ptr<u8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_sc:[0-9]+]] sc: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_csc:[0-9]+]] csc: ptr<const i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_f:[0-9]+]] f: ptr<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_E:[0-9]+]] @foo_E(%[[VALUE0:[0-9]+]] <unnamed>: @type[[TYPE_E1]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_v:[0-9]+]] @foo_v(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_sc:[0-9]+]] @foo_sc(%[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: ptr<i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_fp:[0-9]+]] fp: ptr<fn() -> void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ffp:[0-9]+]] ffp: ptr<fn() -> void> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_foo_v]], pointer_cast<ptr<void>, reason=arg>(read<ptr<fn() -> void>>(%[[VALUE_fp]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_E1]]) -> void>(%[[VALUE_foo_E]], int_to_enum<@type[[TYPE_E1]], reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>) -> void>(%[[VALUE_foo_sc]], const<i32>(1), const<i32>(2), pointer_cast<ptr<i8>, reason=arg>(read<ptr<u8>>(%[[VALUE_uc]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>) -> void>(%[[VALUE_foo_sc]], const<i32>(1), const<i32>(2), pointer_cast<ptr<i8>, reason=arg>(read<ptr<f32>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>) -> void>(%[[VALUE_foo_sc]], const<i32>(1), const<i32>(2), int_to_ptr<ptr<i8>, reason=arg>(read<i8>(%[[VALUE_sc]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>) -> void>(%[[VALUE_foo_sc]], ptr_to_int<i32, reason=arg>(read<ptr<u8>>(%[[VALUE_uc]])), const<i32>(2), addr_of<ptr<i8>>(%[[VALUE_sc]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>) -> void>(%[[VALUE_foo_sc]], const<i32>(1), const<i32>(2), pointer_cast<ptr<i8>, reason=arg>(read<ptr<const i8>>(%[[VALUE_csc]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE4:[0-9]+]] <unnamed>: ptr<fn() -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x:[0-9]+]] x: ptr<fn() -> void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<fn() -> void>) -> void>(%[[VALUE_f1]], read<ptr<fn() -> void>>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
