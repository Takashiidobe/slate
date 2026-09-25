/* { dg-require-effective-target mmap } */
/* { dg-skip-if "the executable is at the same position the test tries to remap"
 * { m68k-*-linux* } } */

#include <limits.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifndef MAP_ANON
#ifdef MAP_ANONYMOUS
#define MAP_ANON MAP_ANONYMOUS
#else
#define MAP_ANON MAP_FILE
#endif
#endif
#ifndef MAP_FILE
#define MAP_FILE 0
#endif
#ifndef MAP_FIXED
#define MAP_FIXED 0
#endif

#define MAP_START (void *)0x7fff8000
#define MAP_LEN   0x10000

#define OFFSET (MAP_LEN / 2 - 2 * sizeof(char));

void f(int s, char *p) {
  int i;
  for (i = s; i >= 0 && &p[i] < &p[40]; i++) {
    p[i] = -2;
  }
}

int main(void) {
#ifdef MAP_ANON
  char *p;
  int   dev_zero;

  dev_zero = open("/dev/zero", O_RDONLY);
  /* -1 is OK when we have MAP_ANON; else mmap will flag an error.  */
  if (INT_MAX != 0x7fffffffL || sizeof(char *) != sizeof(int))
    return 0;
  p = mmap(MAP_START, MAP_LEN, PROT_READ | PROT_WRITE,
           MAP_ANON | MAP_FIXED | MAP_PRIVATE, dev_zero, 0);
  if (p != (char *)-1) {
    p     += OFFSET;
    p[39]  = 0;
    f(0, p);
    if (p[39] != (char)-2)
      __builtin_abort();
    p[39] = 0;
    f(-1, p);
    if (p[39] != 0)
      __builtin_abort();
  }
#endif
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
// DEFAULT-NEXT:     type @type0 __off_t = i64;
// DEFAULT-NEXT:     type @type1 size_t = u64;
// DEFAULT-NEXT:     global %20 .str20: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([47, 100, 101, 118, 47, 122, 101, 114, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @open(%11 __file: ptr<const i8>, %12 __oflag: i32, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @mmap(%13 __addr: ptr<void>, %14 __len: u64, %15 __prot: i32, %16 __flags: i32, %17 __fd: i32, %18 __offset: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @f(%5 s: i32, %6 p: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%5));
// DEFAULT-NEXT:             condition: logical_and<bool>(ge<i32>(read<i32>(%7), const<i32>(0)), lt<ptr<i8>>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), read<i32>(%7)))), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), const<i32>(40))))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), read<i32>(%7))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(2))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %10 dev_zero: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%10, call<i32, signature=fn(ptr<const i8>, i32, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%20)), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, i32, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%20)), const<i32>(0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(widen<i64, reason=usual_arith>(const<i32>(2147483647)), const<i64>(2147483647)), ne<u64>(const<u64>(8), const<u64>(4)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<ptr<i8>>(%9, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, u64, i32, i32, i32, i64) -> ptr<void>>(%3, int_to_ptr<ptr<void>, reason=explicit>(const<i32>(2147450880)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(65536))), or<i32>(const<i32>(1), const<i32>(2)), or<i32>(or<i32>(const<i32>(32), const<i32>(16)), const<i32>(2)), read<i32>(%10), widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, u64, i32, i32, i32, i64) -> ptr<void>>(%3, int_to_ptr<ptr<void>, reason=explicit>(const<i32>(2147450880)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(65536))), or<i32>(const<i32>(1), const<i32>(2)), or<i32>(or<i32>(const<i32>(32), const<i32>(16)), const<i32>(2)), read<i32>(%10), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%9), int_to_ptr<ptr<i8>, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %23: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:                 let %24: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%23), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(65536), const<i32>(2)))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(1))));
// DEFAULT-NEXT:                 write<ptr<i8>>(%9, read<ptr<i8>>(%24));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(39))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 call<void, signature=fn(i32, ptr<i8>) -> void>(%4, const<i32>(0), read<ptr<i8>>(%9));
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(39))))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(39))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 call<void, signature=fn(i32, ptr<i8>) -> void>(%4, neg<i32, overflow=ub>(const<i32>(1)), read<ptr<i8>>(%9));
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(39))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
