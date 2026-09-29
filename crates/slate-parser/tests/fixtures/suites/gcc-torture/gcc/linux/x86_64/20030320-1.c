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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expect:[0-9]+]] @__builtin_expect(%[[VALUE0:[0-9]+]] <unnamed>: i64, %[[VALUE1:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_is_dst:[0-9]+]] @is_dst(%[[VALUE_start:[0-9]+]] start: ptr<const i8>, %[[VALUE_name:[0-9]+]] name: ptr<const i8>, %[[VALUE_str:[0-9]+]] str: ptr<const i8>, %[[VALUE_is_path:[0-9]+]] is_path: i32, %[[VALUE_secure:[0-9]+]] secure: i32) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_is_curly:[0-9]+]] is_curly: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), const<i32>(0))))), const<i32>(123))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<bool>(%[[VALUE_is_curly]], ne<i32, reason=assign>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_name]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%[[VALUE_name]], read<ptr<const i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u64>(%[[VALUE_len]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         while %[[VALUE4:[0-9]+]] logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), read<u64>(%[[VALUE_len]]))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_str]]), read<u64>(%[[VALUE_len]])))))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), read<u64>(%[[VALUE_len]]))))), const<i32>(0)))
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len]]);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE5]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             write<u64>(%[[VALUE_len]], read<u64>(%[[VALUE6]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE_is_curly]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), read<u64>(%[[VALUE_len]]))))), const<i32>(125))
// DEFAULT-NEXT:                     return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_name]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=true, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%[[VALUE_name]], read<ptr<const i8>>(%[[VALUE8]]));
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE9]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_len]], read<u64>(%[[VALUE10]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), read<u64>(%[[VALUE_len]]))))), const<i32>(0)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), read<u64>(%[[VALUE_len]]))))), const<i32>(47))), logical_or<bool>(not<bool>(ne<i32>(read<i32>(%[[VALUE_is_path]]), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), read<u64>(%[[VALUE_len]]))))), const<i32>(58))))
// DEFAULT-NEXT:                 return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(0)));
// DEFAULT-NEXT:         if logical_and<bool>(ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE___builtin_expect]], widen<i64, reason=arg>(read<i32>(%[[VALUE_secure]])), widen<i64, reason=arg>(const<i32>(0))), const<i64>(0)), logical_or<bool>(logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), read<u64>(%[[VALUE_len]]))))), const<i32>(0)), logical_or<bool>(not<bool>(ne<i32>(read<i32>(%[[VALUE_is_path]]), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), read<u64>(%[[VALUE_len]]))))), const<i32>(58)))), logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_name]]), ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_start]]), const<i32>(1))), logical_or<bool>(not<bool>(ne<i32>(read<i32>(%[[VALUE_is_path]]), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), neg<i32, overflow=ub>(const<i32>(2)))))), const<i32>(58))))))
// DEFAULT-NEXT:             return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(0)));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_len]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
