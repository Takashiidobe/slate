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
// DEFAULT-NEXT:     global %10 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([128, 1, 255, 254, 29, 192, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([115, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @memcpy(%27 __dest: ptr<void> [restrict], %28 __src: ptr<const void> [restrict], %29 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @strlen(%30 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %9 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @do_something(%12 item: i32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @pack_unpack(%15 s: ptr<i8>, %16 p: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 send: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %18 pend: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %19 type: i8 [storage=automatic];
// DEFAULT-NEXT:         let %20 integer_size: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%17, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%15), call<u64, signature=fn(ptr<const i8>) -> u64>(%6, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%15)))));
// DEFAULT-NEXT:         write<ptr<i8>>(%18, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), call<u64, signature=fn(ptr<const i8>) -> u64>(%6, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%16)))));
// DEFAULT-NEXT:         while %31 lt<ptr<i8>>(read<ptr<i8>>(%16), read<ptr<i8>>(%18))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %36: ptr<i8> [synthetic] = read<ptr<i8>>(%16);
// DEFAULT-NEXT:                 let %37: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%36), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%16, read<ptr<i8>>(%37));
// DEFAULT-NEXT:                 write<i8>(%19, read<i8>(deref(read<ptr<i8>>(%36))));
// DEFAULT-NEXT:                 switch %32 widen<i32, reason=promotion>(read<i8>(%19))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %32 const<i32>(115):
// DEFAULT-NEXT:                             write<i32>(%20, const<i32>(2));
// DEFAULT-NEXT:                         goto %14;
// DEFAULT-NEXT:                         case %32 const<i32>(108):
// DEFAULT-NEXT:                             write<i32>(%20, const<i32>(4));
// DEFAULT-NEXT:                         goto %14;
// DEFAULT-NEXT:                         label %14 unpack_integer:
// DEFAULT-NEXT:                             switch %33 read<i32>(%20)
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     case %33 const<i32>(2):
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             let %22 v: @type3 [storage=automatic];
// DEFAULT-NEXT:                                             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(field1(%22))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%15)), const<u64>(2));
// DEFAULT-NEXT:                                             let %38: ptr<i8> [synthetic] = read<ptr<i8>>(%15);
// DEFAULT-NEXT:                                             let %39: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%38), const<u64>(2));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%15, read<ptr<i8>>(%39));
// DEFAULT-NEXT:                                             call<void, signature=fn(i32) -> void>(%11, widen<i32, reason=arg>(read<i16>(field0(%22))));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                     break %33;
// DEFAULT-NEXT:                                     case %33 const<i32>(4):
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             let %24 v: @type4 [storage=automatic];
// DEFAULT-NEXT:                                             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(field1(%24))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%15)), const<u64>(4));
// DEFAULT-NEXT:                                             let %40: ptr<i8> [synthetic] = read<ptr<i8>>(%15);
// DEFAULT-NEXT:                                             let %41: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%40), const<u64>(4));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%15, read<ptr<i8>>(%41));
// DEFAULT-NEXT:                                             call<void, signature=fn(i32) -> void>(%11, read<i32>(field0(%24)));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                     break %33;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                         break %32;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %26 n: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, ptr<i8>) -> i32>(%13, array_decay<ptr<i8>, length=Some(7)>(%34), array_decay<ptr<i8>, length=Some(3)>(%35));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%26), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
