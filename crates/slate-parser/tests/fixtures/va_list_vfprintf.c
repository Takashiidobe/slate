#include <stdarg.h>
#include <stdio.h>

static void print_values(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
}

int main(void) {
  print_values("%d %s\n", 42, "forwarded");
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 __uint64_t = u64;
// DEFAULT-NEXT:     type @type3 __off_t = i64;
// DEFAULT-NEXT:     type @type4 __off64_t = i64;
// DEFAULT-NEXT:     type @type5 _IO_FILE = struct incomplete;
// DEFAULT-NEXT:     type @type6 FILE = @type5;
// DEFAULT-NEXT:     type @type7 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type8 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type9 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type10 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type11 va_list = va_list;
// DEFAULT-NEXT:     extern %11 stdout: ptr<@type5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([102, 111, 114, 119, 97, 114, 100, 101, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %12 @vfprintf(%17 __s: ptr<@type5> [restrict], %18 __format: ptr<const i8> [restrict], %19 __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @print_values(%14 format: ptr<const i8>, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 args: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%15);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type5>, ptr<const i8>, va_list) -> i32>(%12, read<ptr<@type5>>(%11), read<ptr<const i8>>(%14), read<va_list>(%15));
// DEFAULT-NEXT:         va_end(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ...) -> void>(%13, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%20)), const<i32>(42), array_decay<ptr<i8>, length=Some(10)>(%21));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
