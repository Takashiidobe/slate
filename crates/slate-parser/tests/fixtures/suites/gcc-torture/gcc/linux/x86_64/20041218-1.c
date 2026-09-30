/* PR rtl-optimization/16968 */
/* Testcase by Jakub Jelinek  <jakub@redhat.com> */

struct T {
  unsigned int  b, c, *d;
  unsigned char e;
};
struct S {
  unsigned int a;
  struct T     f;
};
struct U {
  struct S g, h;
};
struct V {
  unsigned int i;
  struct U     j;
};

extern void exit(int);
extern void abort(void);

void *dummy1(void *x) { return ""; }

void *dummy2(void *x, void *y) { exit(0); }

struct V *baz(unsigned int x) {
  static struct V v;
  __builtin_memset(&v, 0x55, sizeof(v));
  return &v;
}

int check(void *x, struct S *y) {
  if (y->a || y->f.b || y->f.c || y->f.d || y->f.e)
    abort();
  return 1;
}

static struct V *bar(unsigned int x, void *y) {
  const struct T t = {0, 0, (void *)0, 0};
  struct V      *u;
  void          *v;
  v = dummy1(y);
  if (!v)
    return (void *)0;

  u        = baz(sizeof(struct V));
  u->i     = x;
  u->j.g.a = 0;
  u->j.g.f = t;
  u->j.h.a = 0;
  u->j.h.f = t;

  if (!check(v, &u->j.g) || !check(v, &u->j.h))
    return (void *)0;
  return u;
}

int foo(unsigned int *x, unsigned int y, void **z) {
  void        *v;
  unsigned int i, j;

  *z = v = (void *)0;

  for (i = 0; i < y; i++) {
    struct V *c;

    j = *x;

    switch (j) {
    case 1:
      c = bar(j, x);
      break;
    default:
      c = 0;
      break;
    }
    if (c)
      v = dummy2(v, c);
    else
      return 1;
  }

  *z = v;
  return 0;
}

