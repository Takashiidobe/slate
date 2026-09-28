#include <stdio.h>

int main(void) {
  remove("slate_stdio_gets_loop_eof.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_eof.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("only\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_eof.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[64];
  while (fgets(line, sizeof line, g) != NULL) {
    fputs(line, stdout);
  }
  fclose(g);
  puts("done");

  FILE *h = fopen("slate_stdio_gets_loop_eof.tmp", "w");
  if (!h) {
    puts("open-fail");
    return 0;
  }
  fclose(h);

  FILE *e = fopen("slate_stdio_gets_loop_eof.tmp", "r");
  if (!e) {
    puts("reopen-fail");
    return 0;
  }
  char empty_line[64];
  while (fgets(empty_line, sizeof empty_line, e) != NULL) {
    fputs(empty_line, stdout);
  }
  fclose(e);
  puts("empty-done");
  remove("slate_stdio_gets_loop_eof.tmp");
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
// DEFAULT-NEXT:     global %43 .str43: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 101, 111, 102, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 101, 111, 102, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %46 .str46: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([111, 110, 108, 121, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 101, 111, 102, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([114, 101, 111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([100, 111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 101, 111, 102, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 101, 111, 102, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([114, 101, 111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %60 .str60: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([101, 109, 112, 116, 121, 45, 100, 111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %61 .str61: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 101, 111, 102, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %11 @remove(%33 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @fclose(%34 __stream: ptr<@type3>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @fopen(%35 __filename: ptr<const i8> [restrict], %36 __modes: ptr<const i8> [restrict]) -> ptr<@type3> [linkage=external];
// DEFAULT-NEXT:     fn %20 @fgets(%37 __s: ptr<i8> [restrict], %38 __n: i32, %39 __stream: ptr<@type3> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %23 @fputs(%40 __s: ptr<const i8> [restrict], %41 __stream: ptr<@type3> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @puts(%42 __s: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%43)));
// DEFAULT-NEXT:         let %27 f: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%44)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%45)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type3>>(read<ptr<@type3>>(%27), null<ptr<@type3>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%46)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%47)), read<ptr<@type3>>(%27));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%13, read<ptr<@type3>>(%27));
// DEFAULT-NEXT:         let %28 g: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%48)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%49)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type3>>(read<ptr<@type3>>(%28), null<ptr<@type3>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%50)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %29 line: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         while %51 ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, i32, ptr<@type3>) -> ptr<i8>>(%20, array_decay<ptr<i8>, length=Some(64)>(%29), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(64))), read<ptr<@type3>>(%28)), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%29)), read<ptr<@type3>>(%9));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%13, read<ptr<@type3>>(%28));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%52)));
// DEFAULT-NEXT:         let %30 h: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%53)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%54)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type3>>(read<ptr<@type3>>(%30), null<ptr<@type3>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%55)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%13, read<ptr<@type3>>(%30));
// DEFAULT-NEXT:         let %31 e: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%56)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%57)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type3>>(read<ptr<@type3>>(%31), null<ptr<@type3>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%58)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %32 empty_line: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         while %59 ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, i32, ptr<@type3>) -> ptr<i8>>(%20, array_decay<ptr<i8>, length=Some(64)>(%32), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(64))), read<ptr<@type3>>(%31)), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%32)), read<ptr<@type3>>(%9));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%13, read<ptr<@type3>>(%31));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%60)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%61)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
