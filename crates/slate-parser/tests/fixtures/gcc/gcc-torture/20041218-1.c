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
// DEFAULT-NEXT:     type @type0 T = struct {
// DEFAULT-NEXT:         field0 b: u32;
// DEFAULT-NEXT:         field1 c: u32;
// DEFAULT-NEXT:         field2 d: ptr<u32>;
// DEFAULT-NEXT:         field3 e: u8;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 f: @type0;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 U = struct {
// DEFAULT-NEXT:         field0 g: @type1;
// DEFAULT-NEXT:         field1 h: @type1;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 32]];
// DEFAULT-NEXT:     type @type3 V = struct {
// DEFAULT-NEXT:         field0 i: u32;
// DEFAULT-NEXT:         field1 j: @type2;
// DEFAULT-NEXT:     } [size=72, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 v: @type3 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %4 @exit(%34 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @dummy1(%7 x: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(array_decay<ptr<i8>, length=Some(1)>(%35));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @dummy2(%9 x: ptr<void>, %10 y: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @baz(%12 x: u32) -> ptr<@type3> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type3>>(%13)), const<i32>(85), const<u64>(72));
// DEFAULT-NEXT:         return addr_of<ptr<@type3>>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @check(%15 x: ptr<void>, %16 y: ptr<@type1>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(field0(deref(read<ptr<@type1>>(%16)))), const<u32>(0)), ne<u32>(read<u32>(field0(field1(deref(read<ptr<@type1>>(%16))))), const<u32>(0))), ne<u32>(read<u32>(field1(field1(deref(read<ptr<@type1>>(%16))))), const<u32>(0))), ne<ptr<u32>>(read<ptr<u32>>(field2(field1(deref(read<ptr<@type1>>(%16))))), null<ptr<u32>>)), ne<u8>(read<u8>(field3(field1(deref(read<ptr<@type1>>(%16))))), const<u8>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @bar(%18 x: u32, %19 y: ptr<void>) -> ptr<@type3> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 t: @type0 [storage=automatic] [const] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field2 = null<ptr<u32>>, field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %21 u: ptr<@type3> [storage=automatic];
// DEFAULT-NEXT:         let %22 v: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%22, call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%6, read<ptr<void>>(%19)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%6, read<ptr<void>>(%19));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(read<ptr<void>>(%22), null<ptr<void>>))
// DEFAULT-NEXT:             return null<ptr<@type3>>;
// DEFAULT-NEXT:         write<ptr<@type3>>(%21, call<ptr<@type3>, signature=fn(u32) -> ptr<@type3>>(%11, truncate<u32, reason=arg, fits=always>(const<u64>(72))));
// DEFAULT-NEXT:         call<ptr<@type3>, signature=fn(u32) -> ptr<@type3>>(%11, truncate<u32, reason=arg, fits=always>(const<u64>(72)));
// DEFAULT-NEXT:         write<u32>(field0(deref(read<ptr<@type3>>(%21))), read<u32>(%18));
// DEFAULT-NEXT:         write<u32>(field0(field0(field1(deref(read<ptr<@type3>>(%21))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type0>(field1(field0(field1(deref(read<ptr<@type3>>(%21))))), copy<@type0, reason=assign>(read<@type0>(%20)));
// DEFAULT-NEXT:         write<u32>(field0(field1(field1(deref(read<ptr<@type3>>(%21))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type0>(field1(field1(field1(deref(read<ptr<@type3>>(%21))))), copy<@type0, reason=assign>(read<@type0>(%20)));
// DEFAULT-NEXT:         let %38: bool [synthetic];
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<void>, ptr<@type1>) -> i32>(%14, read<ptr<void>>(%22), addr_of<ptr<@type1>>(field0(field1(deref(read<ptr<@type3>>(%21)))))), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%38, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%38, not<bool>(ne<i32>(call<i32, signature=fn(ptr<void>, ptr<@type1>) -> i32>(%14, read<ptr<void>>(%22), addr_of<ptr<@type1>>(field1(field1(deref(read<ptr<@type3>>(%21)))))), const<i32>(0))));
// DEFAULT-NEXT:         if read<bool>(%38)
// DEFAULT-NEXT:             return null<ptr<@type3>>;
// DEFAULT-NEXT:         return read<ptr<@type3>>(%21);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @foo(%24 x: ptr<u32>, %25 y: u32, %26 z: ptr<ptr<void>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 v: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %28 i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %29 j: u32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%27, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(deref(read<ptr<ptr<void>>>(%26)), null<ptr<void>>);
// DEFAULT-NEXT:         for %36
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%28, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%28), read<u32>(%25))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %39: u32 [synthetic] = read<u32>(%28);
// DEFAULT-NEXT:                 let %40: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%39), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%28, read<u32>(%40));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %30 c: ptr<@type3> [storage=automatic];
// DEFAULT-NEXT:                     write<u32>(%29, read<u32>(deref(read<ptr<u32>>(%24))));
// DEFAULT-NEXT:                     switch %37 read<u32>(%29)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %37 const<u32>(1):
// DEFAULT-NEXT:                                 write<ptr<@type3>>(%30, call<ptr<@type3>, signature=fn(u32, ptr<void>) -> ptr<@type3>>(%17, read<u32>(%29), pointer_cast<ptr<void>, reason=arg>(read<ptr<u32>>(%24))));
// DEFAULT-NEXT:                                 call<ptr<@type3>, signature=fn(u32, ptr<void>) -> ptr<@type3>>(%17, read<u32>(%29), pointer_cast<ptr<void>, reason=arg>(read<ptr<u32>>(%24)));
// DEFAULT-NEXT:                             break %37;
// DEFAULT-NEXT:                             default %37:
// DEFAULT-NEXT:                                 write<ptr<@type3>>(%30, null<ptr<@type3>>);
// DEFAULT-NEXT:                             break %37;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     if ne<ptr<@type3>>(read<ptr<@type3>>(%30), null<ptr<@type3>>)
// DEFAULT-NEXT:                         write<ptr<void>>(%27, call<ptr<void>, signature=fn(ptr<void>, ptr<void>) -> ptr<void>>(%8, read<ptr<void>>(%27), pointer_cast<ptr<void>, reason=arg>(read<ptr<@type3>>(%30))));
// DEFAULT-NEXT:                         call<ptr<void>, signature=fn(ptr<void>, ptr<void>) -> ptr<void>>(%8, read<ptr<void>>(%27), pointer_cast<ptr<void>, reason=arg>(read<ptr<@type3>>(%30)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         return const<i32>(1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<void>>(deref(read<ptr<ptr<void>>>(%26)), read<ptr<void>>(%27));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %32 one: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         let %33 p: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u32>, u32, ptr<ptr<void>>) -> i32>(%23, addr_of<ptr<u32>>(%32), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), addr_of<ptr<ptr<void>>>(%33));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
