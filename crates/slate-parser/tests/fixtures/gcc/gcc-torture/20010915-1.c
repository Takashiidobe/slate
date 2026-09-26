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
// DEFAULT-NEXT:     global %7 u: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 h: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 check: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %10 o: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 c: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 cnt: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%26 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @x(%15 argc: i32, %16 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 opt: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %18 g: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         let %19 p: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(gt<i32>(read<i32>(%15), read<i32>(%10)), gt<i32>(read<i32>(%15), const<i32>(2))), ne<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%16), read<i32>(%10)))), null<ptr<i8>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i8>>(%18, call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%5, read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%16), read<i32>(%10)))), addr_of<ptr<ptr<i8>>>(%19)));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%5, read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%16), read<i32>(%10)))), addr_of<ptr<ptr<i8>>>(%19));
// DEFAULT-NEXT:                 if ne<ptr<i8>>(read<ptr<i8>>(%18), null<ptr<i8>>)
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %40: ptr<i8> [synthetic] = read<ptr<i8>>(%18);
// DEFAULT-NEXT:                         let %41: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%40), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<i8>>(%18, read<ptr<i8>>(%41));
// DEFAULT-NEXT:                         write<i8>(deref(read<ptr<i8>>(%40)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         write<ptr<i8>>(%8, call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%5, read<ptr<i8>>(%18), addr_of<ptr<ptr<i8>>>(%19)));
// DEFAULT-NEXT:                         call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%5, read<ptr<i8>>(%18), addr_of<ptr<ptr<i8>>>(%19));
// DEFAULT-NEXT:                         if eq<ptr<i8>>(read<ptr<i8>>(%18), read<ptr<i8>>(%19))
// DEFAULT-NEXT:                             write<ptr<i8>>(%8, call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%6, read<ptr<i8>>(%18)));
// DEFAULT-NEXT:                             call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%6, read<ptr<i8>>(%18));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<ptr<i8>>(%7, call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%5, read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%16), read<i32>(%10)))), addr_of<ptr<ptr<i8>>>(%19)));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> ptr<i8>>(%5, read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%16), read<i32>(%10)))), addr_of<ptr<ptr<i8>>>(%19));
// DEFAULT-NEXT:                 if eq<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%16), read<i32>(%10)))), read<ptr<i8>>(%19))
// DEFAULT-NEXT:                     write<ptr<i8>>(%7, call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%6, read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%16), read<i32>(%10))))));
// DEFAULT-NEXT:                     call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%6, read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%16), read<i32>(%10)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         while %38 {
// DEFAULT-NEXT:             let %42: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:             let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%10, read<i32>(%43));
// DEFAULT-NEXT:             yield lt<i32>(read<i32>(%43), read<i32>(%15));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             if eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%16), read<i32>(%10)))))), const<i32>(0))
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @r(%23 f: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%23)))), widen<i32, reason=promotion>(read<i8>(deref(array_decay<ptr<i8>, length=Some(2)>(%24))))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%23), const<i32>(1))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%24), const<i32>(1))))))), gt<i32>(read<i32>(%25), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %44: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%24), const<i32>(0));
// DEFAULT-NEXT:         let %45: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%44)));
// DEFAULT-NEXT:         let %46: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%45)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%44)), read<i8>(%46));
// DEFAULT-NEXT:         let %47: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:         let %48: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%47), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%25, read<i32>(%48));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @s(%21 v: ptr<i8>, %22 pp: ptr<ptr<i8>>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %49: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(__builtin_strcmp, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%21)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%39))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%49, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %50: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:             let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%9, read<i32>(%51));
// DEFAULT-NEXT:             write<bool>(%49, gt<i32>(read<i32>(%50), const<i32>(1)));
// DEFAULT-NEXT:         if read<bool>(%49)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%22)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%21), const<i32>(1)));
// DEFAULT-NEXT:         return null<ptr<i8>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @m(%20 x: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main(%12 argc: i32, %13 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 args: array<ptr<i8>, 5> [storage=automatic] [align=16] = aggregate<array<ptr<i8>, 5>, zero_fill=false>(index0 = array_decay<ptr<i8>, length=Some(2)>(%33), index1 = array_decay<ptr<i8>, length=Some(2)>(%34), index2 = array_decay<ptr<i8>, length=Some(2)>(%35), index3 = array_decay<ptr<i8>, length=Some(2)>(%36), index4 = array_decay<ptr<i8>, length=Some(2)>(%37));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(call<i32, signature=fn(i32, ptr<ptr<i8>>) -> i32>(%3, const<i32>(5), array_decay<ptr<ptr<i8>>, length=Some(5)>(%14)), const<i32>(0)), ne<i32>(read<i32>(%9), const<i32>(2))), ne<i32>(read<i32>(%10), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
