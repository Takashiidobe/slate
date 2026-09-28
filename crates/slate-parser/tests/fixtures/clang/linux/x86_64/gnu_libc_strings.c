#define _GNU_SOURCE
#include <argz.h>
#include <envz.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int gnu_string_extensions(void) {
  char       destination[16] = {};
  const char repeated[]      = "abca";
  char       obscured[]      = "gnu";
  char       tokens[]        = "a::bc";
  char      *cursor          = tokens;
  char      *token;
  char      *end         = mempcpy(destination, "slate", 6);
  int        token_score = 0;

  while ((token = strsep(&cursor, ":")) != NULL) {
    token_score = token_score * 10 + (int)strlen(token);
  }

  memfrob(obscured, 3);
  memfrob(obscured, 3);

  return (int)(end - destination) + (strcmp(destination, "slate") == 0) +
         ((const char *)memrchr(repeated, 'a', 4) - repeated) +
         ((const char *)rawmemchr(repeated, 'c') - repeated) +
         (strchrnul("abc", 'z') - "abc") +
         (strcasestr("GNU Library", "library") != NULL) +
         (strverscmp("release-2", "release-10") < 0) +
         (strcmp(obscured, "gnu") == 0) + token_score +
         (strcmp(strerrorname_np(EINVAL), "EINVAL") == 0) +
         (strerrordesc_np(EINVAL) != NULL);
}

static int gnu_argz_extensions(void) {
  char        *argz   = NULL;
  size_t       length = 0;
  char        *arguments[6];
  unsigned int replacements = 0;
  int          total        = 0;

  total += argz_create_sep("one:two:three", ':', &argz, &length) == 0;
  total += argz_count(argz, length) == 3;
  argz_extract(argz, length, arguments);
  total += strcmp(arguments[1], "two") == 0;
  total += argz_add(&argz, &length, "four") == 0;
  total += argz_replace(&argz, &length, "three", "THREE", &replacements) == 0;
  total += replacements == 1;
  total += argz_count(argz, length) == 4;
  argz_stringify(argz, length, ',');
  total += strcmp(argz, "one,two,THREE,four") == 0;
  free(argz);
  return total;
}

static int gnu_envz_extensions(void) {
  char  *envz   = NULL;
  size_t length = 0;
  int    total  = 0;

  total += envz_add(&envz, &length, "ALPHA", "one") == 0;
  total += envz_add(&envz, &length, "BETA", NULL) == 0;
  total += strcmp(envz_get(envz, length, "ALPHA"), "one") == 0;
  total += envz_entry(envz, length, "BETA") != NULL;
  envz_remove(&envz, &length, "ALPHA");
  total += envz_get(envz, length, "ALPHA") == NULL;
  envz_strip(&envz, &length);
  total += length == 0;
  free(envz);
  return total;
}