int main(void) {
  unsigned int one = 1;
  void        *p;
  foo(&one, 1, &p);
  abort();
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
// DEFAULT-NEXT:         field0 b: u32;
// DEFAULT-NEXT:         field1 c: u32;
// DEFAULT-NEXT:         field2 d: ptr<u32>;
// DEFAULT-NEXT:         field3 e: u8;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 f: @type[[TYPE_T]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = struct {
// DEFAULT-NEXT:         field0 g: @type[[TYPE_S]];
// DEFAULT-NEXT:         field1 h: @type[[TYPE_S]];
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 32]];
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = struct {
// DEFAULT-NEXT:         field0 i: u32;
// DEFAULT-NEXT:         field1 j: @type[[TYPE_U]];
// DEFAULT-NEXT:     } [size=72, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: @type[[TYPE_V]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_dummy1:[0-9]+]] @dummy1(%[[VALUE_x:[0-9]+]] x: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dummy2:[0-9]+]] @dummy2(%[[VALUE_x_2:[0-9]+]] x: ptr<void>, %[[VALUE_y:[0-9]+]] y: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: u32) -> ptr<@type[[TYPE_V]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_V]]>>(%[[VALUE_v]])), const<i32>(85), const<u64>(72));
// DEFAULT-NEXT:         return addr_of<ptr<@type[[TYPE_V]]>>(%[[VALUE_v]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check:[0-9]+]] @check(%[[VALUE_x_4:[0-9]+]] x: ptr<void>, %[[VALUE_y_2:[0-9]+]] y: ptr<@type[[TYPE_S]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]])))), const<u32>(0)), ne<u32>(read<u32>(field0(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]]))))), const<u32>(0))), ne<u32>(read<u32>(field1(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]]))))), const<u32>(0))), ne<ptr<u32>>(read<ptr<u32>>(field2(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]]))))), null<ptr<u32>>)), ne<u8>(read<u8>(field3(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]]))))), const<u8>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_5:[0-9]+]] x: u32, %[[VALUE_y_3:[0-9]+]] y: ptr<void>) -> ptr<@type[[TYPE_V]]> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_T]] [storage=automatic] [const] = aggregate<@type[[TYPE_T]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field2 = null<ptr<u32>>, field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: ptr<@type[[TYPE_V]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_v_2]], call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%[[VALUE_dummy1]], read<ptr<void>>(%[[VALUE_y_3]])));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_v_2]]), null<ptr<void>>))
// DEFAULT-NEXT:             return null<ptr<@type[[TYPE_V]]>>;
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_V]]>>(%[[VALUE_u]], call<ptr<@type[[TYPE_V]]>, signature=fn(u32) -> ptr<@type[[TYPE_V]]>>(%[[VALUE_baz]], truncate<u32, reason=arg, fits=always>(const<u64>(72))));
// DEFAULT-NEXT:         write<u32>(field0(deref(read<ptr<@type[[TYPE_V]]>>(%[[VALUE_u]]))), read<u32>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:         write<u32>(field0(field0(field1(deref(read<ptr<@type[[TYPE_V]]>>(%[[VALUE_u]]))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field1(field0(field1(deref(read<ptr<@type[[TYPE_V]]>>(%[[VALUE_u]]))))), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_t]])));
// DEFAULT-NEXT:         write<u32>(field0(field1(field1(deref(read<ptr<@type[[TYPE_V]]>>(%[[VALUE_u]]))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field1(field1(field1(deref(read<ptr<@type[[TYPE_V]]>>(%[[VALUE_u]]))))), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_t]])));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<void>, ptr<@type[[TYPE_S]]>) -> i32>(%[[VALUE_check]], read<ptr<void>>(%[[VALUE_v_2]]), addr_of<ptr<@type[[TYPE_S]]>>(field0(field1(deref(read<ptr<@type[[TYPE_V]]>>(%[[VALUE_u]])))))), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], not<bool>(ne<i32>(call<i32, signature=fn(ptr<void>, ptr<@type[[TYPE_S]]>) -> i32>(%[[VALUE_check]], read<ptr<void>>(%[[VALUE_v_2]]), addr_of<ptr<@type[[TYPE_S]]>>(field1(field1(deref(read<ptr<@type[[TYPE_V]]>>(%[[VALUE_u]])))))), const<i32>(0))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             return null<ptr<@type[[TYPE_V]]>>;
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_V]]>>(%[[VALUE_u]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_6:[0-9]+]] x: ptr<u32>, %[[VALUE_y_4:[0-9]+]] y: u32, %[[VALUE_z:[0-9]+]] z: ptr<ptr<void>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v_3:[0-9]+]] v: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: u32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_v_3]], null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_z]])), null<ptr<void>>);
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i]]), read<u32>(%[[VALUE_y_4]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE6]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_V]]> [storage=automatic];
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_j]], read<u32>(deref(read<ptr<u32>>(%[[VALUE_x_6]]))));
// DEFAULT-NEXT:                     switch %[[VALUE8:[0-9]+]] read<u32>(%[[VALUE_j]])
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE8]] const<u32>(1):
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_V]]>>(%[[VALUE_c]], call<ptr<@type[[TYPE_V]]>, signature=fn(u32, ptr<void>) -> ptr<@type[[TYPE_V]]>>(%[[VALUE_bar]], read<u32>(%[[VALUE_j]]), pointer_cast<ptr<void>, reason=arg>(read<ptr<u32>>(%[[VALUE_x_6]]))));
// DEFAULT-NEXT:                             break %[[VALUE8]];
// DEFAULT-NEXT:                             default %[[VALUE8]]:
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_V]]>>(%[[VALUE_c]], null<ptr<@type[[TYPE_V]]>>);
// DEFAULT-NEXT:                             break %[[VALUE8]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     if ne<ptr<@type[[TYPE_V]]>>(read<ptr<@type[[TYPE_V]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_V]]>>)
// DEFAULT-NEXT:                         write<ptr<void>>(%[[VALUE_v_3]], call<ptr<void>, signature=fn(ptr<void>, ptr<void>) -> ptr<void>>(%[[VALUE_dummy2]], read<ptr<void>>(%[[VALUE_v_3]]), pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_V]]>>(%[[VALUE_c]]))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         return const<i32>(1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_z]])), read<ptr<void>>(%[[VALUE_v_3]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_one:[0-9]+]] one: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u32>, u32, ptr<ptr<void>>) -> i32>(%[[VALUE_foo]], addr_of<ptr<u32>>(%[[VALUE_one]]), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), addr_of<ptr<ptr<void>>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
