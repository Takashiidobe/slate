#include <stdio.h>

int main(void) {
  char name[] = "slate_stdio_file_ownership.tmp";
  remove(name);
  FILE *f = fopen(name, "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("owned\n", f);
  fclose(f);

  FILE *g = fopen(name, "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char buf[16] = {0};
  fgets(buf, sizeof buf, g);
  fclose(g);
  fputs(buf, stdout);
  remove(name);
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
// DEFAULT-NEXT:     global %41 .str41: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([111, 119, 110, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([114, 101, 111, 112, 101, 110, 45, 102, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %11 @remove(%31 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @fclose(%32 __stream: ptr<@type3>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @fopen(%33 __filename: ptr<const i8> [restrict], %34 __modes: ptr<const i8> [restrict]) -> ptr<@type3> [linkage=external];
// DEFAULT-NEXT:     fn %20 @fgets(%35 __s: ptr<i8> [restrict], %36 __n: i32, %37 __stream: ptr<@type3> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %23 @fputs(%38 __s: ptr<const i8> [restrict], %39 __stream: ptr<@type3> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @puts(%40 __s: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %27 name: array<i8, 31> [storage=automatic] [align=16] = code_units<array<i8, 31>>([115, 108, 97, 116, 101, 95, 115, 116, 100, 105, 111, 95, 102, 105, 108, 101, 95, 111, 119, 110, 101, 114, 115, 104, 105, 112, 46, 116, 109, 112, 0]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%27)));
// DEFAULT-NEXT:         let %28 f: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%27)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%41)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type3>>(read<ptr<@type3>>(%28), null<ptr<@type3>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%42)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%43)), read<ptr<@type3>>(%28));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%13, read<ptr<@type3>>(%28));
// DEFAULT-NEXT:         let %29 g: ptr<@type3> [storage=automatic] = call<ptr<@type3>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type3>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%27)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%44)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type3>>(read<ptr<@type3>>(%29), null<ptr<@type3>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%45)));
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %30 buf: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, i32, ptr<@type3>) -> ptr<i8>>(%20, array_decay<ptr<i8>, length=Some(16)>(%30), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(16))), read<ptr<@type3>>(%29));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%13, read<ptr<@type3>>(%29));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<@type3>) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%30)), read<ptr<@type3>>(%9));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%27)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
