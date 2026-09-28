#include <stdio.h>

int main(void) {
  FILE *fp = fopen("slate_perror_fopen_missing.tmp", "r");
  if (fp == NULL) {
    perror("open failed");
    return 1;
  }
  fclose(fp);
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
// DEFAULT-NEXT:     type @type0 __uint64_t = u64;
// DEFAULT-NEXT:     type @type1 __off_t = i64;
// DEFAULT-NEXT:     type @type2 __off64_t = i64;
// DEFAULT-NEXT:     type @type3 _IO_FILE = struct incomplete;
// DEFAULT-NEXT:     type @type4 FILE = @type3;
// DEFAULT-NEXT:     type @type5 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type6 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type7 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type8 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     global %22 .str22: array<i8, 31> [storage=static] = code_units<array<i8, 31>>([115, 108, 97, 116, 101, 95, 112, 101, 114, 114, 111, 114, 95, 102, 111, 112, 101, 110, 95, 109, 105, 115, 115, 105, 110, 103, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([111, 112, 101, 110, 32, 102, 97, 105, 108, 101, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %10 @fclose(%18 __stream: ptr<@type3>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @fopen(%19 __filename: ptr<const i8> [restrict], %20 __modes: ptr<const i8> [restrict]) -> ptr<@type3> [linkage=external];
// DEFAULT-NEXT:     fn %15 @perror(%21 __s: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 fp: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%13, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%22)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%23)));
// DEFAULT-NEXT:         if eq<ptr<@type3>>(read<ptr<@type3>>(%17), null<ptr<@type3>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>) -> void>(%15, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%24)));
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%10, read<ptr<@type3>>(%17));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
