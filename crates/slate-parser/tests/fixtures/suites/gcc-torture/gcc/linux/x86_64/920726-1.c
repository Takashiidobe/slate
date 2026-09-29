#include <stdarg.h>
#include <stdio.h>

void abort(void);
void exit(int);

struct spurious {
  int anumber;
};

int first(char *buf, char *fmt, ...) {
  int     pos, number;
  va_list args;
  int     dummy;
  char   *bp = buf;

  va_start(args, fmt);
  for (pos = 0; fmt[pos]; pos++)
    if (fmt[pos] == 'i') {
      number = va_arg(args, int);
      sprintf(bp, "%d", number);
      bp += __builtin_strlen(bp);
    } else
      *bp++ = fmt[pos];

  va_end(args);
  *bp = 0;
  return dummy;
}

struct spurious second(char *buf, char *fmt, ...) {
  int             pos, number;
  va_list         args;
  struct spurious dummy;
  char           *bp = buf;

  va_start(args, fmt);
  for (pos = 0; fmt[pos]; pos++)
    if (fmt[pos] == 'i') {
      number = va_arg(args, int);
      sprintf(bp, "%d", number);
      bp += __builtin_strlen(bp);
    } else
      *bp++ = fmt[pos];

  va_end(args);
  *bp = 0;
  return dummy;
}

int main(void) {
  char buf1[100], buf2[100];
  first(buf1, "i i ", 5, 20);
  second(buf2, "i i ", 5, 20);
  if (__builtin_strcmp("5 20 ", buf1) || __builtin_strcmp("5 20 ", buf2))
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_spurious:[0-9]+]] spurious = struct {
// DEFAULT-NEXT:         field0 anumber: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([105, 32, 105, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([105, 32, 105, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([53, 32, 50, 48, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([53, 32, 50, 48, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_sprintf:[0-9]+]] @sprintf(%[[VALUE___s:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strlen:[0-9]+]] @__builtin_strlen(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_first:[0-9]+]] @first(%[[VALUE_buf:[0-9]+]] buf: ptr<i8>, %[[VALUE_fmt:[0-9]+]] fmt: ptr<i8>, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_pos:[0-9]+]] pos: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_number:[0-9]+]] number: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_args:[0-9]+]] args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_dummy:[0-9]+]] dummy: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bp:[0-9]+]] bp: ptr<i8> [storage=automatic] = read<ptr<i8>>(%[[VALUE_buf]]);
// DEFAULT-NEXT:         va_start(%[[VALUE_args]]);
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_pos]], const<i32>(0));
// DEFAULT-NEXT:             condition: ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_fmt]]), read<i32>(%[[VALUE_pos]])))), const<i8>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_pos]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_pos]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_fmt]]), read<i32>(%[[VALUE_pos]]))))), const<i32>(105))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_number]], va_arg<i32>(%[[VALUE_args]]));
// DEFAULT-NEXT:                         va_arg<i32>(%[[VALUE_args]]);
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sprintf]], read<ptr<i8>>(%[[VALUE_bp]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), read<i32>(%[[VALUE_number]]));
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_bp]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE5]]), call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE___builtin_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_bp]]))));
// DEFAULT-NEXT:                         write<ptr<i8>>(%[[VALUE_bp]], read<ptr<i8>>(%[[VALUE6]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_bp]]);
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_bp]], read<ptr<i8>>(%[[VALUE8]]));
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%[[VALUE7]])), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_fmt]]), read<i32>(%[[VALUE_pos]])))));
// DEFAULT-NEXT:         va_end(%[[VALUE_args]]);
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_bp]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_dummy]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_second:[0-9]+]] @second(%[[VALUE_buf_2:[0-9]+]] buf: ptr<i8>, %[[VALUE_fmt_2:[0-9]+]] fmt: ptr<i8>, ...) -> @type[[TYPE_spurious]] [linkage=external] [abi=sysv64(scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_pos_2:[0-9]+]] pos: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_number_2:[0-9]+]] number: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_args_2:[0-9]+]] args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_dummy_2:[0-9]+]] dummy: @type[[TYPE_spurious]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bp_2:[0-9]+]] bp: ptr<i8> [storage=automatic] = read<ptr<i8>>(%[[VALUE_buf_2]]);
// DEFAULT-NEXT:         va_start(%[[VALUE_args_2]]);
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_pos_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_fmt_2]]), read<i32>(%[[VALUE_pos_2]])))), const<i8>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_pos_2]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_pos_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_fmt_2]]), read<i32>(%[[VALUE_pos_2]]))))), const<i32>(105))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_number_2]], va_arg<i32>(%[[VALUE_args_2]]));
// DEFAULT-NEXT:                         va_arg<i32>(%[[VALUE_args_2]]);
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sprintf]], read<ptr<i8>>(%[[VALUE_bp_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_number_2]]));
// DEFAULT-NEXT:                         let %[[VALUE12:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_bp_2]]);
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE12]]), call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE___builtin_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_bp_2]]))));
// DEFAULT-NEXT:                         write<ptr<i8>>(%[[VALUE_bp_2]], read<ptr<i8>>(%[[VALUE13]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     let %[[VALUE14:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_bp_2]]);
// DEFAULT-NEXT:                     let %[[VALUE15:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_bp_2]], read<ptr<i8>>(%[[VALUE15]]));
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%[[VALUE14]])), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_fmt_2]]), read<i32>(%[[VALUE_pos_2]])))));
// DEFAULT-NEXT:         va_end(%[[VALUE_args_2]]);
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_bp_2]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_spurious]], reason=return>(read<@type[[TYPE_spurious]]>(%[[VALUE_dummy_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcmp:[0-9]+]] @__builtin_strcmp(%[[VALUE16:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE17:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_buf1:[0-9]+]] buf1: array<i8, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_buf2:[0-9]+]] buf2: array<i8, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<i8>, ...) -> i32>(%[[VALUE_first]], array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_buf1]]), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_3]]), const<i32>(5), const<i32>(20));
// DEFAULT-NEXT:         call<@type[[TYPE_spurious]], signature=fn(ptr<i8>, ptr<i8>, ...) -> @type[[TYPE_spurious]], abi=sysv64(scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_second]], array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_buf2]]), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_4]]), const<i32>(5), const<i32>(20));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE___builtin_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_5]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_buf1]]))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE___builtin_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_6]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_buf2]]))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE18]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
