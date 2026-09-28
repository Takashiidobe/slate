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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 B: i32;
// DEFAULT-NEXT:         field1 C: array<i16, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %1 a: array<i32, 10> [storage=static] [align=16] = aggregate<array<i32, 10>, zero_fill=true>(index0 = const<i32>(10), index4 = const<i32>(15)) [linkage=external];
// DEFAULT-NEXT:     global %2 b: @type0 [storage=static] = aggregate<@type0, zero_fill=true>(field0 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %3 c: array<@type0, 4> [storage=static] [align=16] = aggregate<array<@type0, 4>, zero_fill=true>(index3 = aggregate<@type0, zero_fill=true>(field1 = aggregate<array<i16, 2>, zero_fill=true>(index1 = truncate<i16, reason=assign, fits=always>(const<i32>(1))))) [linkage=external];
// DEFAULT-NEXT:     global %4 d: array<@type0, 7> [storage=static] [align=16] = aggregate<array<@type0, 7>, zero_fill=true>(index4..=5 = aggregate<@type0, zero_fill=true>(field1 = aggregate<array<i16, 2>, zero_fill=false>(index0..=1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)))), index6 = aggregate<@type0, zero_fill=true>(field1 = aggregate<array<i16, 2>, zero_fill=false>(index0..=1 = truncate<i16, reason=assign, fits=always>(const<i32>(2))))) [linkage=external];
// DEFAULT-NEXT:     global %5 e: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=true>(index2 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %6 f: @type0 [storage=static] = aggregate<@type0, zero_fill=true>(field1 = aggregate<array<i16, 2>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %7 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @foo(%11 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 x: array<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%13));
// DEFAULT-NEXT:         write<array<i32, 2>>(%10, aggregate<array<i32, 2>, zero_fill=false>(index0 = read<i32>(%12), index1 = const<i32>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%8, array_decay<ptr<i32>, length=Some(2)>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
