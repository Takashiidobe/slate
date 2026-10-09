/* Test C2y static assertions in expressions: -Wc23-c2y-compat warnings.  */
/* { dg-do compile } */
/* { dg-options "-std=c2y -pedantic-errors -Wc23-c2y-compat" } */

/* Old forms of static assertion still valid.  */
static_assert (1);
static_assert (2, "message" );
struct s { int a; static_assert (3); };

void
f ()
{
  static_assert (4);
 label:
  static_assert (5);
  for (static_assert (6);;)
    ;
}

/* Test new forms of static assertion.  */
void
g ()
{
  (void) 0, static_assert (7), (void) 0; /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  extern typeof (static_assert (8)) f (); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  1
    ? static_assert (9) /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
    : static_assert (10); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  if (1)
    static_assert (11); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  else
    static_assert (12); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  for (;;)
    static_assert (13); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  while (true)
    static_assert (14); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  do
    static_assert (15); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  while (false);
  switch (16)
    static_assert (17); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  (void) static_assert (18); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  (static_assert (19)); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
}

void
g2 ()
{
  (void) 0, static_assert (7, "message"), (void) 0; /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  extern typeof (static_assert (8, "message")) f (); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  1
    ? static_assert (9, "message") /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
    : static_assert (10, "message"); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  if (1)
    static_assert (11, "message"); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  else
    static_assert (12, "message"); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  for (;;)
    static_assert (13, "message"); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  while (true)
    static_assert (14, "message"); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  do
    static_assert (15, "message"); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  while (false);
  switch (16)
    static_assert (17, "message"); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  (void) static_assert (18, "message"); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
  (static_assert (19, "message")); /* { dg-warning "ISO C does not support static assertions in expressions before C2Y" } */
}

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         label %[[VALUE_label:[0-9]+]] label:
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         conditional<void>(ne<i32>(const<i32>(1), const<i32>(0)), void, void);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:         while %[[VALUE2:[0-9]+]] const<bool>(true)
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:         while const<bool>(false);
// DEFAULT-NEXT:         switch %[[VALUE4:[0-9]+]] const<i32>(16)
// DEFAULT-NEXT:         void;
// DEFAULT-NEXT:         void;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g2:[0-9]+]] @g2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         conditional<void>(ne<i32>(const<i32>(1), const<i32>(0)), void, void);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:         while %[[VALUE6:[0-9]+]] const<bool>(true)
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:         while const<bool>(false);
// DEFAULT-NEXT:         switch %[[VALUE8:[0-9]+]] const<i32>(16)
// DEFAULT-NEXT:         void;
// DEFAULT-NEXT:         void;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
