void abort(void);
void exit(int);

static int which_alternative = 3;

static const char *i960_output_ldconst(void);

static const char *output_25(void) {
  switch (which_alternative) {
  case 0:
    return "mov	%1,%0";
  case 1:
    return i960_output_ldconst();
  case 2:
    return "ld	%1,%0";
  case 3:
    return "st	%1,%0";
  }
}

static const char *i960_output_ldconst(void) { return "foo"; }
int                main(void) {
  const char *s = output_25();
  if (s[0] != 's')
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
// DEFAULT-NEXT:     global %2 which_alternative: i32 [storage=static] = const<i32>(3) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([109, 111, 118, 9, 37, 49, 44, 37, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([108, 100, 9, 37, 49, 44, 37, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 116, 9, 37, 49, 44, 37, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @i960_output_ldconst() -> ptr<const i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(4)>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @output_25() -> ptr<const i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %8 read<i32>(%2)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %8 const<i32>(0):
// DEFAULT-NEXT:                     return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(10)>(%9));
// DEFAULT-NEXT:                 case %8 const<i32>(1):
// DEFAULT-NEXT:                     return call<ptr<const i8>, signature=fn() -> ptr<const i8>>(%3);
// DEFAULT-NEXT:                 case %8 const<i32>(2):
// DEFAULT-NEXT:                     return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(9)>(%10));
// DEFAULT-NEXT:                 case %8 const<i32>(3):
// DEFAULT-NEXT:                     return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(9)>(%11));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 s: ptr<const i8> [storage=automatic] = call<ptr<const i8>, signature=fn() -> ptr<const i8>>(%4);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%6), const<i32>(0))))), const<i32>(115))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
