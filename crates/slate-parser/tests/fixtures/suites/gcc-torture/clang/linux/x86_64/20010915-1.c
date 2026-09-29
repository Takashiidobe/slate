/* Bug in reorg.c, deleting the "++" in the last loop in main.
   Origin: <hp@axis.com>.  */

void abort(void);
void exit(int);

extern void  f(void);
extern int   x(int, char **);
extern int   r(const char *);
extern char *s(char *, char **);
extern char *m(char *);
char        *u;
char        *h;
int          check = 0;
int          o     = 0;

int main(int argc, char **argv) {
  char *args[] = {"a", "b", "c", "d", "e"};
  if (x(5, args) != 0 || check != 2 || o != 5)
    abort();
  exit(0);
}

int x(int argc, char **argv) {
  int   opt = 0;
  char *g   = 0;
  char *p   = 0;

  if (argc > o && argc > 2 && argv[o]) {
    g = s(argv[o], &p);
    if (g) {
      *g++ = '\0';
      h    = s(g, &p);
      if (g == p)
        h = m(g);
    }
    u = s(argv[o], &p);
    if (argv[o] == p)
      u = m(argv[o]);
  } else
    abort();

  while (++o < argc)
    if (r(argv[o]) == 0)
      return 1;

  return 0;
}

char *m(char *x) { abort(); }
char *s(char *v, char **pp) {
  if (__builtin_strcmp(v, "a") != 0 || check++ > 1)
    abort();
  *pp = v + 1;
  return 0;
}

int r(const char *f) {
  static char c[2] = "b";
  static int  cnt  = 0;

  if (*f != *c || f[1] != c[1] || cnt > 3)
    abort();
  c[0]++;
  cnt++;
  return 1;
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
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_check:[0-9]+]] check: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_cnt:[0-9]+]] cnt: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_x:[0-9]+]] @x(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_opt:[0-9]+]] opt: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_argc]]), read<i32>(%[[VALUE_o]])), gt<i32>(read<i32>(%[[VALUE_argc]]), const<i32>(2))), ne<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), read<i32>(%[[VALUE_o]])))), null<ptr<i8>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_g]], call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%[[VALUE_s:[0-9]+]], read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), read<i32>(%[[VALUE_o]])))), addr_of<ptr<ptr<i8>>>(%[[VALUE_p]])));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), read<i32>(%[[VALUE_o]])))), addr_of<ptr<ptr<i8>>>(%[[VALUE_p]]));
// DEFAULT-NEXT:                 if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_g]]), null<ptr<i8>>)
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_g]]);
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<i8>>(%[[VALUE_g]], read<ptr<i8>>(%[[VALUE2]]));
// DEFAULT-NEXT:                         write<i8>(deref(read<ptr<i8>>(%[[VALUE1]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         write<ptr<i8>>(%[[VALUE_h]], call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(%[[VALUE_g]]), addr_of<ptr<ptr<i8>>>(%[[VALUE_p]])));
// DEFAULT-NEXT:                         call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(%[[VALUE_g]]), addr_of<ptr<ptr<i8>>>(%[[VALUE_p]]));
// DEFAULT-NEXT:                         if eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_g]]), read<ptr<i8>>(%[[VALUE_p]]))
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_h]], call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%[[VALUE_m:[0-9]+]], read<ptr<i8>>(%[[VALUE_g]])));
// DEFAULT-NEXT:                             call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%[[VALUE_m]], read<ptr<i8>>(%[[VALUE_g]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_u]], call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), read<i32>(%[[VALUE_o]])))), addr_of<ptr<ptr<i8>>>(%[[VALUE_p]])));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%[[VALUE_s]], read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), read<i32>(%[[VALUE_o]])))), addr_of<ptr<ptr<i8>>>(%[[VALUE_p]]));
// DEFAULT-NEXT:                 if eq<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), read<i32>(%[[VALUE_o]])))), read<ptr<i8>>(%[[VALUE_p]]))
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_u]], call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%[[VALUE_m]], read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), read<i32>(%[[VALUE_o]]))))));
// DEFAULT-NEXT:                     call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%[[VALUE_m]], read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), read<i32>(%[[VALUE_o]])))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         while %[[VALUE3:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_o]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_o]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:             yield lt<i32>(read<i32>(%[[VALUE5]]), read<i32>(%[[VALUE_argc]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             if eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_r:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), read<i32>(%[[VALUE_o]])))))), const<i32>(0))
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_r]] @r(%[[VALUE_f_2:[0-9]+]] f: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_f_2]])))), widen<i32, reason=promotion>(read<i8>(deref(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_c]]))))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_f_2]]), const<i32>(1))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_c]]), const<i32>(1))))))), gt<i32>(read<i32>(%[[VALUE_cnt]]), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_c]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%[[VALUE6]])));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE7]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE6]])), read<i8>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_cnt]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_cnt]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_s]] @s(%[[VALUE_v:[0-9]+]] v: ptr<i8>, %[[VALUE_pp:[0-9]+]] pp: ptr<ptr<i8>>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE___builtin_strcmp:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_v]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_6]]))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_check]]);
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_check]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], gt<i32>(read<i32>(%[[VALUE12]]), const<i32>(1)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE11]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%[[VALUE_pp]])), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_v]]), const<i32>(1)));
// DEFAULT-NEXT:         return null<ptr<i8>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_m]] @m(%[[VALUE_x_2:[0-9]+]] x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc_2:[0-9]+]] argc: i32, %[[VALUE_argv_2:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_args:[0-9]+]] args: array<ptr<i8>, 5> [storage=automatic] [align=16] = aggregate<array<ptr<i8>, 5>, zero_fill=false>(index0 = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]]), index1 = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]]), index2 = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]]), index3 = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_4]]), index4 = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_5]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(call<i32, signature=fn(i32, ptr<ptr<i8>>) -> i32>(%[[VALUE_x]], const<i32>(5), array_decay<ptr<ptr<i8>>, length=Some(5)>(%[[VALUE_args]])), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_check]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_o]]), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcmp]] @__builtin_strcmp(%[[VALUE14:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE15:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
