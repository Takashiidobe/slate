#include <stdio.h>
#include <string.h>

int main(void) {
  remove("slate_stdio_gets_loop_unsupported.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_unsupported.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("one\n", f);
  fputs("two\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_unsupported.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[64];
  int  count = 0;
  while (fgets(line, sizeof line, g) != NULL) {
    count += (int)strlen(line);
  }
  fclose(g);
  printf("%d\n", count);
  remove("slate_stdio_gets_loop_unsupported.tmp");
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 __uint64_t = u64;
// DEFAULT-NEXT:     type @type2 __off_t = i64;
// DEFAULT-NEXT:     type @type3 __off64_t = i64;
// DEFAULT-NEXT:     type @type4 _IO_FILE = struct incomplete;
// DEFAULT-NEXT:     type @type5 FILE = @type4;
// DEFAULT-NEXT:     type @type6 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type7 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type8 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type9 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     global %47 .str47: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([111, 110, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 119, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([114, 101, 111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 38> [storage=static] = code_units<array<i8, 38>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 103, 101, 116, 115, 95, 108, 111, 111, 112, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %11 @remove(%35 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @fclose(%36 __stream: ptr<@type4>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @fopen(%37 __filename: ptr<const i8> [restrict], %38 __modes: ptr<const i8> [restrict]) -> ptr<@type4> [linkage=external];
// DEFAULT-NEXT:     fn %18 @printf(%39 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %22 @fgets(%40 __s: ptr<i8> [restrict], %41 __n: i32, %42 __stream: ptr<@type4> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %25 @fputs(%43 __s: ptr<const i8> [restrict], %44 __stream: ptr<@type4> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %27 @puts(%45 __s: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %29 @strlen(%46 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %30 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(38)>(%47)));
// DEFAULT-NEXT:         let %31 f: ptr<@type4> [storage=automatic] = call<ptr<@type4>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type4>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(38)>(%48)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%49)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type4>>(read<ptr<@type4>>(%31), null<ptr<@type4>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%27, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%50)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type4>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%51)), read<ptr<@type4>>(%31));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type4>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%52)), read<ptr<@type4>>(%31));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type4>) -> i32>(%13, read<ptr<@type4>>(%31));
// DEFAULT-NEXT:         let %32 g: ptr<@type4> [storage=automatic] = call<ptr<@type4>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type4>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(38)>(%53)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%54)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type4>>(read<ptr<@type4>>(%32), null<ptr<@type4>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%27, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%55)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %33 line: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %34 count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %56 ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, i32, ptr<@type4>) -> ptr<i8>>(%22, array_decay<ptr<i8>, length=Some(64)>(%33), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(64))), read<ptr<@type4>>(%32)), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %59: i32 [synthetic] = read<i32>(%34);
// DEFAULT-NEXT:                 let %60: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%59), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%29, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%33))))));
// DEFAULT-NEXT:                 write<i32>(%34, read<i32>(%60));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type4>) -> i32>(%13, read<ptr<@type4>>(%32));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%18, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%57)), read<i32>(%34));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(38)>(%58)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
