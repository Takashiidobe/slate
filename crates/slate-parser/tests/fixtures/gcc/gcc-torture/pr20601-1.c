/* PR tree-optimization/20601 */
extern void abort(void);
extern void exit(int);

struct T {
  char  *t1;
  char   t2[4096];
  char **t3;
};

int      a[5];
int      b;
char   **c;
int      d;
char   **e;
struct T t;
char    *f[16];
char    *g[] = {"a", "-u", "b", "c"};

__attribute__((__noreturn__)) void foo(void) {
  while (1)
    ;
}

__attribute__((noinline)) char *bar(char *x, unsigned int y) { return 0; }

static inline char *baz(char *x, unsigned int y) {
  if (sizeof(t.t2) != (unsigned int)-1 && y > sizeof(t.t2))
    foo();
  return bar(x, y);
}

static inline int setup1(int x) {
  char *p;
  int   rval;

  if (!baz(t.t2, sizeof(t.t2)))
    baz(t.t2, sizeof(t.t2));

  if (x & 0x200) {
    char **h, **i = e;

    ++d;
    e = f;
    if (t.t1 && *t.t1)
      e[0] = t.t1;
    else
      abort();

    for (h = e + 1; (*h = *i); ++i, ++h)
      ;
  }
  return 1;
}

static inline int setup2(void) {
  int j = 1;

  e = c + 1;
  d = b - 1;
  while (d > 0 && e[0][0] == '-') {
    if (e[0][1] != '\0' && e[0][2] != '\0')
      abort();

    switch (e[0][1]) {
    case 'u':
      if (!e[1])
        abort();

      t.t3 = &e[1];
      d--;
      e++;
      break;
    case 'P':
      j |= 0x1000;
      break;
    case '-':
      d--;
      e++;
      if (j == 1)
        j |= 0x600;
      return j;
    }
    d--;
    e++;
  }

  if (d > 0 && !(j & 1))
    abort();

  return j;
}

