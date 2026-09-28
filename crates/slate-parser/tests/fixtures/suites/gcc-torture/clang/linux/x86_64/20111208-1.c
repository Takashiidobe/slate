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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 int16_t = i16;
// DEFAULT-NEXT:     type @type2 int32_t = i32;
// DEFAULT-NEXT:     type @type3 = union {
// DEFAULT-NEXT:         field0 i: i16;
// DEFAULT-NEXT:         field1 a: array<i8, 2>;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type4 = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 a: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %6 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([128, 1, 255, 254, 29, 192, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([115, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @memcpy(%23 __dest: ptr<void> [restrict], %24 __src: ptr<const void> [restrict], %25 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @strlen(%26 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @do_something(%8 item: i32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @pack_unpack(%11 s: ptr<i8>, %12 p: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 send: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %14 pend: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %15 type: i8 [storage=automatic];
// DEFAULT-NEXT:         let %16 integer_size: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%13, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%11), call<u64, signature=fn(ptr<const i8>) -> u64>(%2, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%11)))));
// DEFAULT-NEXT:         write<ptr<i8>>(%14, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%12), call<u64, signature=fn(ptr<const i8>) -> u64>(%2, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%12)))));
// DEFAULT-NEXT:         while %27 lt<ptr<i8>>(read<ptr<i8>>(%12), read<ptr<i8>>(%14))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %32: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                 let %33: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%32), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%12, read<ptr<i8>>(%33));
// DEFAULT-NEXT:                 write<i8>(%15, read<i8>(deref(read<ptr<i8>>(%32))));
// DEFAULT-NEXT:                 switch %28 widen<i32, reason=promotion>(read<i8>(%15))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %28 const<i32>(115):
// DEFAULT-NEXT:                             write<i32>(%16, const<i32>(2));
// DEFAULT-NEXT:                         goto %10;
// DEFAULT-NEXT:                         case %28 const<i32>(108):
// DEFAULT-NEXT:                             write<i32>(%16, const<i32>(4));
// DEFAULT-NEXT:                         goto %10;
// DEFAULT-NEXT:                         label %10 unpack_integer:
// DEFAULT-NEXT:                             switch %29 read<i32>(%16)
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     case %29 const<i32>(2):
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             let %18 v: @type3 [storage=automatic];
// DEFAULT-NEXT:                                             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(field1(%18))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%11)), const<u64>(2));
// DEFAULT-NEXT:                                             let %34: ptr<i8> [synthetic] = read<ptr<i8>>(%11);
// DEFAULT-NEXT:                                             let %35: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%34), const<u64>(2));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%11, read<ptr<i8>>(%35));
// DEFAULT-NEXT:                                             call<void, signature=fn(i32) -> void>(%7, widen<i32, reason=arg>(read<i16>(field0(%18))));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                     break %29;
// DEFAULT-NEXT:                                     case %29 const<i32>(4):
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             let %20 v: @type4 [storage=automatic];
// DEFAULT-NEXT:                                             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(field1(%20))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%11)), const<u64>(4));
// DEFAULT-NEXT:                                             let %36: ptr<i8> [synthetic] = read<ptr<i8>>(%11);
// DEFAULT-NEXT:                                             let %37: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%36), const<u64>(4));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%11, read<ptr<i8>>(%37));
// DEFAULT-NEXT:                                             call<void, signature=fn(i32) -> void>(%7, read<i32>(field0(%20)));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                     break %29;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                         break %28;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %22 n: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, ptr<i8>) -> i32>(%9, array_decay<ptr<i8>, length=Some(7)>(%30), array_decay<ptr<i8>, length=Some(3)>(%31));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%22), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
