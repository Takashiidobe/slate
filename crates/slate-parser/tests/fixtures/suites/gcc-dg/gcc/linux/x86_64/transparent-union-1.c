/* PR c/20043 */
/* { dg-do compile } */
/* { dg-options "-fpermissive -std=gnu99" } */

extern void f0 (int *);
extern void f0 (int *__restrict);

extern void f1 (int *__restrict);
extern void f1 (int *);

typedef union { int *i; long *l; } U2
  __attribute__((transparent_union));
extern void f2 (U2);
extern void f2 (int *);

typedef union { int *__restrict i; long *__restrict l; } U3
  __attribute__((transparent_union));
extern void f3 (U3);
extern void f3 (int *__restrict);

extern void f4 (U3);
extern void f4 (int *);

extern void f5 (U2);
extern void f5 (int *__restrict);

typedef union { long *l; int *i; } U6
  __attribute__((transparent_union));
extern void f6 (U6);
extern void f6 (int *);

typedef union { long *__restrict l; int *__restrict i; } U7
  __attribute__((transparent_union));
extern void f7 (U7);
extern void f7 (int *__restrict);

extern void f8 (U7);
extern void f8 (int *);

extern void f9 (U6);
extern void f9 (int *__restrict);

extern void f10 (U2);
extern void f11 (U3);
extern void f12 (U6);
extern void f13 (U7);

int i;
long l;

int
main (void)
{
  f0 (&i);
  f0 (&l);	/* { dg-warning "passing argument 1 of 'f0' from incompatible pointer type" } */
  f1 (&i);
  f1 (&l);	/* { dg-warning "passing argument 1 of 'f1' from incompatible pointer type" } */
  f2 (&i);
  f2 (&l);	/* { dg-warning "passing argument 1 of 'f2' from incompatible pointer type" } */
  f3 (&i);
  f3 (&l);	/* { dg-warning "passing argument 1 of 'f3' from incompatible pointer type" } */
  f4 (&i);
  f4 (&l);	/* { dg-warning "passing argument 1 of 'f4' from incompatible pointer type" } */
  f5 (&i);
  f5 (&l);	/* { dg-warning "passing argument 1 of 'f5' from incompatible pointer type" } */
  f6 (&i);
  f6 (&l);	/* { dg-warning "passing argument 1 of 'f6' from incompatible pointer type" } */
  f7 (&i);
  f7 (&l);	/* { dg-warning "passing argument 1 of 'f7' from incompatible pointer type" } */
  f8 (&i);
  f8 (&l);	/* { dg-warning "passing argument 1 of 'f8' from incompatible pointer type" } */
  f9 (&i);
  f9 (&l);	/* { dg-warning "passing argument 1 of 'f9' from incompatible pointer type" } */
  f10 (&i);
  f10 (&l);
  f11 (&i);
  f11 (&l);
  f12 (&i);
  f12 (&l);
  f13 (&i);
  f13 (&l);
  return 0;
}

