/* Origin: PR c/128 from Martin Sebor <sebor@roguewave.com>, adapted
   as a testcase by Joseph Myers <jsm28@cam.ac.uk>.
*/
/* Character arrays initialized by a string literal must have
   uninitialized elements zeroed.  This isn't clear in the 1990
   standard, but was fixed in TC2 and C99; see DRs #060, #092.
*/
extern void abort(void);
extern void exit(int);

int foo(void) {
  char s[2] = "";
  return 0 == s[1];
}

char *t;

int main(void) {
  {
    char s[] = "x";
    t        = s;
  }
  if (foo())
    exit(0);
  else
    abort();
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
// DEFAULT-NEXT:     global %4 t: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 s: array<i8, 2> [storage=automatic] = code_units<array<i8, 2>>([0, 0]);
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(const<i32>(0), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%3), const<i32>(1)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %6 s: array<i8, 2> [storage=automatic] = code_units<array<i8, 2>>([120, 0]);
// DEFAULT-NEXT:             write<ptr<i8>>(%4, array_decay<ptr<i8>, length=Some(2)>(%6));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
