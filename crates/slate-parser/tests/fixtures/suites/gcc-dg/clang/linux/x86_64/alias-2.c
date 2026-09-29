// { dg-do compile }
// { dg-options "-Wstrict-aliasing=2 -fstrict-aliasing" }

struct foo {
  char c;
  char d;
  short s;
  int i;
} bar;

int
sub1 (long long int foobar)
{
  struct foo *tmp = (struct foo *) &foobar; // { dg-warning "type-punned pointer will" }
  return tmp->i;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 d: i8;
// DEFAULT-NEXT:         field2 s: i16;
// DEFAULT-NEXT:         field3 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 1, 2, 4]];
// DEFAULT-NEXT:     global %[[VALUE_bar:[0-9]+]] bar: @type[[TYPE_foo]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sub1:[0-9]+]] @sub1(%[[VALUE_foobar:[0-9]+]] foobar: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: ptr<@type[[TYPE_foo]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_foo]]>, reason=explicit>(addr_of<ptr<i64>>(%[[VALUE_foobar]]));
// DEFAULT-NEXT:         return read<i32>(field3(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_tmp]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
