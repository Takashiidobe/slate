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
// DEFAULT-NEXT:     global %[[VALUE_which_alternative:[0-9]+]] which_alternative: i32 [storage=static] = const<i32>(3) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([109, 111, 118, 9, 37, 49, 44, 37, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([108, 100, 9, 37, 49, 44, 37, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 116, 9, 37, 49, 44, 37, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_i960_output_ldconst:[0-9]+]] @i960_output_ldconst() -> ptr<const i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_output_25:[0-9]+]] @output_25() -> ptr<const i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_which_alternative]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(0):
// DEFAULT-NEXT:                     return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]]));
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(1):
// DEFAULT-NEXT:                     return call<ptr<const i8>, signature=fn() -> ptr<const i8>>(%[[VALUE_i960_output_ldconst]]);
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(2):
// DEFAULT-NEXT:                     return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(3):
// DEFAULT-NEXT:                     return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: ptr<const i8> [storage=automatic] = call<ptr<const i8>, signature=fn() -> ptr<const i8>>(%[[VALUE_output_25]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_s]]), const<i32>(0))))), const<i32>(115))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
