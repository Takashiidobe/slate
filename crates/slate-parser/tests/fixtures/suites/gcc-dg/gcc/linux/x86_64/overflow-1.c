/* PR optimization/13318 */
/* Origin: <bremner@unb.ca> */
/* Reduced testcase: Wolfgang Bangerth <bangerth@dealii.org> */

/* Verify that the big multiplier doesn't cause an integer
   overflow in the loop optimizer.  */

/* { dg-do compile } */
/* { dg-options "-O2" } */

struct S {
  int key;
  int rnext,rprev;
};
 
void foo(struct S* H)
{
  int i, k;
  for (i=0; i<2; i++){
    struct S* cell=H+k;
    cell->key=i*(0xffffffffUL/2);
    cell->rnext=k+(1-i);
    cell->rprev=k+(1-i);
  }
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 key: i32;
// DEFAULT-NEXT:         field1 rnext: i32;
// DEFAULT-NEXT:         field2 rprev: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_H:[0-9]+]] H: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_cell:[0-9]+]] cell: ptr<@type[[TYPE_S]]> [storage=automatic] = ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_H]]), read<i32>(%[[VALUE_k]]));
// DEFAULT-NEXT:                     write<i32>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_cell]]))), reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(4294967295), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))))));
// DEFAULT-NEXT:                     write<i32>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_cell]]))), add<i32, overflow=ub>(read<i32>(%[[VALUE_k]]), sub<i32, overflow=ub>(const<i32>(1), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:                     write<i32>(field2(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_cell]]))), add<i32, overflow=ub>(read<i32>(%[[VALUE_k]]), sub<i32, overflow=ub>(const<i32>(1), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