int main(void) {
  printf("%d %d %d\n", gnu_string_extensions(), gnu_argz_extensions(),
         gnu_envz_extensions());
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
// DEFAULT-NEXT:     type @type0 error_t = i32;
// DEFAULT-NEXT:     type @type1 size_t = u64;
// DEFAULT-NEXT:     global %107 .str107: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([58, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([71, 78, 85, 32, 76, 105, 98, 114, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([108, 105, 98, 114, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([114, 101, 108, 101, 97, 115, 101, 45, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([114, 101, 108, 101, 97, 115, 101, 45, 49, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 110, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([69, 73, 78, 86, 65, 76, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([111, 110, 101, 58, 116, 119, 111, 58, 116, 104, 114, 101, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([116, 119, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 111, 117, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([116, 104, 114, 101, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([84, 72, 82, 69, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([111, 110, 101, 44, 116, 119, 111, 44, 84, 72, 82, 69, 69, 44, 102, 111, 117, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([66, 69, 84, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([66, 69, 84, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @rawmemchr(%47 __s: ptr<const void>, %48 __c: i32) -> ptr<void> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %3 @memrchr(%49 __s: ptr<const void>, %50 __c: i32, %51 __n: u64) -> ptr<void> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %4 @strcmp(%52 __s1: ptr<const i8>, %53 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %5 @strchrnul(%54 __s: ptr<const i8>, %55 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %6 @strcasestr(%56 __haystack: ptr<const i8>, %57 __needle: ptr<const i8>) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %7 @mempcpy(%58 __dest: ptr<void> [restrict], %59 __src: ptr<const void> [restrict], %60 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %8 @strlen(%61 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %9 @strerrordesc_np(%62 __err: i32) -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %10 @strerrorname_np(%63 __err: i32) -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %11 @strsep(%64 __stringp: ptr<ptr<i8>> [restrict], %65 __delim: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %12 @strverscmp(%66 __s1: ptr<const i8>, %67 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %13 @memfrob(%68 __s: ptr<void>, %69 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %14 @argz_create_sep(%70 __string: ptr<const i8> [restrict], %71 __sep: i32, %72 __argz: ptr<ptr<i8>> [restrict], %73 __len: ptr<u64> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @argz_count(%74 __argz: ptr<const i8>, %75 __len: u64) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %16 @argz_extract(%76 __argz: ptr<const i8> [restrict], %77 __len: u64, %78 __argv: ptr<ptr<i8>> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @argz_stringify(%79 __argz: ptr<i8>, %80 __len: u64, %81 __sep: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %18 @argz_add(%82 __argz: ptr<ptr<i8>> [restrict], %83 __argz_len: ptr<u64> [restrict], %84 __str: ptr<const i8> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @argz_replace(%85 __argz: ptr<ptr<i8>> [restrict], %86 __argz_len: ptr<u64> [restrict], %87 __str: ptr<const i8> [restrict], %88 __with: ptr<const i8> [restrict], %89 __replace_count: ptr<u32> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @envz_entry(%90 __envz: ptr<const i8> [restrict], %91 __envz_len: u64, %92 __name: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %21 @envz_get(%93 __envz: ptr<const i8> [restrict], %94 __envz_len: u64, %95 __name: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %22 @envz_add(%96 __envz: ptr<ptr<i8>> [restrict], %97 __envz_len: ptr<u64> [restrict], %98 __name: ptr<const i8> [restrict], %99 __value: ptr<const i8> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @envz_remove(%100 __envz: ptr<ptr<i8>> [restrict], %101 __envz_len: ptr<u64> [restrict], %102 __name: ptr<const i8> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %24 @envz_strip(%103 __envz: ptr<ptr<i8>> [restrict], %104 __envz_len: ptr<u64> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %25 @printf(%105 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @free(%106 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %27 @gnu_string_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 destination: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %29 repeated: array<i8, 5> [storage=automatic] [const] = code_units<array<i8, 5>>([97, 98, 99, 97, 0]);
// DEFAULT-NEXT:         let %30 obscured: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([103, 110, 117, 0]);
// DEFAULT-NEXT:         let %31 tokens: array<i8, 6> [storage=automatic] = code_units<array<i8, 6>>([97, 58, 58, 98, 99, 0]);
// DEFAULT-NEXT:         let %32 cursor: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(6)>(%31);
// DEFAULT-NEXT:         let %33 token: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %34 end: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%7, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%28)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%107)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         let %35 token_score: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %108 {
// DEFAULT-NEXT:             write<ptr<i8>>(%33, call<ptr<i8>, signature=fn(ptr<ptr<i8>>, ptr<const i8>) -> ptr<i8>>(%11, addr_of<ptr<ptr<i8>>>(%32), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%109))));
// DEFAULT-NEXT:             yield ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<ptr<i8>>, ptr<const i8>) -> ptr<i8>>(%11, addr_of<ptr<ptr<i8>>>(%32), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%109))), null<ptr<i8>>);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%35, add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%35), const<i32>(10)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%8, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%33)))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%30)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%30)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(truncate<i32, reason=explicit, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%34), array_decay<ptr<i8>, length=Some(16)>(%28))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%28)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%110))), const<i32>(0))))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%3, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(5)>(%29)), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))), array_decay<ptr<const i8>, length=Some(5)>(%29))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32) -> ptr<void>>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(5)>(%29)), const<i32>(99))), array_decay<ptr<const i8>, length=Some(5)>(%29))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%111)), const<i32>(122)), array_decay<ptr<i8>, length=Some(4)>(%112))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%6, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%113)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%114))), null<ptr<i8>>)))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(lt<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%12, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%115)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%116))), const<i32>(0))))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%30)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%117))), const<i32>(0))))), widen<i64, reason=usual_arith>(read<i32>(%35))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, call<ptr<const i8>, signature=fn(i32) -> ptr<const i8>>(%10, const<i32>(22)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%118))), const<i32>(0))))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(ne<ptr<const i8>>(call<ptr<const i8>, signature=fn(i32) -> ptr<const i8>>(%9, const<i32>(22)), null<ptr<const i8>>)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @gnu_argz_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %37 argz: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %38 length: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %39 arguments: array<ptr<i8>, 6> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %40 replacements: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %41 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %134: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:         let %135: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%134), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, i32, ptr<ptr<i8>>, ptr<u64>) -> i32>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%119)), const<i32>(58), addr_of<ptr<ptr<i8>>>(%37), addr_of<ptr<u64>>(%38)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%41, read<i32>(%135));
// DEFAULT-NEXT:         let %136: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:         let %137: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%136), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%15, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%37)), read<u64>(%38)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))));
// DEFAULT-NEXT:         write<i32>(%41, read<i32>(%137));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, u64, ptr<ptr<i8>>) -> void>(%16, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%37)), read<u64>(%38), array_decay<ptr<ptr<i8>>, length=Some(6)>(%39));
// DEFAULT-NEXT:         let %138: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:         let %139: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%138), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(6)>(%39), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%120))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%41, read<i32>(%139));
// DEFAULT-NEXT:         let %140: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:         let %141: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%140), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>) -> i32>(%18, addr_of<ptr<ptr<i8>>>(%37), addr_of<ptr<u64>>(%38), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%121))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%41, read<i32>(%141));
// DEFAULT-NEXT:         let %142: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:         let %143: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%142), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>, ptr<const i8>, ptr<u32>) -> i32>(%19, addr_of<ptr<ptr<i8>>>(%37), addr_of<ptr<u64>>(%38), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%122)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%123)), addr_of<ptr<u32>>(%40)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%41, read<i32>(%143));
// DEFAULT-NEXT:         let %144: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:         let %145: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%144), from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%40), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(%41, read<i32>(%145));
// DEFAULT-NEXT:         let %146: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:         let %147: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%146), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%15, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%37)), read<u64>(%38)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:         write<i32>(%41, read<i32>(%147));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, u64, i32) -> void>(%17, read<ptr<i8>>(%37), read<u64>(%38), const<i32>(44));
// DEFAULT-NEXT:         let %148: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:         let %149: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%148), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%37)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%124))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%41, read<i32>(%149));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%26, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%37)));
// DEFAULT-NEXT:         return read<i32>(%41);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @gnu_envz_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %43 envz: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %44 length: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %45 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %150: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %151: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%150), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>, ptr<const i8>) -> i32>(%22, addr_of<ptr<ptr<i8>>>(%43), addr_of<ptr<u64>>(%44), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%125)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%126))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%151));
// DEFAULT-NEXT:         let %152: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %153: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%152), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>, ptr<const i8>) -> i32>(%22, addr_of<ptr<ptr<i8>>>(%43), addr_of<ptr<u64>>(%44), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%127)), null<ptr<const i8>>), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%153));
// DEFAULT-NEXT:         let %154: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %155: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%154), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(call<ptr<i8>, signature=fn(ptr<const i8>, u64, ptr<const i8>) -> ptr<i8>>(%21, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%43)), read<u64>(%44), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%128)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%129))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%155));
// DEFAULT-NEXT:         let %156: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %157: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%156), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, u64, ptr<const i8>) -> ptr<i8>>(%20, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%43)), read<u64>(%44), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%130))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%157));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>) -> void>(%23, addr_of<ptr<ptr<i8>>>(%43), addr_of<ptr<u64>>(%44), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%131)));
// DEFAULT-NEXT:         let %158: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %159: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%158), from_bool<i32, reason=promotion>(eq<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, u64, ptr<const i8>) -> ptr<i8>>(%21, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%43)), read<u64>(%44), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%132))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%159));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> void>(%24, addr_of<ptr<ptr<i8>>>(%43), addr_of<ptr<u64>>(%44));
// DEFAULT-NEXT:         let %160: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %161: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%160), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%44), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%161));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%26, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%43)));
// DEFAULT-NEXT:         return read<i32>(%45);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%133)), call<i32, signature=fn() -> i32>(%27), call<i32, signature=fn() -> i32>(%36), call<i32, signature=fn() -> i32>(%42));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
