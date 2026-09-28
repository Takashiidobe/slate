/* Test C23 support for empty initializers: valid use cases.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

extern void abort (void);
extern void *memset (void *, int, __SIZE_TYPE__);
#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)

struct A { unsigned char a; long long b; };
struct B { unsigned char a; long long b; struct A c[3]; };
struct C { struct A a; };
struct D { unsigned char a; long long b; struct C c; };
union U { unsigned char a; long long b; };

[[gnu::noipa]] void
check_A_padding (struct A *p)
{
  unsigned char *q = (unsigned char *) p;
  unsigned char *r = (unsigned char *) p;
  for (q += offsetof (struct A, a) + 1; q != r + offsetof (struct A, b); ++q)
    if (*q != 0)
      abort ();
}

[[gnu::noipa]] void
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

[[gnu::noipa]] void
check_D_padding (struct D *p)
{
  unsigned char *q = (unsigned char *) p;
  unsigned char *r = (unsigned char *) p;
  for (q += offsetof (struct D, a) + 1; q != r + offsetof (struct D, b); ++q)
    if (*q != 0)
      abort ();
  check_A_padding (&p->c.a);
}

[[gnu::noipa]] void
check_U_padding (union U *p)
{
  unsigned char *q = (unsigned char *) p;
  unsigned char *r = (unsigned char *) p;
  for (q += 1; q != r + sizeof (union U); ++q)
    if (*q != 0)
      abort ();
}

[[gnu::noipa]] void
check (struct A *a, struct B *b, struct B *c, struct B *d, struct B *e,
       struct B *f, struct B *g, union U *h, union U *i, union U *j,
       union U *k, struct D *l, struct D *m, struct D *n)
{
  /* Empty initializer in C23 clears padding and default initializes
     all members.  */
  if (a->a != 0 || a->b != 0)
    abort ();
  check_A_padding (a);
  if (b->a != 0 || b->b != 0)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (b->c[i].a != 0 || b->c[i].b != 0)
      abort ();
  check_B_padding (b);
  /* In *c the padding between c->a and c->b is indeterminate, but
     padding in c->c[0] (and 1 and 2) zero initialized (already since C11).  */
  if (c->a != 1 || c->b != 2)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (c->c[i].a != 0 || c->c[i].b != 0)
      abort ();
    else
      check_A_padding (&c->c[i]);
  /* In *d the padding between d->a and d->b is indeterminate, but
     padding in d->c[0] (and 1) zero initialized (already since C11),
     padding in d->c[2] again indeterminate.  */
  if (d->a != 2 || d->b != 1)
    abort ();
  for (int i = 0; i < 2; ++i)
    if (d->c[i].a != 0 || d->c[i].b != 0)
      abort ();
    else
      check_A_padding (&d->c[i]);
  if (d->c[2].a != 3 || d->c[2].b != 4)
    abort ();
  /* In *e the padding between e->a and e->b is indeterminate,
     but padding in e->c[0] (and 2) zero initialized (since C23).  */
  if (e->a != 1 || e->b != 2)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (e->c[i].a != 3 + 2 * i || e->c[i].b != 4 + 2 * i)
      abort ();
    else if (i != 1)
      check_A_padding (&e->c[i]);
  /* In *f the padding between f->a and f->b is indeterminate,
     but padding in f->c[0] (and 1 and 2) zero initialized (since C23).  */
  if (f->a != 1 || f->b != 2)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (f->c[i].a != 3 + 2 * i || f->c[i].b != 4 + 2 * i)
      abort ();
    else
      check_A_padding (&f->c[i]);
  /* In *g all padding is indeterminate.  */
  if (g->a != 1 || g->b != 2)
    abort ();
  for (int i = 0; i < 3; ++i)
    if (g->c[i].a != 3 + 2 * i || g->c[i].b != 4 + 2 * i)
      abort ();
  /* In *h h->a is default initialized and padding cleared.  */
  if (h->a != 0)
    abort ();
  check_U_padding (h);
  /* In *i (and *j) i->a is initialized and padding indeterminate.  */
  if (i->a != 1 || j->a != 1)
    abort ();
  /* In *k k->b is initialized and there is (likely) no padding.  */
  if (k->b != 1)
    abort ();
  /* Empty initializer in C23 clears padding and default initializes
     all members.  */
  if (l->a != 0 || l->b != 0 || l->c.a.a != 0 || l->c.a.b != 0)
    abort ();
  check_D_padding (l);
  /* In *m the padding between m->a and m->b is indeterminate, but
     padding in m->c.a is zero initialized (already since C11).  */
  if (m->a != 1 || m->b != 2 || m->c.a.a != 0 || m->c.a.b != 0)
    abort ();
  check_A_padding (&m->c.a);
  /* In *n the padding between n->a and n->b is indeterminate,
     but padding in n->c.a zero initialized (since C23).  */
  if (n->a != 1 || n->b != 2 || n->c.a.a != 3 || n->c.a.b != 4)
    abort ();
  check_A_padding (&n->c.a);
}

