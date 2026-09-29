/* { dg-do compile } */
/* { dg-options "-O2 -fno-ipa-icf -fwrapv-pointer -fdump-tree-optimized" } */

_Bool
f1 (char *a, char *b)
{
  return (a + 16 <= b) || (b + 16 <= a);
}

_Bool
f2 (char *a, char *b)
{
  return (a + 15 < b) || (b + 15 < a);
}

_Bool
f3 (char *a, char *b)
{
  return (a + 16 <= b) | (b + 16 <= a);
}

_Bool
f4 (char *a, char *b)
{
  return (a + 15 < b) | (b + 15 < a);
}

/* { dg-final { scan-tree-dump-not { = [^\n]* - [^\n]*;} "optimized" } } */
/* { dg-final { scan-tree-dump-times { = [^\n]* \+ [^\n]*;} 8 "optimized" } } */
/* { dg-final { scan-tree-dump-times { \+ 15} 4 "optimized" } } */
/* { dg-final { scan-tree-dump-times { \+ 16} 4 "optimized" } } */

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
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_a:[0-9]+]] a: ptr<i8>, %[[VALUE_b:[0-9]+]] b: ptr<i8>) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return logical_or<bool>(le<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_a]]), const<i32>(16)), read<ptr<i8>>(%[[VALUE_b]])), le<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_b]]), const<i32>(16)), read<ptr<i8>>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_a_2:[0-9]+]] a: ptr<i8>, %[[VALUE_b_2:[0-9]+]] b: ptr<i8>) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return logical_or<bool>(lt<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_a_2]]), const<i32>(15)), read<ptr<i8>>(%[[VALUE_b_2]])), lt<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_b_2]]), const<i32>(15)), read<ptr<i8>>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_a_3:[0-9]+]] a: ptr<i8>, %[[VALUE_b_3:[0-9]+]] b: ptr<i8>) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(or<i32>(from_bool<i32, reason=promotion>(le<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_a_3]]), const<i32>(16)), read<ptr<i8>>(%[[VALUE_b_3]]))), from_bool<i32, reason=promotion>(le<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_b_3]]), const<i32>(16)), read<ptr<i8>>(%[[VALUE_a_3]])))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_a_4:[0-9]+]] a: ptr<i8>, %[[VALUE_b_4:[0-9]+]] b: ptr<i8>) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(or<i32>(from_bool<i32, reason=promotion>(lt<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_a_4]]), const<i32>(15)), read<ptr<i8>>(%[[VALUE_b_4]]))), from_bool<i32, reason=promotion>(lt<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_b_4]]), const<i32>(15)), read<ptr<i8>>(%[[VALUE_a_4]])))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
