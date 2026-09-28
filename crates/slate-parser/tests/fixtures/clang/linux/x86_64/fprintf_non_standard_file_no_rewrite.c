#include <stdio.h>

int main(void) {
  FILE *f = fopen("slate_fprintf_non_standard.tmp", "w");
  if (f == NULL) {
    return 1;
  }
  fprintf(f, "value: %d\n", 7);
  fclose(f);
  remove("slate_fprintf_non_standard.tmp");
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
// DEFAULT-NEXT:     global %27 .str27: array<i8, 31> [storage=static] = code_units<array<i8, 31>>([115, 108, 97, 116, 101, 95, 102, 112, 114, 105, 110, 116, 102, 95, 110, 111, 110, 95, 115, 116, 97, 110, 100, 97, 114, 100, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([118, 97, 108, 117, 101, 58, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 31> [storage=static] = code_units<array<i8, 31>>([115, 108, 97, 116, 101, 95, 102, 112, 114, 105, 110, 116, 102, 95, 110, 111, 110, 95, 115, 116, 97, 110, 100, 97, 114, 100, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %10 @remove(%21 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @fclose(%22 __stream: ptr<@type3>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @fopen(%23 __filename: ptr<const i8> [restrict], %24 __modes: ptr<const i8> [restrict]) -> ptr<@type3> [linkage=external];
// DEFAULT-NEXT:     fn %18 @fprintf(%25 __stream: ptr<@type3> [restrict], %26 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %20 f: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%15, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%27)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%28)));
// DEFAULT-NEXT:         if eq<ptr<@type3>>(read<ptr<@type3>>(%20), null<ptr<@type3>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%18, read<ptr<@type3>>(%20), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%29)), const<i32>(7));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%12, read<ptr<@type3>>(%20));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%10, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%30)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
