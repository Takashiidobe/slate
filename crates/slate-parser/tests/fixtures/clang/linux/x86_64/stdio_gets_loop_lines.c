#include <stdio.h>

int main(void) {
  remove("slate_stdio_gets_loop_lines.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_lines.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("first\n", f);
  fputs("second\n", f);
  fputs("third", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_lines.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[64];
  while (fgets(line, sizeof line, g) != NULL) {
    fputs(line, stdout);
  }
  fclose(g);
  remove("slate_stdio_gets_loop_lines.tmp");
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
// DEFAULT-NEXT:     extern %9 stdout: ptr<@type3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 32> [storage=static] = code_units<array<i8, 32>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 108, 105, 110, 101, 115, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 32> [storage=static] = code_units<array<i8, 32>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 108, 105, 110, 101, 115, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([102, 105, 114, 115, 116, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([115, 101, 99, 111, 110, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([116, 104, 105, 114, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 32> [storage=static] = code_units<array<i8, 32>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 108, 105, 110, 101, 115, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([114, 101, 111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 32> [storage=static] = code_units<array<i8, 32>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 108, 105, 110, 101, 115, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %10 @remove(%20 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @fclose(%21 __stream: ptr<@type3>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @fopen(%22 __filename: ptr<const i8> [restrict], %23 __modes: ptr<const i8> [restrict]) -> ptr<@type3> [linkage=external];
// DEFAULT-NEXT:     fn %13 @fgets(%24 __s: ptr<i8> [restrict], %25 __n: i32, %26 __stream: ptr<@type3> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %14 @fputs(%27 __s: ptr<const i8> [restrict], %28 __stream: ptr<@type3> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @puts(%29 __s: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%10, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%30)));
// DEFAULT-NEXT:         let %17 f: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%12, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%31)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%32)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type3>>(read<ptr<@type3>>(%17), null<ptr<@type3>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%15, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%33)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%34)), read<ptr<@type3>>(%17));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%35)), read<ptr<@type3>>(%17));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%36)), read<ptr<@type3>>(%17));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%11, read<ptr<@type3>>(%17));
// DEFAULT-NEXT:         let %18 g: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%12, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%37)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%38)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type3>>(read<ptr<@type3>>(%18), null<ptr<@type3>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%15, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%39)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %19 line: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         while %40 ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, i32, ptr<@type3>) -> ptr<i8>>(%13, array_decay<ptr<i8>, length=Some(64)>(%19), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(64))), read<ptr<@type3>>(%18)), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%19)), read<ptr<@type3>>(%9));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%11, read<ptr<@type3>>(%18));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%10, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%41)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
