/* Test for non-lvalue arrays decaying to pointers: in C99 only.
   Test various ways of producing non-lvalue arrays.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

struct s { char c[1]; };
struct s a, b, c;
int d;

void
bar (void)
{
  char *t;
  (d ? b : c).c[0];
  (d, b).c[0];
  (a = b).c[0];
  t = (d ? b : c).c;
  t = (d, b).c;
  t = (a = b).c;
  (d ? b : c).c + 1;
  (d, b).c + 1;
  (a = b).c + 1;
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %1 a: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 t: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(temporary %7 = conditional<@type0>(ne<i32>(read<i32>(%4), const<i32>(0)), read<@type0>(%2), read<@type0>(%3)))), const<i32>(0))));
// DEFAULT-NEXT:         read<i32>(%4);
// DEFAULT-NEXT:         write<@type0>(%1, copy<@type0, reason=assign>(read<@type0>(%2)));
// DEFAULT-NEXT:         write<ptr<i8>>(%6, array_decay<ptr<i8>, length=Some(1)>(field0(temporary %10 = conditional<@type0>(ne<i32>(read<i32>(%4), const<i32>(0)), read<@type0>(%2), read<@type0>(%3)))));
// DEFAULT-NEXT:         read<i32>(%4);
// DEFAULT-NEXT:         write<ptr<i8>>(%6, array_decay<ptr<i8>, length=Some(1)>(field0(temporary %11 = read<@type0>(%2))));
// DEFAULT-NEXT:         write<@type0>(%1, copy<@type0, reason=assign>(read<@type0>(%2)));
// DEFAULT-NEXT:         write<ptr<i8>>(%6, array_decay<ptr<i8>, length=Some(1)>(field0(temporary %12 = copy<@type0, reason=assign>(read<@type0>(%2)))));
// DEFAULT-NEXT:         ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(temporary %13 = conditional<@type0>(ne<i32>(read<i32>(%4), const<i32>(0)), read<@type0>(%2), read<@type0>(%3)))), const<i32>(1));
// DEFAULT-NEXT:         read<i32>(%4);
// DEFAULT-NEXT:         write<@type0>(%1, copy<@type0, reason=assign>(read<@type0>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
