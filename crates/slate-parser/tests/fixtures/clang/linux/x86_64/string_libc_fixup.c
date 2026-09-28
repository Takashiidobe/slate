#include <stdio.h>
#include <string.h>

int main(void) {
  char          alpha[]     = "abc";
  char          beta[]      = "abd";
  unsigned char bytes_a[]   = "\xff\x01";
  unsigned char bytes_b[]   = "\xff\x02";
  char          hay[]       = "abacad";
  char          sub[]       = "aca";
  char          empty[]     = "";
  char          set[]       = "cx";
  char          prefix[]    = "ab";
  char          reject[]    = "cd";
  char          utf8[]      = "hé";
  int           second_byte = 0xa9;

  printf("%zu %d %d %d %d\n", strlen(alpha), strcmp(alpha, alpha) == 0,
         strcmp(alpha, beta) < 0, strncmp(alpha, beta, 2) == 0,
         memcmp(bytes_a, bytes_b, 1) == 0);
  char *first = strchr(hay, 'a');
  char *last  = strrchr(hay, 'a');
  printf("%c %c %d %d %d %d %zu %zu\n", *first, *last, last == first + 4,
         strstr(hay, sub) != 0, strstr(hay, empty) != 0, strpbrk(hay, set) == 0,
         strspn(hay, prefix), strcspn(hay, reject));
  printf("%d %d\n", strchr(hay, 0) != 0, strchr(hay, 'z') == 0);
  printf("%d\n", strchr(utf8, second_byte) != 0);
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
// DEFAULT-NEXT:     global %71 .str71: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([37, 122, 117, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([37, 99, 32, 37, 99, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 122, 117, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 .str73: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%49 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @memcmp(%50 __s1: ptr<const void>, %51 __s2: ptr<const void>, %52 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %9 @strcmp(%53 __s1: ptr<const i8>, %54 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %13 @strncmp(%55 __s1: ptr<const i8>, %56 __s2: ptr<const i8>, %57 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %16 @strchr(%58 __s: ptr<const i8>, %59 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %19 @strrchr(%60 __s: ptr<const i8>, %61 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %22 @strcspn(%62 __s: ptr<const i8>, %63 __reject: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %25 @strspn(%64 __s: ptr<const i8>, %65 __accept: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %28 @strpbrk(%66 __s: ptr<const i8>, %67 __accept: ptr<const i8>) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %31 @strstr(%68 __haystack: ptr<const i8>, %69 __needle: ptr<const i8>) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %33 @strlen(%70 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %34 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %35 alpha: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %36 beta: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([97, 98, 100, 0]);
// DEFAULT-NEXT:         let %37 bytes_a: array<u8, 3> [storage=automatic] = code_units<array<u8, 3>>([255, 1, 0]);
// DEFAULT-NEXT:         let %38 bytes_b: array<u8, 3> [storage=automatic] = code_units<array<u8, 3>>([255, 2, 0]);
// DEFAULT-NEXT:         let %39 hay: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([97, 98, 97, 99, 97, 100, 0]);
// DEFAULT-NEXT:         let %40 sub: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([97, 99, 97, 0]);
// DEFAULT-NEXT:         let %41 empty: array<i8, 1> [storage=automatic] = code_units<array<i8, 1>>([0]);
// DEFAULT-NEXT:         let %42 set: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([99, 120, 0]);
// DEFAULT-NEXT:         let %43 prefix: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([97, 98, 0]);
// DEFAULT-NEXT:         let %44 reject: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([99, 100, 0]);
// DEFAULT-NEXT:         let %45 utf8: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([104, 195, 169, 0]);
// DEFAULT-NEXT:         let %46 second_byte: i32 [storage=automatic] = const<i32>(169);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%71)), call<u64, signature=fn(ptr<const i8>) -> u64>(%33, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%35))), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%9, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%35)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%35))), const<i32>(0))), from_bool<i32, reason=vararg>(lt<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%9, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%35)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%36))), const<i32>(0))), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%13, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%35)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%36)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))), const<i32>(0))), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%6, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%37)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%38)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), const<i32>(0))));
// DEFAULT-NEXT:         let %47 first: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), const<i32>(97));
// DEFAULT-NEXT:         let %48 last: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%19, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), const<i32>(97));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(27)>(%72)), widen<i32, reason=vararg>(read<i8>(deref(read<ptr<i8>>(%47)))), widen<i32, reason=vararg>(read<i8>(deref(read<ptr<i8>>(%48)))), from_bool<i32, reason=vararg>(eq<ptr<i8>>(read<ptr<i8>>(%48), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%47), const<i32>(4)))), from_bool<i32, reason=vararg>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%31, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%40))), null<ptr<i8>>)), from_bool<i32, reason=vararg>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%31, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%41))), null<ptr<i8>>)), from_bool<i32, reason=vararg>(eq<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%28, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%42))), null<ptr<i8>>)), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%43))), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%44))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%73)), from_bool<i32, reason=vararg>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), const<i32>(0)), null<ptr<i8>>)), from_bool<i32, reason=vararg>(eq<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%39)), const<i32>(122)), null<ptr<i8>>)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%74)), from_bool<i32, reason=vararg>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%45)), read<i32>(%46)), null<ptr<i8>>)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
