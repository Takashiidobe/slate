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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = enum : u32 {
// DEFAULT-NEXT:         %0 A = const<i32>(0);
// DEFAULT-NEXT:         %1 B = const<i32>(1);
// DEFAULT-NEXT:         %2 C = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type2 t1 = @type1;
// DEFAULT-NEXT:     extern %12 _v: array<i8, incomplete> [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     fn %1 @f1(%26 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f2(%27 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %3 @atoi(%28 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @strchr(%29 <unnamed>: ptr<const i8>, %30 <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %5 @strcmp(%31 <unnamed>: ptr<const i8>, %32 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @strlen(%33 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %13 @f(%14 p1: ptr<const i8>, %15 p2: ptr<const i8>, %16 p3: i8) -> @type1 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 v1: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %18 v2: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %19 a: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %20 v3: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %21 v4: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %22 v5: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %23 e: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %24 v6: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %25 r: @type1 [storage=automatic] = int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<ptr<i8>>(%17, call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%2, read<ptr<const i8>>(%15)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%2, read<ptr<const i8>>(%15));
// DEFAULT-NEXT:         write<ptr<i8>>(%21, call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%2, read<ptr<const i8>>(%14)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%2, read<ptr<const i8>>(%14));
// DEFAULT-NEXT:         write<ptr<i8>>(%18, read<ptr<i8>>(%17));
// DEFAULT-NEXT:         write<ptr<i8>>(%19, read<ptr<i8>>(%17));
// DEFAULT-NEXT:         write<ptr<i8>>(%22, read<ptr<i8>>(%21));
// DEFAULT-NEXT:         write<ptr<i8>>(%23, read<ptr<i8>>(%21));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i8>>>(%23)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<ptr<i8>>>(%23)), const<u64>(8));
// DEFAULT-NEXT:         write<ptr<i8>>(%20, call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)), const<i32>(44)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)), const<i32>(44));
// DEFAULT-NEXT:         write<ptr<i8>>(%24, call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22)), const<i32>(44)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22)), const<i32>(44));
// DEFAULT-NEXT:         while %34 ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=None>(%12), const<i32>(1)), reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%19))))))))), const<i32>(4)), const<i32>(0))
// DEFAULT-NEXT:             let %38: ptr<i8> [synthetic] = read<ptr<i8>>(%19);
// DEFAULT-NEXT:             let %39: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%38), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%19, read<ptr<i8>>(%39));
// DEFAULT-NEXT:         while %35 ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=None>(%12), const<i32>(1)), reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%23))))))))), const<i32>(4)), const<i32>(0))
// DEFAULT-NEXT:             let %40: ptr<i8> [synthetic] = read<ptr<i8>>(%23);
// DEFAULT-NEXT:             let %41: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%40), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%23, read<ptr<i8>>(%41));
// DEFAULT-NEXT:         if logical_and<bool>(eq<ptr<i8>>(read<ptr<i8>>(%19), read<ptr<i8>>(%20)), eq<ptr<i8>>(read<ptr<i8>>(%23), read<ptr<i8>>(%24)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(%16), const<i8>(0))
// DEFAULT-NEXT:                     write<@type1>(%25, int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(lt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22))), call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)))), const<i32>(1), const<i32>(0)))));
// DEFAULT-NEXT:                     int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(lt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22))), call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<@type1>(%25, int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(gt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22))), call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)))), const<i32>(1), const<i32>(0)))));
// DEFAULT-NEXT:                     int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(gt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22))), call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:                 let %42: ptr<i8> [synthetic] = read<ptr<i8>>(%19);
// DEFAULT-NEXT:                 let %43: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%42), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%19, read<ptr<i8>>(%43));
// DEFAULT-NEXT:                 write<ptr<i8>>(%18, read<ptr<i8>>(%43));
// DEFAULT-NEXT:                 let %44: ptr<i8> [synthetic] = read<ptr<i8>>(%23);
// DEFAULT-NEXT:                 let %45: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%44), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%23, read<ptr<i8>>(%45));
// DEFAULT-NEXT:                 write<ptr<i8>>(%22, read<ptr<i8>>(%45));
// DEFAULT-NEXT:                 write<ptr<i8>>(%20, call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)), const<i32>(44)));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)), const<i32>(44));
// DEFAULT-NEXT:                 write<ptr<i8>>(%24, call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22)), const<i32>(44)));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22)), const<i32>(44));
// DEFAULT-NEXT:                 while %36 ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=None>(%12), const<i32>(1)), reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%19))))))))), const<i32>(4)), const<i32>(0))
// DEFAULT-NEXT:                     let %46: ptr<i8> [synthetic] = read<ptr<i8>>(%19);
// DEFAULT-NEXT:                     let %47: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%46), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%19, read<ptr<i8>>(%47));
// DEFAULT-NEXT:                 while %37 ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=None>(%12), const<i32>(1)), reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(deref(read<ptr<i8>>(%23))))))))), const<i32>(4)), const<i32>(0))
// DEFAULT-NEXT:                     let %48: ptr<i8> [synthetic] = read<ptr<i8>>(%23);
// DEFAULT-NEXT:                     let %49: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%48), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%23, read<ptr<i8>>(%49));
// DEFAULT-NEXT:                 if logical_and<bool>(eq<ptr<i8>>(read<ptr<i8>>(%19), read<ptr<i8>>(%20)), eq<ptr<i8>>(read<ptr<i8>>(%23), read<ptr<i8>>(%24)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type1>(%25)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                             write<@type1>(%25, int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if ne<i8>(read<i8>(%16), const<i8>(0))
// DEFAULT-NEXT:                                 write<@type1>(%25, int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(lt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22))), call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)))), const<i32>(1), const<i32>(0)))));
// DEFAULT-NEXT:                                 int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(lt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22))), call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<@type1>(%25, int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(gt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22))), call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)))), const<i32>(1), const<i32>(0)))));
// DEFAULT-NEXT:                                 int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(gt<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%22))), call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)))), const<i32>(1), const<i32>(0))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<@type1>(%25, int_to_enum<@type1, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%17)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%21)));
// DEFAULT-NEXT:         return read<@type1>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
