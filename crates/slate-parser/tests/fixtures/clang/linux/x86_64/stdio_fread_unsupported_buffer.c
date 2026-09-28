#include <stdio.h>
#include <stdlib.h>

int main(void) {
  remove("slate_stdio_fread_unsupported_buffer.tmp");
  FILE *f = fopen("slate_stdio_fread_unsupported_buffer.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("heap-owned", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_fread_unsupported_buffer.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char *buf = malloc(16);
  buf[0]    = 0;
  size_t n  = fread(buf, 1, 10, g);
  buf[n]    = 0;
  printf("%zu %s\n", n, buf);
  free(buf);
  fclose(g);
  remove("slate_stdio_fread_unsupported_buffer.tmp");
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
// DEFAULT-NEXT:     global %52 .str52: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 95, 98, 117, 102, 102, 101, 114, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %53 .str53: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 95, 98, 117, 102, 102, 101, 114, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %55 .str55: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([104, 101, 97, 112, 45, 111, 119, 110, 101, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %57 .str57: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 95, 98, 117, 102, 102, 101, 114, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([114, 101, 111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %60 .str60: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 122, 117, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %61 .str61: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 114, 101, 97, 100, 95, 117, 110, 115, 117, 112, 112, 111, 114, 116, 101, 100, 95, 98, 117, 102, 102, 101, 114, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %11 @remove(%38 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @fclose(%39 __stream: ptr<@type4>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @fopen(%40 __filename: ptr<const i8> [restrict], %41 __modes: ptr<const i8> [restrict]) -> ptr<@type4> [linkage=external];
// DEFAULT-NEXT:     fn %18 @printf(%42 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @fputs(%43 __s: ptr<const i8> [restrict], %44 __stream: ptr<@type4> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @puts(%45 __s: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %28 @fread(%46 __ptr: ptr<void> [restrict], %47 __size: u64, %48 __n: u64, %49 __stream: ptr<@type4> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %30 @malloc(%50 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %32 @free(%51 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %33 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%52)));
// DEFAULT-NEXT:         let %34 f: ptr<@type4> [storage=automatic] = call<ptr<@type4>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type4>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%53)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%54)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type4>>(read<ptr<@type4>>(%34), null<ptr<@type4>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%55)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type4>) -> i32>(%21, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%56)), read<ptr<@type4>>(%34));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type4>) -> i32>(%13, read<ptr<@type4>>(%34));
// DEFAULT-NEXT:         let %35 g: ptr<@type4> [storage=automatic] = call<ptr<@type4>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type4>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%57)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%58)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type4>>(read<ptr<@type4>>(%35), null<ptr<@type4>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%59)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %36 buf: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%30, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%36), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %37 n: u64 [storage=automatic] = call<u64, signature=fn(ptr<void>, u64, u64, ptr<@type4>) -> u64>(%28, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%36)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))), read<ptr<@type4>>(%35));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%36), read<u64>(%37))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%18, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%60)), read<u64>(%37), read<ptr<i8>>(%36));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%32, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%36)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type4>) -> i32>(%13, read<ptr<@type4>>(%35));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%61)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
