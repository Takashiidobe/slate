/* PR optimization/10312 */
/* Originator: Peter van Hoof <p dot van-hoof at qub dot ac dot uk> */

/* Verify that the strength reduction pass doesn't find
   illegitimate givs.  */

struct {
  double a;
  int    n[2];
} g = {0., {1, 2}};

int k = 0;

void b(int *j) {}

int main() {
  int j;

  for (j = 0; j < 2; j++)
    k = (k > g.n[j]) ? k : g.n[j];

  k++;
  b(&j);

  return 0;
}


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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 n: array<i32, 2>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %1 g: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<f64>(0.0), field1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %2 k: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %3 @b(%4 j: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%9));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%2, conditional<i32>(gt<i32>(read<i32>(%2), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%1)), read<i32>(%6))))), read<i32>(%2), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%1)), read<i32>(%6))))));
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%11));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%3, addr_of<ptr<i32>>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
