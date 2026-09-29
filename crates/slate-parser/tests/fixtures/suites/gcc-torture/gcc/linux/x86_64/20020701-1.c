// SLATE-FILECHECK-DEFINES DEFAULT

/* PR target/7177
   Problem with cris-axis-elf: ICE in global.
   Origin: hp@axis.com.  */

typedef __SIZE_TYPE__ size_t;
void f1 (void *);
char *f2 (const char *);
int atoi (const char *);
char *strchr (const char *, int);
int strcmp (const char *, const char *);
size_t strlen (const char *);
typedef enum { A, B, C } t1;
extern const char _v[];

static t1
f (const char* p1, const char* p2, char p3)
{
  char *v1;
  char *v2;
  char *a;
  char *v3;
  char *v4;
  char *v5;
  char *e;
  char *v6;
  t1 r = C;

  v1 = f2 (p2);
  v4 = f2 (p1);

  a = v2 = v1;
  e = v5 = v4;
  __builtin_memcpy (&e, &e, sizeof (e));

  v3 = strchr (v2, ',');
  v6 = strchr (v5, ',');

  while ((_v + 1)[(unsigned) *a] & 4)
    a++;
  while ((_v + 1)[(unsigned) *e] & 4)
    e++;

  if (a == v3 && e == v6)
    {
      if (p3)
        r = atoi (v5) < atoi (v2) ? B : A;
      else
        r = atoi (v5) > atoi (v2) ? B : A;
      v2 = ++a;
      v5 = ++e;
      v3 = strchr (v2, ',');
      v6 = strchr (v5, ',');

      while ((_v + 1)[(unsigned) *a] & 4)
        a++;
      while ((_v + 1)[(unsigned) *e] & 4)
        e++;

      if (a == v3 && e == v6)
        {
          if (r == B)
            r = B;
          else if (p3)
            r = atoi (v5) < atoi (v2) ? B : A;
          else
            r = atoi (v5) > atoi (v2) ? B : A;
        }
      else
        r = C;
    }

  f1 (v1);
  f1 (v4);
  return r;
}

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_B:[0-9]+]] B = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_C:[0-9]+]] C = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_t1:[0-9]+]] t1 = @type[[TYPE0]];
// DEFAULT-NEXT:     extern %[[VALUE__v:[0-9]+]] _v: array<i8, incomplete> [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_B]] @f1(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_C]] @f2(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atoi:[0-9]+]] @atoi(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strchr:[0-9]+]] @strchr(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE4:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE5:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE6:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE7:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE8:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE9:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE10:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_p1:[0-9]+]] p1: ptr<const i8>, %[[VALUE_p2:[0-9]+]] p2: ptr<const i8>, %[[VALUE_p3:[0-9]+]] p3: i8) -> @type[[TYPE0]] [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v1:[0-9]+]] v1: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v2:[0-9]+]] v2: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v3:[0-9]+]] v3: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v4:[0-9]+]] v4: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v5:[0-9]+]] v5: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v6:[0-9]+]] v6: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE0]] [storage=automatic] = int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_v1]], call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_C]], read<ptr<const i8>>(%[[VALUE_p2]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_C]], read<ptr<const i8>>(%[[VALUE_p2]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_v4]], call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_C]], read<ptr<const i8>>(%[[VALUE_p1]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_C]], read<ptr<const i8>>(%[[VALUE_p1]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_v2]], read<ptr<i8>>(%[[VALUE_v1]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_a]], read<ptr<i8>>(%[[VALUE_v1]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_v5]], read<ptr<i8>>(%[[VALUE_v4]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_e]], read<ptr<i8>>(%[[VALUE_v4]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i8>>>(%[[VALUE_e]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<ptr<i8>>>(%[[VALUE_e]])), const<u64>(8));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_v3]], call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])), const<i32>(44)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])), const<i32>(44));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_v6]], call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]])), const<i32>(44)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]])), const<i32>(44));
// DEFAULT-NEXT:         while %[[VALUE11:[0-9]+]] ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=None>(%[[VALUE__v]]), const<i32>(1)), reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_a]]))))))))), const<i32>(4)), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_a]]);
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_a]], read<ptr<i8>>(%[[VALUE13]]));
// DEFAULT-NEXT:         while %[[VALUE14:[0-9]+]] ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=None>(%[[VALUE__v]]), const<i32>(1)), reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_e]]))))))))), const<i32>(4)), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE15:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_e]]);
// DEFAULT-NEXT:             let %[[VALUE16:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_e]], read<ptr<i8>>(%[[VALUE16]]));
// DEFAULT-NEXT:         if logical_and<bool>(eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_a]]), read<ptr<i8>>(%[[VALUE_v3]])), eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_e]]), read<ptr<i8>>(%[[VALUE_v6]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(%[[VALUE_p3]]), const<i8>(0))
// DEFAULT-NEXT:                     write<@type[[TYPE0]]>(%[[VALUE_r]], int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(lt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]]))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])))), const<i32>(1), const<i32>(0)))));
// DEFAULT-NEXT:                     int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(lt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]]))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<@type[[TYPE0]]>(%[[VALUE_r]], int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(gt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]]))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])))), const<i32>(1), const<i32>(0)))));
// DEFAULT-NEXT:                     int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(gt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]]))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_a]]);
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_a]], read<ptr<i8>>(%[[VALUE18]]));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_v2]], read<ptr<i8>>(%[[VALUE18]]));
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_e]], read<ptr<i8>>(%[[VALUE20]]));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_v5]], read<ptr<i8>>(%[[VALUE20]]));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_v3]], call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])), const<i32>(44)));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])), const<i32>(44));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_v6]], call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]])), const<i32>(44)));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]])), const<i32>(44));
// DEFAULT-NEXT:                 while %[[VALUE21:[0-9]+]] ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=None>(%[[VALUE__v]]), const<i32>(1)), reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_a]]))))))))), const<i32>(4)), const<i32>(0))
// DEFAULT-NEXT:                     let %[[VALUE22:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_a]]);
// DEFAULT-NEXT:                     let %[[VALUE23:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_a]], read<ptr<i8>>(%[[VALUE23]]));
// DEFAULT-NEXT:                 while %[[VALUE24:[0-9]+]] ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=None>(%[[VALUE__v]]), const<i32>(1)), reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_e]]))))))))), const<i32>(4)), const<i32>(0))
// DEFAULT-NEXT:                     let %[[VALUE25:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_e]]);
// DEFAULT-NEXT:                     let %[[VALUE26:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE25]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_e]], read<ptr<i8>>(%[[VALUE26]]));
// DEFAULT-NEXT:                 if logical_and<bool>(eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_a]]), read<ptr<i8>>(%[[VALUE_v3]])), eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_e]]), read<ptr<i8>>(%[[VALUE_v6]])))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_r]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             write<@type[[TYPE0]]>(%[[VALUE_r]], int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if ne<i8>(read<i8>(%[[VALUE_p3]]), const<i8>(0))
// DEFAULT-NEXT:                                 write<@type[[TYPE0]]>(%[[VALUE_r]], int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(lt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]]))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])))), const<i32>(1), const<i32>(0)))));
// DEFAULT-NEXT:                                 int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(lt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]]))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<@type[[TYPE0]]>(%[[VALUE_r]], int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(gt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]]))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])))), const<i32>(1), const<i32>(0)))));
// DEFAULT-NEXT:                                 int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(gt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v5]]))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v2]])))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<@type[[TYPE0]]>(%[[VALUE_r]], int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_B]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_v1]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_B]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_v4]])));
// DEFAULT-NEXT:         return read<@type[[TYPE0]]>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
