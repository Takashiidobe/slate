#include <stdio.h>

int main(void) {
  remove("slate_stdio_fread_multi_byte.tmp");
  FILE *f = fopen("slate_stdio_fread_multi_byte.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("abcdefghijkl", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_fread_multi_byte.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char   buf[16] = {0};
  size_t n       = fread(buf, 4, 3, g);
  printf("%zu %s\n", n, buf);
  fclose(g);
  remove("slate_stdio_fread_multi_byte.tmp");
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
// DEFAULT-NEXT:     global %46 .str46: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 109, 117, 108, 116, 105, 95, 98, 121, 116, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 109, 117, 108, 116, 105, 95, 98, 121, 116, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %49 .str49: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 109, 117, 108, 116, 105, 95, 98, 121, 116, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([114, 101, 111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 122, 117, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 109, 117, 108, 116, 105, 95, 98, 121, 116, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %11 @remove(%34 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @fclose(%35 __stream: ptr<@type4>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @fopen(%36 __filename: ptr<const i8> [restrict], %37 __modes: ptr<const i8> [restrict]) -> ptr<@type4> [linkage=external];
// DEFAULT-NEXT:     fn %18 @printf(%38 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @fputs(%39 __s: ptr<const i8> [restrict], %40 __stream: ptr<@type4> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @puts(%41 __s: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %28 @fread(%42 __ptr: ptr<void> [restrict], %43 __size: u64, %44 __n: u64, %45 __stream: ptr<@type4> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %29 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%46)));
// DEFAULT-NEXT:         let %30 f: ptr<@type4> [storage=automatic] = call<ptr<@type4>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type4>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%47)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%48)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type4>>(read<ptr<@type4>>(%30), null<ptr<@type4>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%49)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type4>) -> i32>(%21, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%50)), read<ptr<@type4>>(%30));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type4>) -> i32>(%13, read<ptr<@type4>>(%30));
// DEFAULT-NEXT:         let %31 g: ptr<@type4> [storage=automatic] = call<ptr<@type4>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type4>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%51)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%52)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type4>>(read<ptr<@type4>>(%31), null<ptr<@type4>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%53)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %32 buf: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %33 n: u64 [storage=automatic] = call<u64, signature=fn(ptr<void>, u64, u64, ptr<@type4>) -> u64>(%28, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%32)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), read<ptr<@type4>>(%31));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%18, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%54)), read<u64>(%33), array_decay<ptr<i8>, length=Some(16)>(%32));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type4>) -> i32>(%13, read<ptr<@type4>>(%31));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%55)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