int main(void) {
  int x;
  c    = g;
  b    = 4;
  x    = setup2();
  t.t1 = "/bin/sh";
  setup1(x);
  /* PRE shouldn't transform x into the constant 0x601 here, it's not legal.  */
  if ((x & 0x400) && !a[4])
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
// DEFAULT-NEXT:     type @type0 T = struct {
// DEFAULT-NEXT:         field0 t1: ptr<i8>;
// DEFAULT-NEXT:         field1 t2: array<i8, 4096>;
// DEFAULT-NEXT:         field2 t3: ptr<ptr<i8>>;
// DEFAULT-NEXT:     } [size=4112, align=8, offsets=[0, 8, 4104]];
// DEFAULT-NEXT:     global %3 a: array<i32, 5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 c: ptr<ptr<i8>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 e: ptr<ptr<i8>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 t: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 f: array<ptr<i8>, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([45, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 g: array<ptr<i8>, 4> [storage=static] = aggregate<array<ptr<i8>, 4>, zero_fill=false>(index0 = array_decay<ptr<i8>, length=Some(2)>(%29), index1 = array_decay<ptr<i8>, length=Some(3)>(%30), index2 = array_decay<ptr<i8>, length=Some(2)>(%31), index3 = array_decay<ptr<i8>, length=Some(2)>(%32)) [linkage=external];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([47, 98, 105, 110, 47, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%28 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @foo() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         while %33 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @bar(%13 x: ptr<i8>, %14 y: u32) -> ptr<i8> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<i8>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @baz(%16 x: ptr<i8>, %17 y: u32) -> ptr<i8> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(ne<u64>(const<u64>(4096), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), gt<u64>(widen<u64, reason=usual_arith>(read<u32>(%17)), const<u64>(4096)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, u32) -> ptr<i8>>(%12, read<ptr<i8>>(%16), read<u32>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @setup1(%19 x: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %21 rval: i32 [storage=automatic];
// DEFAULT-NEXT:         if not<bool>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, u32) -> ptr<i8>>(%15, array_decay<ptr<i8>, length=Some(4096)>(field1(%8)), truncate<u32, reason=arg, fits=always>(const<u64>(4096))), null<ptr<i8>>))
// DEFAULT-NEXT:             call<ptr<i8>, signature=fn(ptr<i8>, u32) -> ptr<i8>>(%15, array_decay<ptr<i8>, length=Some(4096)>(field1(%8)), truncate<u32, reason=arg, fits=always>(const<u64>(4096)));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(%19), const<i32>(512)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %22 h: ptr<ptr<i8>> [storage=automatic];
// DEFAULT-NEXT:                 let %23 i: ptr<ptr<i8>> [storage=automatic] = read<ptr<ptr<i8>>>(%7);
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%39));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%7, array_decay<ptr<ptr<i8>>, length=Some(16)>(%9));
// DEFAULT-NEXT:                 if logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(field0(%8)), null<ptr<i8>>), ne<i8>(read<i8>(deref(read<ptr<i8>>(field0(%8)))), const<i8>(0)))
// DEFAULT-NEXT:                     write<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%7), const<i32>(0))), read<ptr<i8>>(field0(%8)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 for %34
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%22, ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%7), const<i32>(1)));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%22)), read<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%23))));
// DEFAULT-NEXT:                         yield ne<ptr<i8>>(read<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%23))), null<ptr<i8>>);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %40: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%23);
// DEFAULT-NEXT:                         let %41: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%40), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%23, read<ptr<ptr<i8>>>(%41));
// DEFAULT-NEXT:                         let %42: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%22);
// DEFAULT-NEXT:                         let %43: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%42), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%22, read<ptr<ptr<i8>>>(%43));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @setup2() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %25 j: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         write<ptr<ptr<i8>>>(%7, ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%5), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%6, sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1)));
// DEFAULT-NEXT:         while %35 logical_and<bool>(gt<i32>(read<i32>(%6), const<i32>(0)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%7), const<i32>(0)))), const<i32>(0))))), const<i32>(45)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%7), const<i32>(0)))), const<i32>(1))))), const<i32>(0)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%7), const<i32>(0)))), const<i32>(2))))), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 switch %36 widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%7), const<i32>(0)))), const<i32>(1)))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %36 const<i32>(117):
// DEFAULT-NEXT:                             if not<bool>(ne<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%7), const<i32>(1)))), null<ptr<i8>>))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(field2(%8), addr_of<ptr<ptr<i8>>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%7), const<i32>(1)))));
// DEFAULT-NEXT:                         let %44: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                         let %45: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%6, read<i32>(%45));
// DEFAULT-NEXT:                         let %46: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%7);
// DEFAULT-NEXT:                         let %47: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%46), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%7, read<ptr<ptr<i8>>>(%47));
// DEFAULT-NEXT:                         break %36;
// DEFAULT-NEXT:                         case %36 const<i32>(80):
// DEFAULT-NEXT:                             let %48: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:                             let %49: i32 [synthetic] = or<i32>(read<i32>(%48), const<i32>(4096));
// DEFAULT-NEXT:                             write<i32>(%25, read<i32>(%49));
// DEFAULT-NEXT:                         break %36;
// DEFAULT-NEXT:                         case %36 const<i32>(45):
// DEFAULT-NEXT:                             let %50: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                             let %51: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%6, read<i32>(%51));
// DEFAULT-NEXT:                         let %52: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%7);
// DEFAULT-NEXT:                         let %53: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%52), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%7, read<ptr<ptr<i8>>>(%53));
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%25), const<i32>(1))
// DEFAULT-NEXT:                             let %54: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:                             let %55: i32 [synthetic] = or<i32>(read<i32>(%54), const<i32>(1536));
// DEFAULT-NEXT:                             write<i32>(%25, read<i32>(%55));
// DEFAULT-NEXT:                         return read<i32>(%25);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %56: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %57: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%57));
// DEFAULT-NEXT:                 let %58: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%7);
// DEFAULT-NEXT:                 let %59: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%58), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%7, read<ptr<ptr<i8>>>(%59));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%6), const<i32>(0)), not<bool>(ne<i32>(and<i32>(read<i32>(%25), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return read<i32>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %27 x: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<ptr<i8>>>(%5, array_decay<ptr<ptr<i8>>, length=Some(4)>(%10));
// DEFAULT-NEXT:         write<i32>(%4, const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%27, call<i32, signature=fn() -> i32>(%24));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%24);
// DEFAULT-NEXT:         write<ptr<i8>>(field0(%8), array_decay<ptr<i8>, length=Some(8)>(%37));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%18, read<i32>(%27));
// DEFAULT-NEXT:         if logical_and<bool>(ne<i32>(and<i32>(read<i32>(%27), const<i32>(1024)), const<i32>(0)), not<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%3), const<i32>(4)))), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
