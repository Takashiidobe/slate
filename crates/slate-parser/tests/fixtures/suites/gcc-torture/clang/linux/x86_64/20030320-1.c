// SLATE-FILECHECK-DEFINES DEFAULT

/* Failed on powerpc64-linux with a segfault due to ifcvt generating
   conditional returns without updating dominance info.
   Extracted from glibc's dl-load.c.  */

typedef __SIZE_TYPE__ size_t;

static size_t
is_dst (const char *start, const char *name, const char *str,
        int is_path, int secure)
{
  size_t len;
  _Bool is_curly = 0;

  if (name[0] == '{')
    {
      is_curly = 1;
      ++name;
    }

  len = 0;
  while (name[len] == str[len] && name[len] != '\0')
    ++len;

  if (is_curly)
    {
      if (name[len] != '}')
        return 0;


      --name;

      len += 2;
    }
  else if (name[len] != '\0' && name[len] != '/'
           && (!is_path || name[len] != ':'))
    return 0;

  if (__builtin_expect (secure, 0)
      && ((name[len] != '\0' && (!is_path || name[len] != ':'))
          || (name != start + 1 && (!is_path || name[-2] != ':'))))
    return 0;

  return len;
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
// DEFAULT-NEXT:     fn %12 @__builtin_expect(%10 <unnamed>: i64, %11 <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %1 @is_dst(%2 start: ptr<const i8>, %3 name: ptr<const i8>, %4 str: ptr<const i8>, %5 is_path: i32, %6 secure: i32) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 len: u64 [storage=automatic];
// DEFAULT-NEXT:         let %8 is_curly: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), const<i32>(0))))), const<i32>(123))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<bool>(%8, ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:                 let %13: ptr<const i8> [synthetic] = read<ptr<const i8>>(%3);
// DEFAULT-NEXT:                 let %14: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%3, read<ptr<const i8>>(%14));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u64>(%7, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         while %9 logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), read<u64>(%7))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%4), read<u64>(%7)))))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), read<u64>(%7))))), const<i32>(0)))
// DEFAULT-NEXT:             let %15: u64 [synthetic] = read<u64>(%7);
// DEFAULT-NEXT:             let %16: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             write<u64>(%7, read<u64>(%16));
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), read<u64>(%7))))), const<i32>(125))
// DEFAULT-NEXT:                     return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(0)));
// DEFAULT-NEXT:                 let %17: ptr<const i8> [synthetic] = read<ptr<const i8>>(%3);
// DEFAULT-NEXT:                 let %18: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=true, element=i8, overflow=ub>(read<ptr<const i8>>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%3, read<ptr<const i8>>(%18));
// DEFAULT-NEXT:                 let %19: u64 [synthetic] = read<u64>(%7);
// DEFAULT-NEXT:                 let %20: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%19), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:                 write<u64>(%7, read<u64>(%20));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), read<u64>(%7))))), const<i32>(0)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), read<u64>(%7))))), const<i32>(47))), logical_or<bool>(not<bool>(ne<i32>(read<i32>(%5), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), read<u64>(%7))))), const<i32>(58))))
// DEFAULT-NEXT:                 return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(0)));
// DEFAULT-NEXT:         if logical_and<bool>(ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%12, widen<i64, reason=arg>(read<i32>(%6)), widen<i64, reason=arg>(const<i32>(0))), const<i64>(0)), logical_or<bool>(logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), read<u64>(%7))))), const<i32>(0)), logical_or<bool>(not<bool>(ne<i32>(read<i32>(%5), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), read<u64>(%7))))), const<i32>(58)))), logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%3), ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%2), const<i32>(1))), logical_or<bool>(not<bool>(ne<i32>(read<i32>(%5), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%3), neg<i32, overflow=ub>(const<i32>(2)))))), const<i32>(58))))))
// DEFAULT-NEXT:             return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(0)));
// DEFAULT-NEXT:         return read<u64>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
