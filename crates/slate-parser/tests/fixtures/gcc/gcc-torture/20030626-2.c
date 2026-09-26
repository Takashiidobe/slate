char buf[40];

extern int  sprintf(char *, const char *, ...);
extern void abort(void);

int main() {
  int i = 0;
  int l = sprintf(buf, "%s", i++ ? "string" : "other string");
  if (l != sizeof("other string") - 1 || i != 1)
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
// DEFAULT-NEXT:     global %0 buf: array<i8, 40> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([115, 116, 114, 105, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([111, 116, 104, 101, 114, 32, 115, 116, 114, 105, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @sprintf(%6 <unnamed>: ptr<i8>, %7 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %5 l: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%12));
// DEFAULT-NEXT:         write<i32>(%5, call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%1, array_decay<ptr<i8>, length=Some(40)>(%0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%8)), conditional<ptr<i8>>(ne<i32>(read<i32>(%11), const<i32>(0)), array_decay<ptr<i8>, length=Some(7)>(%9), array_decay<ptr<i8>, length=Some(13)>(%10))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%5))), sub<u64, overflow=wrap>(const<u64>(13), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), ne<i32>(read<i32>(%4), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
