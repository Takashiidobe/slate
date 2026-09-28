#include <stdio.h>
#include <string.h>

int main(void) {
  char copy[16]         = {0};
  char append[16]       = "foo";
  char trunc_copy[16]   = {0};
  char trunc_append[16] = "pre";

  strcpy(copy, "abc");
  strcat(append, "bar");
  strncpy(trunc_copy, "abcdef", 3);
  strncat(trunc_append, "suffix", 3);

  printf("%s %s %s %s %zu\n", copy, append, trunc_copy, trunc_append,
         strlen(trunc_append));
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
// DEFAULT-NEXT:     global %36 .str36: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([115, 117, 102, 102, 105, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([37, 115, 32, 37, 115, 32, 37, 115, 32, 37, 115, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%24 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @strcpy(%25 __dest: ptr<i8> [restrict], %26 __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %9 @strncpy(%27 __dest: ptr<i8> [restrict], %28 __src: ptr<const i8> [restrict], %29 __n: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %12 @strcat(%30 __dest: ptr<i8> [restrict], %31 __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %16 @strncat(%32 __dest: ptr<i8> [restrict], %33 __src: ptr<const i8> [restrict], %34 __n: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %18 @strlen(%35 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %20 copy: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %21 append: array<i8, 16> [storage=automatic] [align=16] = code_units<array<i8, 16>>([102, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         let %22 trunc_copy: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %23 trunc_append: array<i8, 16> [storage=automatic] [align=16] = code_units<array<i8, 16>>([112, 114, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%5, array_decay<ptr<i8>, length=Some(16)>(%20), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%36)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%12, array_decay<ptr<i8>, length=Some(16)>(%21), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%37)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%9, array_decay<ptr<i8>, length=Some(16)>(%22), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%38)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(16)>(%23), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%40)), array_decay<ptr<i8>, length=Some(16)>(%20), array_decay<ptr<i8>, length=Some(16)>(%21), array_decay<ptr<i8>, length=Some(16)>(%22), array_decay<ptr<i8>, length=Some(16)>(%23), call<u64, signature=fn(ptr<const i8>) -> u64>(%18, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%23))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
