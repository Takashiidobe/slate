/* Test that both arrays are initialized by store_by_pieces.  */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-options "-O2 -mtune=i686" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

struct A { char c[10]; };
extern void baz (struct A *);

void
foo (void)
{
  struct A a = { "abcdefghi" };
  baz (&a);
}

void
bar (void)
{
  struct A a;
  __builtin_strcpy (&a.c[0], "abcdefghi");
  baz (&a);
}

/* { dg-final { scan-assembler-not "abcdefghi" { target i?86-*-* x86_64-*-* ia64-*-* } } } */
/* { dg-final { scan-assembler-times "7017280452245743464\|7523094288207667809\|6867666564636261\|1684234849\|64636261" 2 { target i?86-*-* x86_64-*-* ia64-*-* } } } */

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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 10>;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_A]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic] = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 0]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_baz]], addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcpy:[0-9]+]] @__builtin_strcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strcpy]], addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(field0(%[[VALUE_a_2]])), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_baz]], addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
