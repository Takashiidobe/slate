/* Test for constant expressions: invalid null pointer constants in
   various contexts (make sure NOPs are not inappropriately
   stripped).  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

__extension__ typedef __SIZE_TYPE__ size_t;

void *p = (size_t)(void *)0; /* { dg-error "without a cast" } */
struct s { void *a; } q = { (size_t)(void *)0 }; /* { dg-error "without a cast|near initialization" } */
void *
f (void)
{
  void *r;
  r = (size_t)(void *)0; /* { dg-error "without a cast" } */
  return (size_t)(void *)0; /* { dg-error "without a cast" } */
}
void g (void *); /* { dg-message "but argument is of type" } */
void
h (void)
{
  g ((size_t)(void *)0); /* { dg-error "without a cast" } */
}
void g2 (int, void *); /* { dg-message "but argument is of type" } */
void
h2 (void)
{
  g2 (0, (size_t)(void *)0); /* { dg-error "without a cast" } */
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=static] = int_to_ptr<ptr<void>, reason=assign>(ptr_to_int<u64, reason=explicit>(null<ptr<void>>)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = int_to_ptr<ptr<void>, reason=assign>(ptr_to_int<u64, reason=explicit>(null<ptr<void>>))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_r]], int_to_ptr<ptr<void>, reason=assign>(ptr_to_int<u64, reason=explicit>(null<ptr<void>>)));
// DEFAULT-NEXT:         return int_to_ptr<ptr<void>, reason=return>(ptr_to_int<u64, reason=explicit>(null<ptr<void>>));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_h:[0-9]+]] @h() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_g]], int_to_ptr<ptr<void>, reason=arg>(ptr_to_int<u64, reason=explicit>(null<ptr<void>>)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g2:[0-9]+]] @g2(%[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_h2:[0-9]+]] @h2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<void>) -> void>(%[[VALUE_g2]], const<i32>(0), int_to_ptr<ptr<void>, reason=arg>(ptr_to_int<u64, reason=explicit>(null<ptr<void>>)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
