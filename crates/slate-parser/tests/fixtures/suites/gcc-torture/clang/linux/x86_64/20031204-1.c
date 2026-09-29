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
// DEFAULT-NEXT:     type @type[[TYPE_u32:[0-9]+]] u32 = u64;
// DEFAULT-NEXT:     global %[[VALUE_addr:[0-9]+]] addr: array<i8, 19> [storage=static] [align=16] = code_units<array<i8, 19>>([49, 48, 46, 49, 49, 46, 49, 50, 46, 49, 51, 58, 47, 104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<i8> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_in_aton:[0-9]+]] @in_aton(%[[VALUE_x:[0-9]+]] x: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(168496141)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_root_nfs_parse_addr:[0-9]+]] @root_nfs_parse_addr(%[[VALUE_name:[0-9]+]] name: ptr<i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_addr_2:[0-9]+]] addr: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_octets:[0-9]+]] octets: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_cp:[0-9]+]] cp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cq:[0-9]+]] cq: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_cq]], read<ptr<i8>>(%[[VALUE_name]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_cp]], read<ptr<i8>>(%[[VALUE_name]]));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] lt<i32>(read<i32>(%[[VALUE_octets]]), const<i32>(4))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %[[VALUE1:[0-9]+]] logical_and<bool>(ge<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_cp]])))), const<i32>(48)), le<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_cp]])))), const<i32>(57)))
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_cp]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_cp]], read<ptr<i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:                 if logical_or<bool>(eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_cp]]), read<ptr<i8>>(%[[VALUE_cq]])), gt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%[[VALUE_cp]]), read<ptr<i8>>(%[[VALUE_cq]])), widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:                     break %[[VALUE0]];
// DEFAULT-NEXT:                 if logical_or<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_cp]])))), const<i32>(46)), eq<i32>(read<i32>(%[[VALUE_octets]]), const<i32>(3)))
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_octets]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_octets]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 if lt<i32>(read<i32>(%[[VALUE_octets]]), const<i32>(4))
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_cp]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_cp]], read<ptr<i8>>(%[[VALUE7]]));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_cq]], read<ptr<i8>>(%[[VALUE_cp]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_octets]]), const<i32>(4)), logical_or<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_cp]])))), const<i32>(58)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_cp]])))), const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_cp]])))), const<i32>(58))
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_cp]]);
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_cp]], read<ptr<i8>>(%[[VALUE9]]));
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%[[VALUE8]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_addr_2]], call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_in_aton]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_name]]))));
// DEFAULT-NEXT:                 call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_in_aton]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_name]])));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], read<ptr<i8>>(%[[VALUE_name]]), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_cp]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u64>(%[[VALUE_addr_2]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_addr_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>) -> u64>(%[[VALUE_root_nfs_parse_addr]], array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_addr]]));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_result]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(168496141))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
