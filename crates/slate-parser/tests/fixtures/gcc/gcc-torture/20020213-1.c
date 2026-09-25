/* PR c/5681
   This testcase failed on IA-32 at -O0, because safe_from_p
   incorrectly assumed it is safe to first write into a.a2 b-1
   and then read the original value from it.  */

void abort(void);
int  bar(float);

struct A {
  float a1;
  int   a2;
} a;

int b;

void foo(void) {
  a.a2 = bar(a.a1);
  a.a2 = a.a2 < b - 1 ? a.a2 : b - 1;
  if (a.a2 >= b - 1)
    abort();
}

int bar(float x) { return 2241; }

int main() {
  a.a1 = 1.0f;
  b    = 3384;
  foo();
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a1: f32;
// DEFAULT-NEXT:         field1 a2: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %3 a: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar(%6 x: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(2241);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(field1(%3), call<i32, signature=fn(f32) -> i32>(%1, read<f32>(field0(%3))));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%1, read<f32>(field0(%3)));
// DEFAULT-NEXT:         write<i32>(field1(%3), conditional<i32>(lt<i32>(read<i32>(field1(%3)), sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1))), read<i32>(field1(%3)), sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1))));
// DEFAULT-NEXT:         if ge<i32>(read<i32>(field1(%3)), sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f32>(field0(%3), const<f32>(1.0));
// DEFAULT-NEXT:         write<i32>(%4, const<i32>(3384));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
