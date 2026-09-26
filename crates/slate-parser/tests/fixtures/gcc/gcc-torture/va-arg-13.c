/* derived from mozilla source code */

#include <stdarg.h>

void abort(void);
void exit(int);

typedef struct {
  void   *stream;
  va_list ap;
  int     nChar;
} ScanfState;

void dummy(va_list vap) {
  if (va_arg(vap, int) != 1234)
    abort();
  return;
}

void test(int fmt, ...) {
  ScanfState state, *statep;

  statep = &state;

  va_start(statep->ap, fmt);
  dummy(statep->ap);
  va_end(statep->ap);

  va_start(state.ap, fmt);
  dummy(state.ap);
  va_end(state.ap);

  return;
}

int main(void) {
  test(456, 1234);
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 stream: ptr<void>;
// DEFAULT-NEXT:         field1 ap: va_list;
// DEFAULT-NEXT:         field2 nChar: i32;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 32]];
// DEFAULT-NEXT:     type @type2 ScanfState = @type1;
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%12 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @dummy(%6 vap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%6), const<i32>(1234))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test(%8 fmt: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 state: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %10 statep: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type1>>(%10, addr_of<ptr<@type1>>(%9));
// DEFAULT-NEXT:         va_start(field1(deref(read<ptr<@type1>>(%10))));
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%5, read<va_list>(field1(deref(read<ptr<@type1>>(%10)))));
// DEFAULT-NEXT:         va_end(field1(deref(read<ptr<@type1>>(%10))));
// DEFAULT-NEXT:         va_start(field1(%9));
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%5, read<va_list>(field1(%9)));
// DEFAULT-NEXT:         va_end(field1(%9));
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%7, const<i32>(456), const<i32>(1234));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