[[gnu::noipa]] void
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

[[gnu::noipa]] void
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

[[gnu::noipa]] void
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

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: array<@type0, 3>;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 a: @type0;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type3 D = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: @type2;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type4 U = union {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @memset(%91 <unnamed>: ptr<void>, %92 <unnamed>: i32, %93 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @check_A_padding(%8 p: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 q: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type0>>(%8));
// DEFAULT-NEXT:         let %10 r: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type0>>(%8));
// DEFAULT-NEXT:         for %94
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %105: ptr<u8> [synthetic] = read<ptr<u8>>(%9);
// DEFAULT-NEXT:                 let %106: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%105), add<u64, overflow=wrap>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<ptr<u8>>(%9, read<ptr<u8>>(%106));
// DEFAULT-NEXT:             condition: ne<ptr<u8>>(read<ptr<u8>>(%9), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%10), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %107: ptr<u8> [synthetic] = read<ptr<u8>>(%9);
// DEFAULT-NEXT:                 let %108: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%107), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%9, read<ptr<u8>>(%108));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%9))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @check_B_padding(%12 p: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 q: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type1>>(%12));
// DEFAULT-NEXT:         let %14 r: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type1>>(%12));
// DEFAULT-NEXT:         for %95
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %109: ptr<u8> [synthetic] = read<ptr<u8>>(%13);
// DEFAULT-NEXT:                 let %110: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%109), add<u64, overflow=wrap>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<ptr<u8>>(%13, read<ptr<u8>>(%110));
// DEFAULT-NEXT:             condition: ne<ptr<u8>>(read<ptr<u8>>(%13), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%14), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %111: ptr<u8> [synthetic] = read<ptr<u8>>(%13);
// DEFAULT-NEXT:                 let %112: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%111), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%13, read<ptr<u8>>(%112));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%13))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         for %96
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %15 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%15), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %113: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %114: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%113), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%114));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%12)))), read<i32>(%15)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @check_D_padding(%17 p: ptr<@type3>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %18 q: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type3>>(%17));
// DEFAULT-NEXT:         let %19 r: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type3>>(%17));
// DEFAULT-NEXT:         for %97
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %115: ptr<u8> [synthetic] = read<ptr<u8>>(%18);
// DEFAULT-NEXT:                 let %116: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%115), add<u64, overflow=wrap>(const<u64>(0), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<ptr<u8>>(%18, read<ptr<u8>>(%116));
// DEFAULT-NEXT:             condition: ne<ptr<u8>>(read<ptr<u8>>(%18), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %117: ptr<u8> [synthetic] = read<ptr<u8>>(%18);
// DEFAULT-NEXT:                 let %118: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%117), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%18, read<ptr<u8>>(%118));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%18))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(field0(field2(deref(read<ptr<@type3>>(%17))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @check_U_padding(%21 p: ptr<@type4>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %22 q: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type4>>(%21));
// DEFAULT-NEXT:         let %23 r: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<@type4>>(%21));
// DEFAULT-NEXT:         for %98
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %119: ptr<u8> [synthetic] = read<ptr<u8>>(%22);
// DEFAULT-NEXT:                 let %120: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%119), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%22, read<ptr<u8>>(%120));
// DEFAULT-NEXT:             condition: ne<ptr<u8>>(read<ptr<u8>>(%22), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %121: ptr<u8> [synthetic] = read<ptr<u8>>(%22);
// DEFAULT-NEXT:                 let %122: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%121), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%22, read<ptr<u8>>(%122));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%22))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @check(%25 a: ptr<@type0>, %26 b: ptr<@type1>, %27 c: ptr<@type1>, %28 d: ptr<@type1>, %29 e: ptr<@type1>, %30 f: ptr<@type1>, %31 g: ptr<@type1>, %32 h: ptr<@type4>, %33 i: ptr<@type4>, %34 j: ptr<@type4>, %35 k: ptr<@type4>, %36 l: ptr<@type3>, %37 m: ptr<@type3>, %38 n: ptr<@type3>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type0>>(%25)))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(read<ptr<@type0>>(%25)))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%7, read<ptr<@type0>>(%25));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type1>>(%26)))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(read<ptr<@type1>>(%26)))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         for %99
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %39 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%39), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %123: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:                 let %124: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%123), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%39, read<i32>(%124));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%26)))), read<i32>(%39))))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%26)))), read<i32>(%39))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%11, read<ptr<@type1>>(%26));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type1>>(%27)))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type1>>(%27)))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         for %100
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %40 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%40), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %125: i32 [synthetic] = read<i32>(%40);
// DEFAULT-NEXT:                 let %126: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%125), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%40, read<i32>(%126));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%27)))), read<i32>(%40))))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%27)))), read<i32>(%40))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%27)))), read<i32>(%40)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type1>>(%28)))))), const<i32>(2)), ne<i64>(read<i64>(field1(deref(read<ptr<@type1>>(%28)))), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         for %101
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %41 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%41), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %127: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:                 let %128: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%127), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%41, read<i32>(%128));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%28)))), read<i32>(%41))))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%28)))), read<i32>(%41))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%28)))), read<i32>(%41)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%28)))), const<i32>(2))))))), const<i32>(3)), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%28)))), const<i32>(2))))), widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type1>>(%29)))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type1>>(%29)))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         for %102
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %42 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%42), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %129: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:                 let %130: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%129), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%42, read<i32>(%130));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%29)))), read<i32>(%42))))))), add<i32, overflow=ub>(const<i32>(3), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%42)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%29)))), read<i32>(%42))))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(4), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%42))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%42), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%29)))), read<i32>(%42)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type1>>(%30)))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type1>>(%30)))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         for %103
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %43 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%43), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %131: i32 [synthetic] = read<i32>(%43);
// DEFAULT-NEXT:                 let %132: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%131), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%43, read<i32>(%132));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%30)))), read<i32>(%43))))))), add<i32, overflow=ub>(const<i32>(3), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%43)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%30)))), read<i32>(%43))))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(4), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%43))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%30)))), read<i32>(%43)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type1>>(%31)))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type1>>(%31)))), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         for %104
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %44 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%44), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %133: i32 [synthetic] = read<i32>(%44);
// DEFAULT-NEXT:                 let %134: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%133), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%44, read<i32>(%134));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%31)))), read<i32>(%44))))))), add<i32, overflow=ub>(const<i32>(3), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%44)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(field2(deref(read<ptr<@type1>>(%31)))), read<i32>(%44))))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(4), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%44))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type4>>(%32)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>) -> void>(%20, read<ptr<@type4>>(%32));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type4>>(%33)))))), const<i32>(1)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type4>>(%34)))))), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field1(deref(read<ptr<@type4>>(%35)))), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type3>>(%36)))))), const<i32>(0)), ne<i64>(read<i64>(field1(deref(read<ptr<@type3>>(%36)))), widen<i64, reason=usual_arith>(const<i32>(0)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(field0(field2(deref(read<ptr<@type3>>(%36)))))))), const<i32>(0))), ne<i64>(read<i64>(field1(field0(field2(deref(read<ptr<@type3>>(%36)))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>) -> void>(%16, read<ptr<@type3>>(%36));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type3>>(%37)))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type3>>(%37)))), widen<i64, reason=usual_arith>(const<i32>(2)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(field0(field2(deref(read<ptr<@type3>>(%37)))))))), const<i32>(0))), ne<i64>(read<i64>(field1(field0(field2(deref(read<ptr<@type3>>(%37)))))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(field0(field2(deref(read<ptr<@type3>>(%37))))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(deref(read<ptr<@type3>>(%38)))))), const<i32>(1)), ne<i64>(read<i64>(field1(deref(read<ptr<@type3>>(%38)))), widen<i64, reason=usual_arith>(const<i32>(2)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(field0(field2(deref(read<ptr<@type3>>(%38)))))))), const<i32>(3))), ne<i64>(read<i64>(field1(field0(field2(deref(read<ptr<@type3>>(%38)))))), widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%7, addr_of<ptr<@type0>>(field0(field2(deref(read<ptr<@type3>>(%38))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %46 a: @type0 [storage=automatic] = aggregate<@type0, zero_fill=true>();
// DEFAULT-NEXT:         let %47 b: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         let %48 c: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %49 d: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), field1 = widen<i64, reason=assign>(const<i32>(1)), field2 = aggregate<array<@type0, 3>, zero_fill=true>(index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4)))));
// DEFAULT-NEXT:         let %50 e: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %51 f: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %52 g: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %53 h: @type4 [storage=automatic] = aggregate<@type4, zero_fill=false>();
// DEFAULT-NEXT:         let %54 i: @type4 [storage=automatic] = aggregate<@type4, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %55 j: @type4 [storage=automatic] = aggregate<@type4, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %56 k: @type4 [storage=automatic] = aggregate<@type4, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %57 l: @type3 [storage=automatic] = aggregate<@type3, zero_fill=true>();
// DEFAULT-NEXT:         let %58 m: @type3 [storage=automatic] = aggregate<@type3, zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %59 n: @type3 [storage=automatic] = aggregate<@type3, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<@type1>, ptr<@type1>, ptr<@type1>, ptr<@type1>, ptr<@type1>, ptr<@type1>, ptr<@type4>, ptr<@type4>, ptr<@type4>, ptr<@type4>, ptr<@type3>, ptr<@type3>, ptr<@type3>) -> void>(%24, addr_of<ptr<@type0>>(%46), addr_of<ptr<@type1>>(%47), addr_of<ptr<@type1>>(%48), addr_of<ptr<@type1>>(%49), addr_of<ptr<@type1>>(%50), addr_of<ptr<@type1>>(%51), addr_of<ptr<@type1>>(%52), addr_of<ptr<@type4>>(%53), addr_of<ptr<@type4>>(%54), addr_of<ptr<@type4>>(%55), addr_of<ptr<@type4>>(%56), addr_of<ptr<@type3>>(%57), addr_of<ptr<@type3>>(%58), addr_of<ptr<@type3>>(%59));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @set(%61 a: ptr<@type0>, %62 b: ptr<@type1>, %63 c: ptr<@type1>, %64 d: ptr<@type1>, %65 e: ptr<@type1>, %66 f: ptr<@type1>, %67 g: ptr<@type1>, %68 h: ptr<@type4>, %69 i: ptr<@type4>, %70 j: ptr<@type4>, %71 k: ptr<@type4>, %72 l: ptr<@type3>, %73 m: ptr<@type3>, %74 n: ptr<@type3>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type0>>(%61)), not<i32>(const<i32>(0)), const<u64>(16));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%62)), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%63)), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%64)), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%65)), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%66)), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%67)), not<i32>(const<i32>(0)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type4>>(%68)), not<i32>(const<i32>(0)), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type4>>(%69)), not<i32>(const<i32>(0)), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type4>>(%70)), not<i32>(const<i32>(0)), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type4>>(%71)), not<i32>(const<i32>(0)), const<u64>(8));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type3>>(%72)), not<i32>(const<i32>(0)), const<u64>(32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type3>>(%73)), not<i32>(const<i32>(0)), const<u64>(32));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type3>>(%74)), not<i32>(const<i32>(0)), const<u64>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @prepare() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %76 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %77 b: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %78 c: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %79 d: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %80 e: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %81 f: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %82 g: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %83 h: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %84 i: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %85 j: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %86 k: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %87 l: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %88 m: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %89 n: @type3 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<@type1>, ptr<@type1>, ptr<@type1>, ptr<@type1>, ptr<@type1>, ptr<@type1>, ptr<@type4>, ptr<@type4>, ptr<@type4>, ptr<@type4>, ptr<@type3>, ptr<@type3>, ptr<@type3>) -> void>(%60, addr_of<ptr<@type0>>(%76), addr_of<ptr<@type1>>(%77), addr_of<ptr<@type1>>(%78), addr_of<ptr<@type1>>(%79), addr_of<ptr<@type1>>(%80), addr_of<ptr<@type1>>(%81), addr_of<ptr<@type1>>(%82), addr_of<ptr<@type4>>(%83), addr_of<ptr<@type4>>(%84), addr_of<ptr<@type4>>(%85), addr_of<ptr<@type4>>(%86), addr_of<ptr<@type3>>(%87), addr_of<ptr<@type3>>(%88), addr_of<ptr<@type3>>(%89));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%75);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%45);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
