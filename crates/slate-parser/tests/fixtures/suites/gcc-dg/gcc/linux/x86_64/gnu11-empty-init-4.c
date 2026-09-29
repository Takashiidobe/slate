/* Test GNU C11 support for empty initializers.  */
/* { dg-do run } */
/* { dg-options "-std=gnu11 -fzero-init-padding-bits=all" } */

extern void abort (void);
extern void *memset (void *, int, __SIZE_TYPE__);
#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)

struct A { unsigned char a; long long b; };
struct B { unsigned char a; long long b; struct A c[3]; };
struct C { struct A a; };
struct D { unsigned char a; long long b; struct C c; };
union U { unsigned char a; long long b; };

__attribute__((noipa)) void
check_A_padding (struct A *p)
{
  unsigned char *q = (unsigned char *) p;
  unsigned char *r = (unsigned char *) p;
  for (q += offsetof (struct A, a) + 1; q != r + offsetof (struct A, b); ++q)
    if (*q != 0)
      abort ();
}

__attribute__((noipa)) void
check_B_padding (struct B *p)
{
  unsigned char *q = (unsigned char *) p;
  unsigned char *r = (unsigned char *) p;
  for (q += offsetof (struct B, a) + 1; q != r + offsetof (struct B, b); ++q)
    if (*q != 0)
      abort ();
  for (int i = 0; i < 3; ++i)
    check_A_padding (&p->c[i]);
}

__attribute__((noipa)) void
check_D_padding (struct D *p)
{
  unsigned char *q = (unsigned char *) p;
  unsigned char *r = (unsigned char *) p;
  for (q += offsetof (struct D, a) + 1; q != r + offsetof (struct D, b); ++q)
    if (*q != 0)
      abort ();
  check_A_padding (&p->c.a);
}

__attribute__((noipa)) void
check_U_padding (union U *p)
{
  unsigned char *q = (unsigned char *) p;
  unsigned char *r = (unsigned char *) p;
  for (q += 1; q != r + sizeof (union U); ++q)
    if (*q != 0)
      abort ();
}

__attribute__((noipa)) void
check (struct A *a, struct B *b, struct B *c, struct B *d, struct B *e,
       struct B *f, struct B *g, union U *h, union U *i, union U *j,
       union U *k, struct D *l, struct D *m, struct D *n)
{
  /* All padding bits are well defined with -fzero-init-padding-bits=all.  */
  if (a->a != 0 || a->b != 0)
    abort ();
  check_A_padding (a);
  if (b->a != 0 || b->b != 0)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (b->c[i].a != 0 || b->c[i].b != 0)
      abort ();
  check_B_padding (b);
  if (c->a != 1 || c->b != 2)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (c->c[i].a != 0 || c->c[i].b != 0)
      abort ();
  check_B_padding (c);
  if (d->a != 2 || d->b != 1)
    abort ();
  for (int i = 0; i < 2; ++i)
    if (d->c[i].a != 0 || d->c[i].b != 0)
      abort ();
  if (d->c[2].a != 3 || d->c[2].b != 4)
    abort ();
  check_B_padding (d);
  if (e->a != 1 || e->b != 2)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (e->c[i].a != 3 + 2 * i || e->c[i].b != 4 + 2 * i)
      abort ();
  check_B_padding (e);
  if (f->a != 1 || f->b != 2)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (f->c[i].a != 3 + 2 * i || f->c[i].b != 4 + 2 * i)
      abort ();
  check_B_padding (f);
  if (g->a != 1 || g->b != 2)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (g->c[i].a != 3 + 2 * i || g->c[i].b != 4 + 2 * i)
      abort ();
  check_B_padding (g);
  if (h->a != 0)
    abort ();
  check_U_padding (h);
  if (i->a != 1 || j->a != 1)
    abort ();
  check_U_padding (i);
  check_U_padding (j);
  /* In *k k->b is initialized and there is (likely) no padding.  */
  if (k->b != 1)
    abort ();
  if (l->a != 0 || l->b != 0 || l->c.a.a != 0 || l->c.a.b != 0)
    abort ();
  check_D_padding (l);
  if (m->a != 1 || m->b != 2 || m->c.a.a != 0 || m->c.a.b != 0)
    abort ();
  check_D_padding (m);
  if (n->a != 1 || n->b != 2 || n->c.a.a != 3 || n->c.a.b != 4)
    abort ();
  check_D_padding (n);
}

