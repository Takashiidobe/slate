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
// DEFAULT-NEXT:     global %167 .str167: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %169 .str169: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([58, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %170 .str170: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %171 .str171: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %172 .str172: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %173 .str173: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([71, 78, 85, 32, 76, 105, 98, 114, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %174 .str174: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([108, 105, 98, 114, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %175 .str175: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([114, 101, 108, 101, 97, 115, 101, 45, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %176 .str176: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([114, 101, 108, 101, 97, 115, 101, 45, 49, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %177 .str177: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 110, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %178 .str178: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([69, 73, 78, 86, 65, 76, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %179 .str179: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([111, 110, 101, 58, 116, 119, 111, 58, 116, 104, 114, 101, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %180 .str180: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([116, 119, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %181 .str181: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 111, 117, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %182 .str182: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([116, 104, 114, 101, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %183 .str183: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([84, 72, 82, 69, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %184 .str184: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([111, 110, 101, 44, 116, 119, 111, 44, 84, 72, 82, 69, 69, 44, 102, 111, 117, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %185 .str185: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %186 .str186: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %187 .str187: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([66, 69, 84, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %188 .str188: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %189 .str189: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %190 .str190: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([66, 69, 84, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %191 .str191: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %192 .str192: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %193 .str193: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @rawmemchr(%107 __s: ptr<const void>, %108 __c: i32) -> ptr<void> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %8 @memrchr(%109 __s: ptr<const void>, %110 __c: i32, %111 __n: u64) -> ptr<void> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %11 @strcmp(%112 __s1: ptr<const i8>, %113 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %14 @strchrnul(%114 __s: ptr<const i8>, %115 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %17 @strcasestr(%116 __haystack: ptr<const i8>, %117 __needle: ptr<const i8>) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %21 @mempcpy(%118 __dest: ptr<void> [restrict], %119 __src: ptr<const void> [restrict], %120 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %23 @strlen(%121 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %25 @strerrordesc_np(%122 __err: i32) -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %27 @strerrorname_np(%123 __err: i32) -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %30 @strsep(%124 __stringp: ptr<ptr<i8>> [restrict], %125 __delim: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %33 @strverscmp(%126 __s1: ptr<const i8>, %127 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %36 @memfrob(%128 __s: ptr<void>, %129 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %41 @argz_create_sep(%130 __string: ptr<const i8> [restrict], %131 __sep: i32, %132 __argz: ptr<ptr<i8>> [restrict], %133 __len: ptr<u64> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %44 @argz_count(%134 __argz: ptr<const i8>, %135 __len: u64) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %48 @argz_extract(%136 __argz: ptr<const i8> [restrict], %137 __len: u64, %138 __argv: ptr<ptr<i8>> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %52 @argz_stringify(%139 __argz: ptr<i8>, %140 __len: u64, %141 __sep: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %56 @argz_add(%142 __argz: ptr<ptr<i8>> [restrict], %143 __argz_len: ptr<u64> [restrict], %144 __str: ptr<const i8> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %62 @argz_replace(%145 __argz: ptr<ptr<i8>> [restrict], %146 __argz_len: ptr<u64> [restrict], %147 __str: ptr<const i8> [restrict], %148 __with: ptr<const i8> [restrict], %149 __replace_count: ptr<u32> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %66 @envz_entry(%150 __envz: ptr<const i8> [restrict], %151 __envz_len: u64, %152 __name: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %70 @envz_get(%153 __envz: ptr<const i8> [restrict], %154 __envz_len: u64, %155 __name: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %75 @envz_add(%156 __envz: ptr<ptr<i8>> [restrict], %157 __envz_len: ptr<u64> [restrict], %158 __name: ptr<const i8> [restrict], %159 __value: ptr<const i8> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %79 @envz_remove(%160 __envz: ptr<ptr<i8>> [restrict], %161 __envz_len: ptr<u64> [restrict], %162 __name: ptr<const i8> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %82 @envz_strip(%163 __envz: ptr<ptr<i8>> [restrict], %164 __envz_len: ptr<u64> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %84 @printf(%165 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %86 @free(%166 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %87 @gnu_string_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %88 destination: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %89 repeated: array<i8, 5> [storage=automatic] [const] = code_units<array<i8, 5>>([97, 98, 99, 97, 0]);
// DEFAULT-NEXT:         let %90 obscured: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([103, 110, 117, 0]);
// DEFAULT-NEXT:         let %91 tokens: array<i8, 6> [storage=automatic] = code_units<array<i8, 6>>([97, 58, 58, 98, 99, 0]);
// DEFAULT-NEXT:         let %92 cursor: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(6)>(%91);
// DEFAULT-NEXT:         let %93 token: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %94 end: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%21, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%88)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%167)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         let %95 token_score: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %168 {
// DEFAULT-NEXT:             write<ptr<i8>>(%93, call<ptr<i8>, signature=fn(ptr<ptr<i8>>, ptr<const i8>) -> ptr<i8>>(%30, addr_of<ptr<ptr<i8>>>(%92), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%169))));
// DEFAULT-NEXT:             yield ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<ptr<i8>>, ptr<const i8>) -> ptr<i8>>(%30, addr_of<ptr<ptr<i8>>>(%92), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%169))), null<ptr<i8>>);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%95, add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%95), const<i32>(10)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%23, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%93)))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%36, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%90)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%36, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%90)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(truncate<i32, reason=explicit, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%94), array_decay<ptr<i8>, length=Some(16)>(%88))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%88)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%170))), const<i32>(0))))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%8, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(5)>(%89)), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))), array_decay<ptr<const i8>, length=Some(5)>(%89))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32) -> ptr<void>>(%4, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(5)>(%89)), const<i32>(99))), array_decay<ptr<const i8>, length=Some(5)>(%89))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%171)), const<i32>(122)), array_decay<ptr<i8>, length=Some(4)>(%172))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%17, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%173)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%174))), null<ptr<i8>>)))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(lt<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%33, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%175)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%176))), const<i32>(0))))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%90)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%177))), const<i32>(0))))), widen<i64, reason=usual_arith>(read<i32>(%95))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%11, call<ptr<const i8>, signature=fn(i32) -> ptr<const i8>>(%27, const<i32>(22)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%178))), const<i32>(0))))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(ne<ptr<const i8>>(call<ptr<const i8>, signature=fn(i32) -> ptr<const i8>>(%25, const<i32>(22)), null<ptr<const i8>>)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %96 @gnu_argz_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %97 argz: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %98 length: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %99 arguments: array<ptr<i8>, 6> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %100 replacements: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %101 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %194: i32 [synthetic] = read<i32>(%101);
// DEFAULT-NEXT:         let %195: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%194), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, i32, ptr<ptr<i8>>, ptr<u64>) -> i32>(%41, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%179)), const<i32>(58), addr_of<ptr<ptr<i8>>>(%97), addr_of<ptr<u64>>(%98)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%101, read<i32>(%195));
// DEFAULT-NEXT:         let %196: i32 [synthetic] = read<i32>(%101);
// DEFAULT-NEXT:         let %197: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%196), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%44, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%97)), read<u64>(%98)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))));
// DEFAULT-NEXT:         write<i32>(%101, read<i32>(%197));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, u64, ptr<ptr<i8>>) -> void>(%48, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%97)), read<u64>(%98), array_decay<ptr<ptr<i8>>, length=Some(6)>(%99));
// DEFAULT-NEXT:         let %198: i32 [synthetic] = read<i32>(%101);
// DEFAULT-NEXT:         let %199: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%198), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(6)>(%99), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%180))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%101, read<i32>(%199));
// DEFAULT-NEXT:         let %200: i32 [synthetic] = read<i32>(%101);
// DEFAULT-NEXT:         let %201: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%200), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>) -> i32>(%56, addr_of<ptr<ptr<i8>>>(%97), addr_of<ptr<u64>>(%98), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%181))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%101, read<i32>(%201));
// DEFAULT-NEXT:         let %202: i32 [synthetic] = read<i32>(%101);
// DEFAULT-NEXT:         let %203: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%202), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>, ptr<const i8>, ptr<u32>) -> i32>(%62, addr_of<ptr<ptr<i8>>>(%97), addr_of<ptr<u64>>(%98), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%182)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%183)), addr_of<ptr<u32>>(%100)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%101, read<i32>(%203));
// DEFAULT-NEXT:         let %204: i32 [synthetic] = read<i32>(%101);
// DEFAULT-NEXT:         let %205: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%204), from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%100), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(%101, read<i32>(%205));
// DEFAULT-NEXT:         let %206: i32 [synthetic] = read<i32>(%101);
// DEFAULT-NEXT:         let %207: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%206), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%44, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%97)), read<u64>(%98)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:         write<i32>(%101, read<i32>(%207));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, u64, i32) -> void>(%52, read<ptr<i8>>(%97), read<u64>(%98), const<i32>(44));
// DEFAULT-NEXT:         let %208: i32 [synthetic] = read<i32>(%101);
// DEFAULT-NEXT:         let %209: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%208), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%97)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%184))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%101, read<i32>(%209));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%86, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%97)));
// DEFAULT-NEXT:         return read<i32>(%101);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %102 @gnu_envz_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %103 envz: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %104 length: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %105 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %210: i32 [synthetic] = read<i32>(%105);
// DEFAULT-NEXT:         let %211: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%210), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>, ptr<const i8>) -> i32>(%75, addr_of<ptr<ptr<i8>>>(%103), addr_of<ptr<u64>>(%104), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%185)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%186))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%105, read<i32>(%211));
// DEFAULT-NEXT:         let %212: i32 [synthetic] = read<i32>(%105);
// DEFAULT-NEXT:         let %213: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%212), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>, ptr<const i8>) -> i32>(%75, addr_of<ptr<ptr<i8>>>(%103), addr_of<ptr<u64>>(%104), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%187)), null<ptr<const i8>>), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%105, read<i32>(%213));
// DEFAULT-NEXT:         let %214: i32 [synthetic] = read<i32>(%105);
// DEFAULT-NEXT:         let %215: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%214), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(call<ptr<i8>, signature=fn(ptr<const i8>, u64, ptr<const i8>) -> ptr<i8>>(%70, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%103)), read<u64>(%104), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%188)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%189))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%105, read<i32>(%215));
// DEFAULT-NEXT:         let %216: i32 [synthetic] = read<i32>(%105);
// DEFAULT-NEXT:         let %217: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%216), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, u64, ptr<const i8>) -> ptr<i8>>(%66, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%103)), read<u64>(%104), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%190))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%105, read<i32>(%217));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>) -> void>(%79, addr_of<ptr<ptr<i8>>>(%103), addr_of<ptr<u64>>(%104), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%191)));
// DEFAULT-NEXT:         let %218: i32 [synthetic] = read<i32>(%105);
// DEFAULT-NEXT:         let %219: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%218), from_bool<i32, reason=promotion>(eq<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, u64, ptr<const i8>) -> ptr<i8>>(%70, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%103)), read<u64>(%104), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%192))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%105, read<i32>(%219));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> void>(%82, addr_of<ptr<ptr<i8>>>(%103), addr_of<ptr<u64>>(%104));
// DEFAULT-NEXT:         let %220: i32 [synthetic] = read<i32>(%105);
// DEFAULT-NEXT:         let %221: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%220), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%104), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%105, read<i32>(%221));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%86, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%103)));
// DEFAULT-NEXT:         return read<i32>(%105);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%84, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%193)), call<i32, signature=fn() -> i32>(%87), call<i32, signature=fn() -> i32>(%96), call<i32, signature=fn() -> i32>(%102));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
