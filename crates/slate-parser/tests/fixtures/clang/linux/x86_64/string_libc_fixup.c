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
// DEFAULT-NEXT:     global %49 .str49: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([37, 122, 117, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([37, 99, 32, 37, 99, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 122, 117, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %51 .str51: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%27 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @memcmp(%28 __s1: ptr<const void>, %29 __s2: ptr<const void>, %30 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %3 @strcmp(%31 __s1: ptr<const i8>, %32 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %4 @strncmp(%33 __s1: ptr<const i8>, %34 __s2: ptr<const i8>, %35 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %5 @strchr(%36 __s: ptr<const i8>, %37 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %6 @strrchr(%38 __s: ptr<const i8>, %39 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %7 @strcspn(%40 __s: ptr<const i8>, %41 __reject: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %8 @strspn(%42 __s: ptr<const i8>, %43 __accept: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %9 @strpbrk(%44 __s: ptr<const i8>, %45 __accept: ptr<const i8>) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %10 @strstr(%46 __haystack: ptr<const i8>, %47 __needle: ptr<const i8>) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %11 @strlen(%48 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 alpha: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %14 beta: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([97, 98, 100, 0]);
// DEFAULT-NEXT:         let %15 bytes_a: array<u8, 3> [storage=automatic] = code_units<array<u8, 3>>([255, 1, 0]);
// DEFAULT-NEXT:         let %16 bytes_b: array<u8, 3> [storage=automatic] = code_units<array<u8, 3>>([255, 2, 0]);
// DEFAULT-NEXT:         let %17 hay: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([97, 98, 97, 99, 97, 100, 0]);
// DEFAULT-NEXT:         let %18 sub: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([97, 99, 97, 0]);
// DEFAULT-NEXT:         let %19 empty: array<i8, 1> [storage=automatic] = code_units<array<i8, 1>>([0]);
// DEFAULT-NEXT:         let %20 set: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([99, 120, 0]);
// DEFAULT-NEXT:         let %21 prefix: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([97, 98, 0]);
// DEFAULT-NEXT:         let %22 reject: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([99, 100, 0]);
// DEFAULT-NEXT:         let %23 utf8: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([104, 195, 169, 0]);
// DEFAULT-NEXT:         let %24 second_byte: i32 [storage=automatic] = const<i32>(169);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%49)), call<u64, signature=fn(ptr<const i8>) -> u64>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13))), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13))), const<i32>(0))), from_bool<i32, reason=vararg>(lt<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%14))), const<i32>(0))), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%14)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))), const<i32>(0))), from_bool<i32, reason=vararg>(eq<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%15)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(3)>(%16)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), const<i32>(0))));
// DEFAULT-NEXT:         let %25 first: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), const<i32>(97));
// DEFAULT-NEXT:         let %26 last: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%6, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), const<i32>(97));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(27)>(%50)), widen<i32, reason=vararg>(read<i8>(deref(read<ptr<i8>>(%25)))), widen<i32, reason=vararg>(read<i8>(deref(read<ptr<i8>>(%26)))), from_bool<i32, reason=vararg>(eq<ptr<i8>>(read<ptr<i8>>(%26), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%25), const<i32>(4)))), from_bool<i32, reason=vararg>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%10, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18))), null<ptr<i8>>)), from_bool<i32, reason=vararg>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%10, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%19))), null<ptr<i8>>)), from_bool<i32, reason=vararg>(eq<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%9, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%20))), null<ptr<i8>>)), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%21))), call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%22))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%51)), from_bool<i32, reason=vararg>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), const<i32>(0)), null<ptr<i8>>)), from_bool<i32, reason=vararg>(eq<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), const<i32>(122)), null<ptr<i8>>)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%52)), from_bool<i32, reason=vararg>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%23)), read<i32>(%24)), null<ptr<i8>>)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