__attribute__((noipa)) void
test (void)
{
  struct A a = {};
  struct B b = {};
  struct B c = { 1, 2 };
  struct B d = { .b = 1, .a = 2, .c[2].a = 3, .c[2].b = 4 };
  struct B e = { 1, 2, .c[2] = {}, .c[1] = { 9 }, .c[0] = {},
		 .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };
  struct B f = { 1, 2, {},
		 .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };
  struct B g = { 1, 2, .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };
  union U h = {};
  union U i = { 1 };
  union U j = { .a = 1 };
  union U k = { .b = 1 };
  struct D l = {};
  struct D m = { 1, 2 };
  struct D n = { 1, 2, {}, .c.a.a = 3, .c.a.b = 4 };
  check (&a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n);
}

__attribute__((noipa)) void
set (struct A *a, struct B *b, struct B *c, struct B *d, struct B *e,
     struct B *f, struct B *g, union U *h, union U *i, union U *j,
     union U *k, struct D *l, struct D *m, struct D *n)
{
  memset (a, ~0, sizeof (*a));
  memset (b, ~0, sizeof (*b));
  memset (c, ~0, sizeof (*c));
  memset (d, ~0, sizeof (*d));
  memset (e, ~0, sizeof (*e));
  memset (f, ~0, sizeof (*f));
  memset (g, ~0, sizeof (*g));
  memset (h, ~0, sizeof (*h));
  memset (i, ~0, sizeof (*i));
  memset (j, ~0, sizeof (*j));
  memset (k, ~0, sizeof (*k));
  memset (l, ~0, sizeof (*l));
  memset (m, ~0, sizeof (*m));
  memset (n, ~0, sizeof (*n));
}

__attribute__((noipa)) void
prepare (void)
{
  struct A a;
  struct B b, c, d, e, f, g;
  union U h, i, j, k;
  struct D l, m, n;
  set (&a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n);
}

int
main ()
{
  prepare ();
  test ();
}

