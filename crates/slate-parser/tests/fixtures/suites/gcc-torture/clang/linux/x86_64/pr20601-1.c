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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 t1: ptr<i8>;
// DEFAULT-NEXT:         field1 t2: array<i8, 4096>;
// DEFAULT-NEXT:         field2 t3: ptr<ptr<i8>>;
// DEFAULT-NEXT:     } [size=4112, align=8, offsets=[0, 8, 4104]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 5> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<ptr<i8>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: ptr<ptr<i8>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: @type[[TYPE_T]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: array<ptr<i8>, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([45, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: array<ptr<i8>, 4> [storage=static] [align=16] = aggregate<array<ptr<i8>, 4>, zero_fill=false>(index0 = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]]), index1 = array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_2]]), index2 = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]]), index3 = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_4]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([47, 98, 105, 110, 47, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<i8>, %[[VALUE_y:[0-9]+]] y: u32) -> ptr<i8> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<i8>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_2:[0-9]+]] x: ptr<i8>, %[[VALUE_y_2:[0-9]+]] y: u32) -> ptr<i8> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(ne<u64>(const<u64>(4096), widen<u64, reason=usual_arith>(reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))), gt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_y_2]])), const<u64>(4096)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         return call<ptr<i8>, signature=fn(ptr<i8>, u32) -> ptr<i8>>(%[[VALUE_bar]], read<ptr<i8>>(%[[VALUE_x_2]]), read<u32>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_setup1:[0-9]+]] @setup1(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_rval:[0-9]+]] rval: i32 [storage=automatic];
// DEFAULT-NEXT:         if not<bool>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, u32) -> ptr<i8>>(%[[VALUE_baz]], array_decay<ptr<i8>, length=Some(4096)>(field1(%[[VALUE_t]])), truncate<u32, reason=arg, fits=always>(const<u64>(4096))), null<ptr<i8>>))
// DEFAULT-NEXT:             call<ptr<i8>, signature=fn(ptr<i8>, u32) -> ptr<i8>>(%[[VALUE_baz]], array_decay<ptr<i8>, length=Some(4096)>(field1(%[[VALUE_t]])), truncate<u32, reason=arg, fits=always>(const<u64>(4096)));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(512)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_h:[0-9]+]] h: ptr<ptr<i8>> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: ptr<ptr<i8>> [storage=automatic] = read<ptr<ptr<i8>>>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%[[VALUE_e]], array_decay<ptr<ptr<i8>>, length=Some(16)>(%[[VALUE_f]]));
// DEFAULT-NEXT:                 if logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(field0(%[[VALUE_t]])), null<ptr<i8>>), ne<i8>(read<i8>(deref(read<ptr<i8>>(field0(%[[VALUE_t]])))), const<i8>(0)))
// DEFAULT-NEXT:                     write<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_e]]), const<i32>(0))), read<ptr<i8>>(field0(%[[VALUE_t]])));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%[[VALUE_h]], ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_e]]), const<i32>(1)));
// DEFAULT-NEXT:                     condition: {
// DEFAULT-NEXT:                         write<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%[[VALUE_h]])), read<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%[[VALUE_i]]))));
// DEFAULT-NEXT:                         yield ne<ptr<i8>>(read<ptr<i8>>(deref(read<ptr<ptr<i8>>>(%[[VALUE_i]]))), null<ptr<i8>>);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%[[VALUE_i]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%[[VALUE_i]], read<ptr<ptr<i8>>>(%[[VALUE6]]));
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%[[VALUE_h]]);
// DEFAULT-NEXT:                         let %[[VALUE8:[0-9]+]]: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%[[VALUE_h]], read<ptr<ptr<i8>>>(%[[VALUE8]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_setup2:[0-9]+]] @setup2() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         write<ptr<ptr<i8>>>(%[[VALUE_e]], ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_c]]), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_b]]), const<i32>(1)));
// DEFAULT-NEXT:         while %[[VALUE9:[0-9]+]] logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_e]]), const<i32>(0)))), const<i32>(0))))), const<i32>(45)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_e]]), const<i32>(0)))), const<i32>(1))))), const<i32>(0)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_e]]), const<i32>(0)))), const<i32>(2))))), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 switch %[[VALUE10:[0-9]+]] widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_e]]), const<i32>(0)))), const<i32>(1)))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %[[VALUE10]] const<i32>(117):
// DEFAULT-NEXT:                             if not<bool>(ne<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_e]]), const<i32>(1)))), null<ptr<i8>>))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(field2(%[[VALUE_t]]), addr_of<ptr<ptr<i8>>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_e]]), const<i32>(1)))));
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%[[VALUE_e]]);
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%[[VALUE_e]], read<ptr<ptr<i8>>>(%[[VALUE14]]));
// DEFAULT-NEXT:                         break %[[VALUE10]];
// DEFAULT-NEXT:                         case %[[VALUE10]] const<i32>(80):
// DEFAULT-NEXT:                             let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE16:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE15]]), const<i32>(4096));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                         break %[[VALUE10]];
// DEFAULT-NEXT:                         case %[[VALUE10]] const<i32>(45):
// DEFAULT-NEXT:                             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:                         let %[[VALUE19:[0-9]+]]: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%[[VALUE_e]]);
// DEFAULT-NEXT:                         let %[[VALUE20:[0-9]+]]: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<ptr<i8>>>(%[[VALUE_e]], read<ptr<ptr<i8>>>(%[[VALUE20]]));
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_j]]), const<i32>(1))
// DEFAULT-NEXT:                             let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE22:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE21]]), const<i32>(1536));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:                         return read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: ptr<ptr<i8>> [synthetic] = read<ptr<ptr<i8>>>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: ptr<ptr<i8>> [synthetic] = ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE25]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<ptr<i8>>>(%[[VALUE_e]], read<ptr<ptr<i8>>>(%[[VALUE26]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0)), not<bool>(ne<i32>(and<i32>(read<i32>(%[[VALUE_j]]), const<i32>(1)), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<ptr<i8>>>(%[[VALUE_c]], array_decay<ptr<ptr<i8>>, length=Some(4)>(%[[VALUE_g]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x_4]], call<i32, signature=fn() -> i32>(%[[VALUE_setup2]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_setup2]]);
// DEFAULT-NEXT:         write<ptr<i8>>(field0(%[[VALUE_t]]), array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_5]]));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_setup1]], read<i32>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         if logical_and<bool>(ne<i32>(and<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(1024)), const<i32>(0)), not<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_a]]), const<i32>(4)))), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
