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
// DEFAULT-NEXT:     type @type[[TYPE_error_t:[0-9]+]] error_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([58, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([71, 78, 85, 32, 76, 105, 98, 114, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([108, 105, 98, 114, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([114, 101, 108, 101, 97, 115, 101, 45, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([114, 101, 108, 101, 97, 115, 101, 45, 49, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 110, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([69, 73, 78, 86, 65, 76, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([111, 110, 101, 58, 116, 119, 111, 58, 116, 104, 114, 101, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([116, 119, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 111, 117, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([116, 104, 114, 101, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([84, 72, 82, 69, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([111, 110, 101, 44, 116, 119, 111, 44, 84, 72, 82, 69, 69, 44, 102, 111, 117, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([66, 69, 84, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([66, 69, 84, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([65, 76, 80, 72, 65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_rawmemchr:[0-9]+]] @rawmemchr(%[[VALUE___s:[0-9]+]] __s: ptr<const void>, %[[VALUE___c:[0-9]+]] __c: i32) -> ptr<void> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_memrchr:[0-9]+]] @memrchr(%[[VALUE___s_2:[0-9]+]] __s: ptr<const void>, %[[VALUE___c_2:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strchrnul:[0-9]+]] @strchrnul(%[[VALUE___s_3:[0-9]+]] __s: ptr<const i8>, %[[VALUE___c_3:[0-9]+]] __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strcasestr:[0-9]+]] @strcasestr(%[[VALUE___haystack:[0-9]+]] __haystack: ptr<const i8>, %[[VALUE___needle:[0-9]+]] __needle: ptr<const i8>) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_mempcpy:[0-9]+]] @mempcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n_2:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE___s_4:[0-9]+]] __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strerrordesc_np:[0-9]+]] @strerrordesc_np(%[[VALUE___err:[0-9]+]] __err: i32) -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strerrorname_np:[0-9]+]] @strerrorname_np(%[[VALUE___err_2:[0-9]+]] __err: i32) -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strsep:[0-9]+]] @strsep(%[[VALUE___stringp:[0-9]+]] __stringp: ptr<ptr<i8>> [restrict], %[[VALUE___delim:[0-9]+]] __delim: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strverscmp:[0-9]+]] @strverscmp(%[[VALUE___s1_2:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2_2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_memfrob:[0-9]+]] @memfrob(%[[VALUE___s_5:[0-9]+]] __s: ptr<void>, %[[VALUE___n_3:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_argz_create_sep:[0-9]+]] @argz_create_sep(%[[VALUE___string:[0-9]+]] __string: ptr<const i8> [restrict], %[[VALUE___sep:[0-9]+]] __sep: i32, %[[VALUE___argz:[0-9]+]] __argz: ptr<ptr<i8>> [restrict], %[[VALUE___len:[0-9]+]] __len: ptr<u64> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_argz_count:[0-9]+]] @argz_count(%[[VALUE___argz_2:[0-9]+]] __argz: ptr<const i8>, %[[VALUE___len_2:[0-9]+]] __len: u64) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_argz_extract:[0-9]+]] @argz_extract(%[[VALUE___argz_3:[0-9]+]] __argz: ptr<const i8> [restrict], %[[VALUE___len_3:[0-9]+]] __len: u64, %[[VALUE___argv:[0-9]+]] __argv: ptr<ptr<i8>> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_argz_stringify:[0-9]+]] @argz_stringify(%[[VALUE___argz_4:[0-9]+]] __argz: ptr<i8>, %[[VALUE___len_4:[0-9]+]] __len: u64, %[[VALUE___sep_2:[0-9]+]] __sep: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_argz_add:[0-9]+]] @argz_add(%[[VALUE___argz_5:[0-9]+]] __argz: ptr<ptr<i8>> [restrict], %[[VALUE___argz_len:[0-9]+]] __argz_len: ptr<u64> [restrict], %[[VALUE___str:[0-9]+]] __str: ptr<const i8> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_argz_replace:[0-9]+]] @argz_replace(%[[VALUE___argz_6:[0-9]+]] __argz: ptr<ptr<i8>> [restrict], %[[VALUE___argz_len_2:[0-9]+]] __argz_len: ptr<u64> [restrict], %[[VALUE___str_2:[0-9]+]] __str: ptr<const i8> [restrict], %[[VALUE___with:[0-9]+]] __with: ptr<const i8> [restrict], %[[VALUE___replace_count:[0-9]+]] __replace_count: ptr<u32> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_envz_entry:[0-9]+]] @envz_entry(%[[VALUE___envz:[0-9]+]] __envz: ptr<const i8> [restrict], %[[VALUE___envz_len:[0-9]+]] __envz_len: u64, %[[VALUE___name:[0-9]+]] __name: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_envz_get:[0-9]+]] @envz_get(%[[VALUE___envz_2:[0-9]+]] __envz: ptr<const i8> [restrict], %[[VALUE___envz_len_2:[0-9]+]] __envz_len: u64, %[[VALUE___name_2:[0-9]+]] __name: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_envz_add:[0-9]+]] @envz_add(%[[VALUE___envz_3:[0-9]+]] __envz: ptr<ptr<i8>> [restrict], %[[VALUE___envz_len_3:[0-9]+]] __envz_len: ptr<u64> [restrict], %[[VALUE___name_3:[0-9]+]] __name: ptr<const i8> [restrict], %[[VALUE___value:[0-9]+]] __value: ptr<const i8> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_envz_remove:[0-9]+]] @envz_remove(%[[VALUE___envz_4:[0-9]+]] __envz: ptr<ptr<i8>> [restrict], %[[VALUE___envz_len_4:[0-9]+]] __envz_len: ptr<u64> [restrict], %[[VALUE___name_4:[0-9]+]] __name: ptr<const i8> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_envz_strip:[0-9]+]] @envz_strip(%[[VALUE___envz_5:[0-9]+]] __envz: ptr<ptr<i8>> [restrict], %[[VALUE___envz_len_5:[0-9]+]] __envz_len: ptr<u64> [restrict]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_string_extensions:[0-9]+]] @gnu_string_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_destination:[0-9]+]] destination: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_repeated:[0-9]+]] repeated: array<i8, 5> [storage=automatic] [const] = code_units<array<i8, 5>>([97, 98, 99, 97, 0]);
// DEFAULT-NEXT:         let %[[VALUE_obscured:[0-9]+]] obscured: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([103, 110, 117, 0]);
// DEFAULT-NEXT:         let %[[VALUE_tokens:[0-9]+]] tokens: array<i8, 6> [storage=automatic] = code_units<array<i8, 6>>([97, 58, 58, 98, 99, 0]);
// DEFAULT-NEXT:         let %[[VALUE_cursor:[0-9]+]] cursor: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_tokens]]);
// DEFAULT-NEXT:         let %[[VALUE_token:[0-9]+]] token: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_end:[0-9]+]] end: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_mempcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_destination]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         let %[[VALUE_token_score:[0-9]+]] token_score: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = call<ptr<i8>, signature=fn(ptr<ptr<i8>>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strsep]], addr_of<ptr<ptr<i8>>>(%[[VALUE_cursor]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_token]], read<ptr<i8>>(%[[VALUE1]]));
// DEFAULT-NEXT:             yield ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE1]]), null<ptr<i8>>);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_token_score]], add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_token_score]]), const<i32>(10)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_token]])))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_memfrob]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_obscured]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_memfrob]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_obscured]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(truncate<i32, reason=explicit, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%[[VALUE_end]]), array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_destination]]))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_destination]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_3]]))), const<i32>(0))))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memrchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(5)>(%[[VALUE_repeated]])), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))), array_decay<ptr<const i8>, length=Some(5)>(%[[VALUE_repeated]]))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32) -> ptr<void>>(%[[VALUE_rawmemchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(5)>(%[[VALUE_repeated]])), const<i32>(99))), array_decay<ptr<const i8>, length=Some(5)>(%[[VALUE_repeated]]))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchrnul]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), const<i32>(122)), array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_5]]))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcasestr]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_6]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_7]]))), null<ptr<i8>>)))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(lt<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strverscmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_8]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_9]]))), const<i32>(0))))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_obscured]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_10]]))), const<i32>(0))))), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_token_score]]))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], call<ptr<const i8>, signature=fn(i32) -> ptr<const i8>>(%[[VALUE_strerrorname_np]], const<i32>(22)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_11]]))), const<i32>(0))))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(ne<ptr<const i8>>(call<ptr<const i8>, signature=fn(i32) -> ptr<const i8>>(%[[VALUE_strerrordesc_np]], const<i32>(22)), null<ptr<const i8>>)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_argz_extensions:[0-9]+]] @gnu_argz_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_argz:[0-9]+]] argz: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %[[VALUE_length:[0-9]+]] length: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_arguments:[0-9]+]] arguments: array<ptr<i8>, 6> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_replacements:[0-9]+]] replacements: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, i32, ptr<ptr<i8>>, ptr<u64>) -> i32>(%[[VALUE_argz_create_sep]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_12]])), const<i32>(58), addr_of<ptr<ptr<i8>>>(%[[VALUE_argz]]), addr_of<ptr<u64>>(%[[VALUE_length]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%[[VALUE_argz_count]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_argz]])), read<u64>(%[[VALUE_length]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, u64, ptr<ptr<i8>>) -> void>(%[[VALUE_argz_extract]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_argz]])), read<u64>(%[[VALUE_length]]), array_decay<ptr<ptr<i8>>, length=Some(6)>(%[[VALUE_arguments]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(6)>(%[[VALUE_arguments]]), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_13]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>) -> i32>(%[[VALUE_argz_add]], addr_of<ptr<ptr<i8>>>(%[[VALUE_argz]]), addr_of<ptr<u64>>(%[[VALUE_length]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_14]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>, ptr<const i8>, ptr<u32>) -> i32>(%[[VALUE_argz_replace]], addr_of<ptr<ptr<i8>>>(%[[VALUE_argz]]), addr_of<ptr<u64>>(%[[VALUE_length]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_15]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_16]])), addr_of<ptr<u32>>(%[[VALUE_replacements]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%[[VALUE_replacements]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%[[VALUE_argz_count]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_argz]])), read<u64>(%[[VALUE_length]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, u64, i32) -> void>(%[[VALUE_argz_stringify]], read<ptr<i8>>(%[[VALUE_argz]]), read<u64>(%[[VALUE_length]]), const<i32>(44));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_argz]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_17]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_argz]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_envz_extensions:[0-9]+]] @gnu_envz_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_envz:[0-9]+]] envz: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %[[VALUE_length_2:[0-9]+]] length: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_total_2:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_envz_add]], addr_of<ptr<ptr<i8>>>(%[[VALUE_envz]]), addr_of<ptr<u64>>(%[[VALUE_length_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_18]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_19]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_envz_add]], addr_of<ptr<ptr<i8>>>(%[[VALUE_envz]]), addr_of<ptr<u64>>(%[[VALUE_length_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_20]])), null<ptr<const i8>>), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(call<ptr<i8>, signature=fn(ptr<const i8>, u64, ptr<const i8>) -> ptr<i8>>(%[[VALUE_envz_get]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_envz]])), read<u64>(%[[VALUE_length_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_21]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_22]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, u64, ptr<const i8>) -> ptr<i8>>(%[[VALUE_envz_entry]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_envz]])), read<u64>(%[[VALUE_length_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_23]]))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i8>>, ptr<u64>, ptr<const i8>) -> void>(%[[VALUE_envz_remove]], addr_of<ptr<ptr<i8>>>(%[[VALUE_envz]]), addr_of<ptr<u64>>(%[[VALUE_length_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_24]])));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), from_bool<i32, reason=promotion>(eq<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, u64, ptr<const i8>) -> ptr<i8>>(%[[VALUE_envz_get]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_envz]])), read<u64>(%[[VALUE_length_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_25]]))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i8>>, ptr<u64>) -> void>(%[[VALUE_envz_strip]], addr_of<ptr<ptr<i8>>>(%[[VALUE_envz]]), addr_of<ptr<u64>>(%[[VALUE_length_2]]));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%[[VALUE_length_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_envz]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_26]])), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_string_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_argz_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_envz_extensions]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