/* { dg-message "note: expected '\[^\n'\]*' but argument is of type '\[^\n'\]*'" "note: expected" { target *-*-* } 0 } */

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 i: ptr<i32>;
// DEFAULT-NEXT:         field1 l: ptr<i64>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U2:[0-9]+]] U2 = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 i: ptr<i32>;
// DEFAULT-NEXT:         field1 l: ptr<i64>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U3:[0-9]+]] U3 = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 l: ptr<i64>;
// DEFAULT-NEXT:         field1 i: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U6:[0-9]+]] U6 = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 l: ptr<i64>;
// DEFAULT-NEXT:         field1 i: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U7:[0-9]+]] U7 = @type[[TYPE3]];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f0:[0-9]+]] @f0(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i32> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE2:[0-9]+]] <unnamed>: @type[[TYPE0]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE3:[0-9]+]] <unnamed>: @type[[TYPE1]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE4:[0-9]+]] <unnamed>: @type[[TYPE1]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE5:[0-9]+]] <unnamed>: @type[[TYPE0]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE6:[0-9]+]] <unnamed>: @type[[TYPE2]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE7:[0-9]+]] <unnamed>: @type[[TYPE3]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE8:[0-9]+]] <unnamed>: @type[[TYPE3]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE9:[0-9]+]] <unnamed>: @type[[TYPE2]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE10:[0-9]+]] <unnamed>: @type[[TYPE0]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE11:[0-9]+]] <unnamed>: @type[[TYPE1]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE12:[0-9]+]] <unnamed>: @type[[TYPE2]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f13:[0-9]+]] @f13(%[[VALUE13:[0-9]+]] <unnamed>: @type[[TYPE3]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_f0]], addr_of<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_f0]], pointer_cast<ptr<i32>, reason=arg>(addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_f1]], addr_of<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_f1]], pointer_cast<ptr<i32>, reason=arg>(addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE0]]) -> void>(%[[VALUE_f2]], aggregate<@type[[TYPE0]], zero_fill=false>(field0 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE0]]) -> void>(%[[VALUE_f2]], aggregate<@type[[TYPE0]], zero_fill=false>(field1 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE1]]) -> void>(%[[VALUE_f3]], aggregate<@type[[TYPE1]], zero_fill=false>(field0 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE1]]) -> void>(%[[VALUE_f3]], aggregate<@type[[TYPE1]], zero_fill=false>(field1 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE1]]) -> void>(%[[VALUE_f4]], aggregate<@type[[TYPE1]], zero_fill=false>(field0 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE1]]) -> void>(%[[VALUE_f4]], aggregate<@type[[TYPE1]], zero_fill=false>(field1 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE0]]) -> void>(%[[VALUE_f5]], aggregate<@type[[TYPE0]], zero_fill=false>(field0 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE0]]) -> void>(%[[VALUE_f5]], aggregate<@type[[TYPE0]], zero_fill=false>(field1 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE2]]) -> void>(%[[VALUE_f6]], aggregate<@type[[TYPE2]], zero_fill=false>(field1 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE2]]) -> void>(%[[VALUE_f6]], aggregate<@type[[TYPE2]], zero_fill=false>(field0 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE3]]) -> void>(%[[VALUE_f7]], aggregate<@type[[TYPE3]], zero_fill=false>(field1 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE3]]) -> void>(%[[VALUE_f7]], aggregate<@type[[TYPE3]], zero_fill=false>(field0 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE3]]) -> void>(%[[VALUE_f8]], aggregate<@type[[TYPE3]], zero_fill=false>(field1 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE3]]) -> void>(%[[VALUE_f8]], aggregate<@type[[TYPE3]], zero_fill=false>(field0 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE2]]) -> void>(%[[VALUE_f9]], aggregate<@type[[TYPE2]], zero_fill=false>(field1 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE2]]) -> void>(%[[VALUE_f9]], aggregate<@type[[TYPE2]], zero_fill=false>(field0 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE0]]) -> void>(%[[VALUE_f10]], aggregate<@type[[TYPE0]], zero_fill=false>(field0 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE0]]) -> void>(%[[VALUE_f10]], aggregate<@type[[TYPE0]], zero_fill=false>(field1 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE1]]) -> void>(%[[VALUE_f11]], aggregate<@type[[TYPE1]], zero_fill=false>(field0 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE1]]) -> void>(%[[VALUE_f11]], aggregate<@type[[TYPE1]], zero_fill=false>(field1 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE2]]) -> void>(%[[VALUE_f12]], aggregate<@type[[TYPE2]], zero_fill=false>(field1 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE2]]) -> void>(%[[VALUE_f12]], aggregate<@type[[TYPE2]], zero_fill=false>(field0 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE3]]) -> void>(%[[VALUE_f13]], aggregate<@type[[TYPE3]], zero_fill=false>(field1 = addr_of<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE3]]) -> void>(%[[VALUE_f13]], aggregate<@type[[TYPE3]], zero_fill=false>(field0 = addr_of<ptr<i64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
