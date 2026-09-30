/* Test whether a partly call-clobbered register will be moved over a call.
   Although the original test case didn't use any GNUisms, it proved
   difficult to reduce without the named register extension.  */

void abort(void);
void exit(int);

#if __SH64__ == 32
#define LOC asm("r10")
#else
#define LOC
#endif

unsigned int foo(char *c, unsigned int x, unsigned int y) {
  register unsigned int z LOC;

  __builtin_sprintf(c, "%d", x / y);
  z = x + 1;
  return z / (y + 1);
}

int main() {
  char c[16];

  if (foo(c, ~1U, 4) != (~0U / 5))
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sprintf:[0-9]+]] @__builtin_sprintf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_c:[0-9]+]] c: ptr<i8>, %[[VALUE_x:[0-9]+]] x: u32, %[[VALUE_y:[0-9]+]] y: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: u32 [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_sprintf]], read<ptr<i8>>(%[[VALUE_c]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), div<u32, by_zero=ub>(read<u32>(%[[VALUE_x]]), read<u32>(%[[VALUE_y]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_z]], add<u32, overflow=wrap>(read<u32>(%[[VALUE_x]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         return div<u32, by_zero=ub>(read<u32>(%[[VALUE_z]]), add<u32, overflow=wrap>(read<u32>(%[[VALUE_y]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: array<i8, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(ptr<i8>, u32, u32) -> u32>(%[[VALUE_foo]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_c_2]]), not<u32>(const<u32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(4))), div<u32, by_zero=ub>(not<u32>(const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
