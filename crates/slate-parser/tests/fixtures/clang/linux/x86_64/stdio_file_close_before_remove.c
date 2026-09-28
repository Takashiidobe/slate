#include <stdio.h>

int main(void) {
  FILE *f = fopen("slate_stdio_close_before_remove.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("owned\n", f);
  fclose(f);
  remove("slate_stdio_close_before_remove.tmp");
  puts("done");
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
// DEFAULT-NEXT:     global %30 .str30: array<i8, 36> [storage=static] = code_units<array<i8, 36>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 99, 108, 111, 115, 101, 95, 98, 101, 102, 111, 114, 101, 95, 114, 101, 109, 111, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([111, 119, 110, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 36> [storage=static] = code_units<array<i8, 36>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 99, 108, 111, 115, 101, 95, 98, 101, 102, 111, 114, 101, 95, 114, 101, 109, 111, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([100, 111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %10 @remove(%23 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @fclose(%24 __stream: ptr<@type3>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @fopen(%25 __filename: ptr<const i8> [restrict], %26 __modes: ptr<const i8> [restrict]) -> ptr<@type3> [linkage=external];
// DEFAULT-NEXT:     fn %18 @fputs(%27 __s: ptr<const i8> [restrict], %28 __stream: ptr<@type3> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @puts(%29 __s: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %22 f: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%15, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(36)>(%30)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%31)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type3>>(read<ptr<@type3>>(%22), null<ptr<@type3>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%20, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%32)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%18, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%33)), read<ptr<@type3>>(%22));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%12, read<ptr<@type3>>(%22));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%10, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(36)>(%34)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%20, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%35)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
