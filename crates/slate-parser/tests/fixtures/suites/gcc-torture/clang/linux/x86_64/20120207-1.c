/* PR middle-end/51994 */
/* Testcase by Uros Bizjak <ubizjak@gmail.com> */

extern char *strcpy(char *, const char *);
extern void  abort(void);

char __attribute__((noinline)) test(int a) {
  char  buf[16];
  char *output = buf;

  strcpy(&buf[0], "0123456789");

  output += a;
  output -= 1;

  return output[0];
}

int main() {
  if (test(2) != '1')
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_a:[0-9]+]] a: i32) -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<i8, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_output:[0-9]+]] output: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_buf]]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_buf]]), const<i32>(0)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_output]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_output]], read<ptr<i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_output]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_output]], read<ptr<i8>>(%[[VALUE5]]));
// DEFAULT-NEXT:         return read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_output]]), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i32) -> i8>(%[[VALUE_test]], const<i32>(2))), const<i32>(49))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
