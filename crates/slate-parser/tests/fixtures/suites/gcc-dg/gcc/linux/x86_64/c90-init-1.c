/* Test for C99 designated initializers */
/* Origin: Jakub Jelinek <jakub@redhat.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

struct A {
  int B;
  short C[2];
};
int a[10] = { 10, [4] = 15 };			/* { dg-error "ISO (C89|C90) forbids specifying subobject to initialize" } */
struct A b = { .B = 2 };			/* { dg-error "ISO (C89|C90) forbids specifying subobject to initialize" } */
struct A c[] = { [3].C[1] = 1 };		/* { dg-error "ISO (C89|C90) forbids specifying subobject to initialize" } */
struct A d[] = { [4 ... 6].C[0 ... 1] = 2 };	/* { dg-error "(forbids specifying range of elements to initialize)|(ISO (C89|C90) forbids specifying subobject to initialize)" } */
int e[] = { [2] 2 };				/* { dg-error "use of designated initializer without" } */
struct A f = { C: { 0, 1 } };			/* { dg-error "use of designated initializer with " } */
int g;

void foo (int *);

void bar (void)
{
  int x[] = { g++, 2 };				/* { dg-error "is not computable at load time" } */

  foo (x);
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 B: i32;
// DEFAULT-NEXT:         field1 C: array<i16, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 10> [storage=static] [align=16] = aggregate<array<i32, 10>, zero_fill=true>(index0 = const<i32>(10), index4 = const<i32>(15)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE_A]] [storage=static] = aggregate<@type[[TYPE_A]], zero_fill=true>(field0 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: array<@type[[TYPE_A]], 4> [storage=static] [align=16] = aggregate<array<@type[[TYPE_A]], 4>, zero_fill=true>(index3 = aggregate<@type[[TYPE_A]], zero_fill=true>(field1 = aggregate<array<i16, 2>, zero_fill=true>(index1 = truncate<i16, reason=assign, fits=always>(const<i32>(1))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: array<@type[[TYPE_A]], 7> [storage=static] [align=16] = aggregate<array<@type[[TYPE_A]], 7>, zero_fill=true>(index4..=5 = aggregate<@type[[TYPE_A]], zero_fill=true>(field1 = aggregate<array<i16, 2>, zero_fill=false>(index0..=1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)))), index6 = aggregate<@type[[TYPE_A]], zero_fill=true>(field1 = aggregate<array<i16, 2>, zero_fill=false>(index0..=1 = truncate<i16, reason=assign, fits=always>(const<i32>(2))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=true>(index2 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: @type[[TYPE_A]] [storage=static] = aggregate<@type[[TYPE_A]], zero_fill=true>(field1 = aggregate<array<i16, 2>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: array<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_g]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_g]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         write<array<i32, 2>>(%[[VALUE_x]], aggregate<array<i32, 2>, zero_fill=false>(index0 = read<i32>(%[[VALUE1]]), index1 = const<i32>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_foo]], array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