// SLATE-FILECHECK-STD DEFAULT gnu11
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: array<@type[[TYPE_A]], 3>;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 a: @type[[TYPE_A]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: @type[[TYPE_C]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_check_A_padding:[0-9]+]] @check_A_padding(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_A]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE4]]), add<u64, overflow=wrap>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_q]], read<ptr<u8>>(%[[VALUE5]]));
// DEFAULT-NEXT:             condition: ne<ptr<u8>>(read<ptr<u8>>(%[[VALUE_q]]), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_r]]), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_q]], read<ptr<u8>>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%[[VALUE_q]]))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_B_padding:[0-9]+]] @check_B_padding(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_B]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q_2:[0-9]+]] q: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:         for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_q_2]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE9]]), add<u64, overflow=wrap>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_q_2]], read<ptr<u8>>(%[[VALUE10]]));
// DEFAULT-NEXT:             condition: ne<ptr<u8>>(read<ptr<u8>>(%[[VALUE_q_2]]), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_r_2]]), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_q_2]]);
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_q_2]], read<ptr<u8>>(%[[VALUE12]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%[[VALUE_q_2]]))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_check_A_padding]], addr_of<ptr<@type[[TYPE_A]]>>(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_p_2]])))), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_D_padding:[0-9]+]] @check_D_padding(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_D]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q_3:[0-9]+]] q: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:         for %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_q_3]]);
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE17]]), add<u64, overflow=wrap>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_q_3]], read<ptr<u8>>(%[[VALUE18]]));
// DEFAULT-NEXT:             condition: ne<ptr<u8>>(read<ptr<u8>>(%[[VALUE_q_3]]), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_r_3]]), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_q_3]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_q_3]], read<ptr<u8>>(%[[VALUE20]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%[[VALUE_q_3]]))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_check_A_padding]], addr_of<ptr<@type[[TYPE_A]]>>(field0(field2(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_p_3]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_U_padding:[0-9]+]] @check_U_padding(%[[VALUE_p_4:[0-9]+]] p: ptr<@type[[TYPE_U]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q_4:[0-9]+]] q: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_p_4]]));
// DEFAULT-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_p_4]]));
// DEFAULT-NEXT:         for %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_q_4]]);
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_q_4]], read<ptr<u8>>(%[[VALUE23]]));
// DEFAULT-NEXT:             condition: ne<ptr<u8>>(read<ptr<u8>>(%[[VALUE_q_4]]), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_r_4]]), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_q_4]]);
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_q_4]], read<ptr<u8>>(%[[VALUE25]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%[[VALUE_q_4]]))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check:[0-9]+]] @check(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_A]]>, %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_B]]>, %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_B]]>, %[[VALUE_d:[0-9]+]] d: ptr<@type[[TYPE_B]]>, %[[VALUE_e:[0-9]+]] e: ptr<@type[[TYPE_B]]>, %[[VALUE_f:[0-9]+]] f: ptr<@type[[TYPE_B]]>, %[[VALUE_g:[0-9]+]] g: ptr<@type[[TYPE_B]]>, %[[VALUE_h:[0-9]+]] h: ptr<@type[[TYPE_U]]>, %[[VALUE_i_2:[0-9]+]] i: ptr<@type[[TYPE_U]]>, %[[VALUE_j:[0-9]+]] j: ptr<@type[[TYPE_U]]>, %[[VALUE_k:[0-9]+]] k: ptr<@type[[TYPE_U]]>, %[[VALUE_l:[0-9]+]] l: ptr<@type[[TYPE_D]]>, %[[VALUE_m:[0-9]+]] m: ptr<@type[[TYPE_D]]>, %[[VALUE_n:[0-9]+]] n: ptr<@type[[TYPE_D]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]])))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]])))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_check_A_padding]], read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_b]])))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_b]])))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE28]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_b]])))), read<i32>(%[[VALUE_i_3]]))))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_b]])))), read<i32>(%[[VALUE_i_3]]))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_B]]>) -> void>(%[[VALUE_check_B_padding]], read<ptr<@type[[TYPE_B]]>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_c]])))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_c]])))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_4]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE30]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_c]])))), read<i32>(%[[VALUE_i_4]]))))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_c]])))), read<i32>(%[[VALUE_i_4]]))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_B]]>) -> void>(%[[VALUE_check_B_padding]], read<ptr<@type[[TYPE_B]]>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_d]])))))), const<i32>(2)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_d]])))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_5:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_5]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE33]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE34]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_d]])))), read<i32>(%[[VALUE_i_5]]))))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_d]])))), read<i32>(%[[VALUE_i_5]]))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_d]])))), const<i32>(2))))))), const<i32>(3)), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_d]])))), const<i32>(2))))), widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_B]]>) -> void>(%[[VALUE_check_B_padding]], read<ptr<@type[[TYPE_B]]>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_e]])))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_e]])))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_6:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_6]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_6]]);
// DEFAULT-NEXT:                 let %[[VALUE37:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE36]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_6]], read<i32>(%[[VALUE37]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_e]])))), read<i32>(%[[VALUE_i_6]]))))))), add<i32, overflow=ub>(const<i32>(3), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_i_6]])))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_e]])))), read<i32>(%[[VALUE_i_6]]))))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(4), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_i_6]]))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_B]]>) -> void>(%[[VALUE_check_B_padding]], read<ptr<@type[[TYPE_B]]>>(%[[VALUE_e]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_f]])))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_f]])))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_7:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_7]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_7]]);
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE39]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_7]], read<i32>(%[[VALUE40]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_f]])))), read<i32>(%[[VALUE_i_7]]))))))), add<i32, overflow=ub>(const<i32>(3), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_i_7]])))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_f]])))), read<i32>(%[[VALUE_i_7]]))))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(4), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_i_7]]))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_B]]>) -> void>(%[[VALUE_check_B_padding]], read<ptr<@type[[TYPE_B]]>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_g]])))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_g]])))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE41:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_8:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_8]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE42:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_8]]);
// DEFAULT-NEXT:                 let %[[VALUE43:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE42]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_8]], read<i32>(%[[VALUE43]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_g]])))), read<i32>(%[[VALUE_i_8]]))))))), add<i32, overflow=ub>(const<i32>(3), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_i_8]])))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(3)>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_g]])))), read<i32>(%[[VALUE_i_8]]))))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(4), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_i_8]]))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_B]]>) -> void>(%[[VALUE_check_B_padding]], read<ptr<@type[[TYPE_B]]>>(%[[VALUE_g]]));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_h]])))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_U]]>) -> void>(%[[VALUE_check_U_padding]], read<ptr<@type[[TYPE_U]]>>(%[[VALUE_h]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_i_2]])))))), const<i32>(1)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_j]])))))), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_U]]>) -> void>(%[[VALUE_check_U_padding]], read<ptr<@type[[TYPE_U]]>>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_U]]>) -> void>(%[[VALUE_check_U_padding]], read<ptr<@type[[TYPE_U]]>>(%[[VALUE_j]]));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_k]])))), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_l]])))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_l]])))), widen<i64, reason=usual_arith>(const<i32>(0)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(field0(field2(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_l]])))))))), const<i32>(0))), ne<i64>(read<i64>(field1(field0(field2(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_l]])))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_D]]>) -> void>(%[[VALUE_check_D_padding]], read<ptr<@type[[TYPE_D]]>>(%[[VALUE_l]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_m]])))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_m]])))), widen<i64, reason=usual_arith>(const<i32>(2)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(field0(field2(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_m]])))))))), const<i32>(0))), ne<i64>(read<i64>(field1(field0(field2(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_m]])))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_D]]>) -> void>(%[[VALUE_check_D_padding]], read<ptr<@type[[TYPE_D]]>>(%[[VALUE_m]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_n]])))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_n]])))), widen<i64, reason=usual_arith>(const<i32>(2)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(field0(field2(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_n]])))))))), const<i32>(3))), ne<i64>(read<i64>(field1(field0(field2(deref(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_n]])))))), widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_D]]>) -> void>(%[[VALUE_check_D_padding]], read<ptr<@type[[TYPE_D]]>>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic] = aggregate<@type[[TYPE_A]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), field1 = widen<i64, reason=assign>(const<i32>(1)), field2 = aggregate<array<@type[[TYPE_A]], 3>, zero_fill=true>(index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4)))));
// DEFAULT-NEXT:         let %[[VALUE_e_2:[0-9]+]] e: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type[[TYPE_A]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %[[VALUE_f_2:[0-9]+]] f: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type[[TYPE_A]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %[[VALUE_g_2:[0-9]+]] g: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type[[TYPE_A]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %[[VALUE_h_2:[0-9]+]] h: @type[[TYPE_U]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>();
// DEFAULT-NEXT:         let %[[VALUE_i_9:[0-9]+]] i: @type[[TYPE_U]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_j_2:[0-9]+]] j: @type[[TYPE_U]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_k_2:[0-9]+]] k: @type[[TYPE_U]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_l_2:[0-9]+]] l: @type[[TYPE_D]] [storage=automatic] = aggregate<@type[[TYPE_D]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_m_2:[0-9]+]] m: @type[[TYPE_D]] [storage=automatic] = aggregate<@type[[TYPE_D]], zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: @type[[TYPE_D]] [storage=automatic] = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_A]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_U]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_U]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_U]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_U]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_D]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_D]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_D]]>) ->
// DEFAULT-SAME: void>(%[[VALUE_check]],
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_b_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_c_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_d_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_e_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_f_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_g_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_h_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_i_9]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_j_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_k_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_D]]>>(%[[VALUE_l_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_D]]>>(%[[VALUE_m_2]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_D]]>>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_set:[0-9]+]] @set(%[[VALUE_a_3:[0-9]+]] a: ptr<@type[[TYPE_A]]>, %[[VALUE_b_3:[0-9]+]] b: ptr<@type[[TYPE_B]]>, %[[VALUE_c_3:[0-9]+]] c: ptr<@type[[TYPE_B]]>, %[[VALUE_d_3:[0-9]+]] d: ptr<@type[[TYPE_B]]>, %[[VALUE_e_3:[0-9]+]] e: ptr<@type[[TYPE_B]]>, %[[VALUE_f_3:[0-9]+]] f: ptr<@type[[TYPE_B]]>, %[[VALUE_g_3:[0-9]+]] g: ptr<@type[[TYPE_B]]>, %[[VALUE_h_3:[0-9]+]] h: ptr<@type[[TYPE_U]]>, %[[VALUE_i_10:[0-9]+]] i: ptr<@type[[TYPE_U]]>, %[[VALUE_j_3:[0-9]+]] j: ptr<@type[[TYPE_U]]>, %[[VALUE_k_3:[0-9]+]] k: ptr<@type[[TYPE_U]]>, %[[VALUE_l_3:[0-9]+]] l: ptr<@type[[TYPE_D]]>, %[[VALUE_m_3:[0-9]+]] m: ptr<@type[[TYPE_D]]>, %[[VALUE_n_3:[0-9]+]] n: ptr<@type[[TYPE_D]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a_3]])), not<i32>(const<i32>(0)), const<u64>(16));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_b_3]])), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_c_3]])), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_d_3]])), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_e_3]])), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_f_3]])), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_g_3]])), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_h_3]])), not<i32>(const<i32>(0)), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_i_10]])), not<i32>(const<i32>(0)), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_j_3]])), not<i32>(const<i32>(0)), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_k_3]])), not<i32>(const<i32>(0)), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_l_3]])), not<i32>(const<i32>(0)), const<u64>(32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_m_3]])), not<i32>(const<i32>(0)), const<u64>(32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_D]]>>(%[[VALUE_n_3]])), not<i32>(const<i32>(0)), const<u64>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_prepare:[0-9]+]] @prepare() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_4:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_4:[0-9]+]] b: @type[[TYPE_B]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_4:[0-9]+]] c: @type[[TYPE_B]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d_4:[0-9]+]] d: @type[[TYPE_B]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e_4:[0-9]+]] e: @type[[TYPE_B]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f_4:[0-9]+]] f: @type[[TYPE_B]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_g_4:[0-9]+]] g: @type[[TYPE_B]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_h_4:[0-9]+]] h: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i_11:[0-9]+]] i: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j_4:[0-9]+]] j: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k_4:[0-9]+]] k: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_l_4:[0-9]+]] l: @type[[TYPE_D]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_m_4:[0-9]+]] m: @type[[TYPE_D]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n_4:[0-9]+]] n: @type[[TYPE_D]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_A]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_B]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_U]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_U]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_U]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_U]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_D]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_D]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_D]]>) ->
// DEFAULT-SAME: void>(%[[VALUE_set]],
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_b_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_c_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_d_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_e_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_f_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_g_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_h_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_i_11]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_j_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_k_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_D]]>>(%[[VALUE_l_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_D]]>>(%[[VALUE_m_4]]),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_D]]>>(%[[VALUE_n_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_prepare]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
