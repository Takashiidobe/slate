/* PR middle-end/52979 */

/* { dg-require-effective-target int32plus } */

extern void abort(void);
int         c, d, e;

void foo(void) {}

struct __attribute__((packed)) S {
  int g : 31;
  int h : 6;
};
struct S        a = {1};
static struct S b = {1};

void bar(void) {
  a.h        = 1;
  struct S f = {};
  b          = f;
  e          = 0;
  if (d)
    c = a.g;
}

void baz(void) {
  bar();
  a = b;
}

int main() {
  baz();
  if (a.g)
    abort();
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 g: i32 : 31;
// DEFAULT-NEXT:         field1 h: i32 : 6;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 3], bit_offsets=[Some(0), Some(31)], bit_units=[(0, 5)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %1 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 a: @type0 [storage=static] = aggregate<@type0, zero_fill=true>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %7 b: @type0 [storage=static] = aggregate<@type0, zero_fill=true>(field0 = const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..5, bits=31..37>(%6), const<i32>(1));
// DEFAULT-NEXT:         let %9 f: @type0 [storage=automatic] = aggregate<@type0, zero_fill=true>();
// DEFAULT-NEXT:         write<@type0>(%7, copy<@type0, reason=assign>(read<@type0>(%9)));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%1, read<i32>(bitfield0<unit=0, bytes=0..5, bits=0..31>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<@type0>(%6, copy<@type0, reason=assign>(read<@type0>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(bitfield0<unit=0, bytes=0..5, bits=0..31>(%6)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
