void abort(void);
void exit(int);

char *f(char *s, unsigned int i) { return &s[i + 3 - 1]; }

int main(void) {
  char *str = "abcdefghijkl";
  char *x2  = f(str, 12);
  if (str + 14 != x2)
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
// DEFAULT-NEXT:     global %9 .str9: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 s: ptr<i8>, %4 i: u32) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), sub<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 str: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(13)>(%9);
// DEFAULT-NEXT:         let %7 x2: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<i8>, u32) -> ptr<i8>>(%2, read<ptr<i8>>(%6), reinterpret<u32, reason=arg, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:         if ne<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), const<i32>(14)), read<ptr<i8>>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
