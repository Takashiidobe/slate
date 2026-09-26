/* PR optimization/13260 */

#include <string.h>

void abort(void);

typedef unsigned long u32;

u32 in_aton(const char *x) { return 0x0a0b0c0d; }

u32 root_nfs_parse_addr(char *name) {
  u32   addr;
  int   octets = 0;
  char *cp, *cq;

  cp = cq = name;
  while (octets < 4) {
    while (*cp >= '0' && *cp <= '9')
      cp++;
    if (cp == cq || cp - cq > 3)
      break;
    if (*cp == '.' || octets == 3)
      octets++;
    if (octets < 4)
      cp++;
    cq = cp;
  }

  if (octets == 4 && (*cp == ':' || *cp == '\0')) {
    if (*cp == ':')
      *cp++ = '\0';
    addr = in_aton(name);
    strcpy(name, cp);
  } else
    addr = (-1);

  return addr;
}

int main() {
  static char addr[] = "10.11.12.13:/hello";
  u32         result = root_nfs_parse_addr(addr);
  if (result != 0x0a0b0c0d) {
    abort();
  }
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
// DEFAULT-NEXT:     type @type0 u32 = u64;
// DEFAULT-NEXT:     global %12 addr: array<i8, 19> [storage=static] [align=16] = code_units<array<i8, 19>>([49, 48, 46, 49, 49, 46, 49, 50, 46, 49, 51, 58, 47, 104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @strcpy(%14 __dest: ptr<i8> [restrict], %15 __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @in_aton(%4 x: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(168496141)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @root_nfs_parse_addr(%6 name: ptr<i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 addr: u64 [storage=automatic];
// DEFAULT-NEXT:         let %8 octets: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %9 cp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %10 cq: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%10, read<ptr<i8>>(%6));
// DEFAULT-NEXT:         write<ptr<i8>>(%9, read<ptr<i8>>(%6));
// DEFAULT-NEXT:         while %16 lt<i32>(read<i32>(%8), const<i32>(4))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %17 logical_and<bool>(ge<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(48)), le<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(57)))
// DEFAULT-NEXT:                     let %18: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:                     let %19: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%18), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%9, read<ptr<i8>>(%19));
// DEFAULT-NEXT:                 if logical_or<bool>(eq<ptr<i8>>(read<ptr<i8>>(%9), read<ptr<i8>>(%10)), gt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%9), read<ptr<i8>>(%10)), widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:                     break %16;
// DEFAULT-NEXT:                 if logical_or<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(46)), eq<i32>(read<i32>(%8), const<i32>(3)))
// DEFAULT-NEXT:                     let %20: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%8, read<i32>(%21));
// DEFAULT-NEXT:                 if lt<i32>(read<i32>(%8), const<i32>(4))
// DEFAULT-NEXT:                     let %22: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:                     let %23: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%22), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%9, read<ptr<i8>>(%23));
// DEFAULT-NEXT:                 write<ptr<i8>>(%10, read<ptr<i8>>(%9));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_and<bool>(eq<i32>(read<i32>(%8), const<i32>(4)), logical_or<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(58)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(58))
// DEFAULT-NEXT:                     let %24: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:                     let %25: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%24), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%9, read<ptr<i8>>(%25));
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%24)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 write<u64>(%7, call<u64, signature=fn(ptr<const i8>) -> u64>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%6))));
// DEFAULT-NEXT:                 call<u64, signature=fn(ptr<const i8>) -> u64>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%6)));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(strcpy, read<ptr<i8>>(%6), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%9)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u64>(%7, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 result: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>) -> u64>(%5, array_decay<ptr<i8>, length=Some(19)>(%12));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%13), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(168496141))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
