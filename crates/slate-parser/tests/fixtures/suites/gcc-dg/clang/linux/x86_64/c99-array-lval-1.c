/* Test for non-lvalue arrays decaying to pointers: in C99 only.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

struct s { char c[1]; };

extern struct s foo (void);

void
bar (void)
{
  char *t;
  (foo ()).c[0]; /* { dg-bogus "non-lvalue" "array not decaying to lvalue" } */
  t = (foo ()).c; /* { dg-bogus "non-lvalue" "array not decaying to lvalue" } */
  (foo ()).c + 1; /* { dg-bogus "non-lvalue" "array not decaying to lvalue" } */
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> @type[[TYPE_s]] [linkage=external] [abi=sysv64() -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(temporary %[[VALUE0:[0-9]+]] = call<@type[[TYPE_s]], signature=fn() -> @type[[TYPE_s]], abi=sysv64() -> native_c>(%[[VALUE_foo]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_t]], array_decay<ptr<i8>, length=Some(1)>(field0(temporary %[[VALUE1:[0-9]+]] = call<@type[[TYPE_s]], signature=fn() -> @type[[TYPE_s]], abi=sysv64() -> native_c>(%[[VALUE_foo]]))));
// DEFAULT-NEXT:         ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(temporary %[[VALUE2:[0-9]+]] = call<@type[[TYPE_s]], signature=fn() -> @type[[TYPE_s]], abi=sysv64() -> native_c>(%[[VALUE_foo]]))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
