#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  char path[] = "slate-XXXXXX";
  int  fd     = mkstemp(path);
  printf("%d\n", fd >= 0);
  if (fd >= 0) {
    close(fd);
    unlink(path);
  }
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mkstemp:[0-9]+]] @mkstemp(%[[VALUE___template:[0-9]+]] __template: ptr<i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_close:[0-9]+]] @close(%[[VALUE___fd:[0-9]+]] __fd: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_unlink:[0-9]+]] @unlink(%[[VALUE___name:[0-9]+]] __name: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_path:[0-9]+]] path: array<i8, 13> [storage=automatic] = code_units<array<i8, 13>>([115, 108, 97, 116, 101, 45, 88, 88, 88, 88, 88, 88, 0]);
// DEFAULT-NEXT:         let %[[VALUE_fd:[0-9]+]] fd: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_mkstemp]], array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_path]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), from_bool<i32, reason=vararg>(ge<i32>(read<i32>(%[[VALUE_fd]]), const<i32>(0))));
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_fd]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(i32) -> i32>(%[[VALUE_close]], read<i32>(%[[VALUE_fd]]));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_unlink]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_path]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
