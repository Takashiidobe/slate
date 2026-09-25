extern void exit(int);
extern void abort(void);

struct {
  int a;
  int b[5];
} x;
int *y;

int foo(void) { return y - x.b; }

int main(void) {
  y = x.b;
  if (foo())
    abort();
  exit(0);
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
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: array<i32, 5>;
// DEFAULT-NEXT:     } [size=24, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %3 x: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 y: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<i32>>(%4), array_decay<ptr<i32>, length=Some(5)>(field1(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<i32>>(%4, array_decay<ptr<i32>, length=Some(5)>(field1(%3)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
