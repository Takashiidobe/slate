void abort(void);
void exit(int);

typedef enum foo E;
enum foo { e0, e1 };

struct {
  E eval;
} s;

void p(void) { abort(); }

void f(void) {
  switch (s.eval) {
  case e0:
    p();
  }
}

int main(void) {
  s.eval = e1;
  f();
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
// DEFAULT-NEXT:     type @type0 foo = enum incomplete;
// DEFAULT-NEXT:     type @type1 E = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 eval: @type0;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %7 s: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @p() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %12 enum_to_int<u32, reason=promotion>(read<@type0>(field0(%7)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %12 const<u32>(0):
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<@type0>(field0(%7), int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
