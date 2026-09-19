// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct s3 { char a[3]; };
struct s3 g;
double d;
enum { K = 1 };

int by_size[sizeof(g)];
int by_align[__alignof__(d)];
_Static_assert(sizeof(g) == 3, "");
_Static_assert(__alignof__(d) == 8, "");

int f(int p) {
  char K[5];
  long by_param[sizeof p];
  int shadowed[sizeof K];
  return sizeof by_param + sizeof shadowed;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 s3 = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type1 = enum : u32 {
// IR-NEXT:         %0 K = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %1 g: @type0 [storage=static] [linkage=external];
// IR-NEXT:     global %2 d: f64 [storage=static] [linkage=external];
// IR-NEXT:     global %5 by_size: array<i32, 3> [storage=static] [linkage=external];
// IR-NEXT:     global %6 by_align: array<i32, 8> [storage=static] [linkage=external];
// IR-NEXT:     fn %7 @f(%8 p: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %9 K: array<i8, 5> [storage=automatic];
// IR-NEXT:         let %10 by_param: array<i64, 4> [storage=automatic];
// IR-NEXT:         let %11 shadowed: array<i32, 5> [storage=automatic];
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(32), const<u64>(20))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
