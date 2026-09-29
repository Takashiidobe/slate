/* PR tree-optimization/51315 */
/* Reported by Jurij Smakov <jurij@wooyd.org> */

typedef __SIZE_TYPE__ size_t;

extern void *memcpy(void *__restrict __dest, __const void *__restrict __src,
                    size_t __n) __attribute__((__nothrow__))
__attribute__((__nonnull__(1, 2)));

extern size_t strlen(__const char *__s) __attribute__((__nothrow__))
__attribute__((__pure__)) __attribute__((__nonnull__(1)));

typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;

extern void abort(void);

int a;

static void __attribute__((noinline, noclone)) do_something(int item) {
  a = item;
}

int pack_unpack(char *s, char *p) {
  char *send, *pend;
  char  type;
  int   integer_size;

  send = s + strlen(s);
  pend = p + strlen(p);

  while (p < pend) {
    type = *p++;

    switch (type) {
    case 's':
      integer_size = 2;
      goto unpack_integer;

    case 'l':
      integer_size = 4;
      goto unpack_integer;

    unpack_integer:
      switch (integer_size) {
      case 2: {
        union {
          int16_t i;
          char    a[sizeof(int16_t)];
        } v;
        memcpy(v.a, s, sizeof(int16_t));
        s += sizeof(int16_t);
        do_something(v.i);
      } break;

      case 4: {
        union {
          int32_t i;
          char    a[sizeof(int32_t)];
        } v;
        memcpy(v.a, s, sizeof(int32_t));
        s += sizeof(int32_t);
        do_something(v.i);
      } break;
      }
      break;
    }
  }
  return (int)*s;
}

int main(void) {
  int n = pack_unpack("\200\001\377\376\035\300", "sl");
  if (n != 0)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_int16_t:[0-9]+]] int16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 i: i16;
// DEFAULT-NEXT:         field1 a: array<i8, 2>;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 a: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([128, 1, 255, 254, 29, 192, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([115, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE___s:[0-9]+]] __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_do_something:[0-9]+]] @do_something(%[[VALUE_item:[0-9]+]] item: i32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE_item]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pack_unpack:[0-9]+]] @pack_unpack(%[[VALUE_s:[0-9]+]] s: ptr<i8>, %[[VALUE_p:[0-9]+]] p: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_send:[0-9]+]] send: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pend:[0-9]+]] pend: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_type:[0-9]+]] type: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_integer_size:[0-9]+]] integer_size: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_send]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_s]]), call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_pend]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] lt<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), read<ptr<i8>>(%[[VALUE_pend]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE2]]));
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_type]], read<i8>(deref(read<ptr<i8>>(%[[VALUE1]]))));
// DEFAULT-NEXT:                 switch %[[VALUE3:[0-9]+]] widen<i32, reason=promotion>(read<i8>(%[[VALUE_type]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %[[VALUE3]] const<i32>(115):
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_integer_size]], const<i32>(2));
// DEFAULT-NEXT:                         goto %[[VALUE_unpack_integer:[0-9]+]];
// DEFAULT-NEXT:                         case %[[VALUE3]] const<i32>(108):
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_integer_size]], const<i32>(4));
// DEFAULT-NEXT:                         goto %[[VALUE_unpack_integer]];
// DEFAULT-NEXT:                         label %[[VALUE_unpack_integer]] unpack_integer:
// DEFAULT-NEXT:                             switch %[[VALUE4:[0-9]+]] read<i32>(%[[VALUE_integer_size]])
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     case %[[VALUE4]] const<i32>(2):
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             let %[[VALUE_v:[0-9]+]] v: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:                                             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(field1(%[[VALUE_v]]))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), const<u64>(2));
// DEFAULT-NEXT:                                             let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_s]]);
// DEFAULT-NEXT:                                             let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE5]]), const<u64>(2));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(%[[VALUE6]]));
// DEFAULT-NEXT:                                             call<void, signature=fn(i32) -> void>(%[[VALUE_do_something]], widen<i32, reason=arg>(read<i16>(field0(%[[VALUE_v]]))));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                     break %[[VALUE4]];
// DEFAULT-NEXT:                                     case %[[VALUE4]] const<i32>(4):
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             let %[[VALUE_v_2:[0-9]+]] v: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:                                             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(field1(%[[VALUE_v_2]]))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), const<u64>(4));
// DEFAULT-NEXT:                                             let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_s]]);
// DEFAULT-NEXT:                                             let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE7]]), const<u64>(4));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(%[[VALUE8]]));
// DEFAULT-NEXT:                                             call<void, signature=fn(i32) -> void>(%[[VALUE_do_something]], read<i32>(field0(%[[VALUE_v_2]])));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                     break %[[VALUE4]];
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                         break %[[VALUE3]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_s]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, ptr<i8>) -> i32>(%[[VALUE_pack_unpack]], array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]]), array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
