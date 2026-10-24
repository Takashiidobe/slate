/* PR c/81233 */
/* { dg-do compile } */
/* { dg-options "-fpermissive -Wc++-compat -Wpedantic" } */
/* Test we're printing the types, like the good compiler we are.  */

enum E1 { A } e;
enum E2 { B };
extern void foo_E (enum E1); /* { dg-message "expected 'enum E1' but argument is of type 'int'" } */
extern void foo (char *); /* { dg-message "expected 'char \\*' but argument is of type 'int \\*'" } */
extern void foo2 (int *); /* { dg-message "expected 'int \\*' but argument is of type 'int'" } */
extern void foo3 (int); /* { dg-message "expected 'int' but argument is of type 'int \\*'" } */
extern void foo4 (int *); /* { dg-message "expected 'int \\*' but argument is of type 'unsigned int \\*'" } */

char *
fn0 (int *p, char *q)
{
  p = q; /* { dg-warning "assignment to 'int \\*' from incompatible pointer type 'char \\*'" } */
  int *r = q; /* { dg-warning "initialization of 'int \\*' from incompatible pointer type 'char \\*'" } */
  foo (r); /* { dg-warning "passing argument 1 of 'foo' from incompatible pointer type" } */
  return p; /* { dg-warning "returning 'int \\*' from a function with incompatible return type 'char \\*'" } */
}

int *
fn1 (int *p)
{
  p = 1; /* { dg-warning "assignment to 'int \\*' from 'int' makes pointer from integer without a cast" } */
  int *q = 1; /* { dg-warning "initialization of 'int \\*' from 'int' makes pointer from integer without a cast" } */
  foo2 (1); /* { dg-warning "passing argument 1 of 'foo2' makes pointer from integer without a cast" } */
  return 1; /* { dg-warning "returning 'int' from a function with return type 'int \\*' makes pointer from integer without a cast" } */
}

int
fn2 (int i, int *p)
{
  i = p; /* { dg-warning "assignment to 'int' from 'int \\*' makes integer from pointer without a cast" } */
  int j = p; /* { dg-warning "initialization of 'int' from 'int \\*' makes integer from pointer without a cast" } */
  foo3 (p); /* { dg-warning "passing argument 1 of 'foo3' makes integer from pointer without a cast" } */
  return p; /* { dg-warning "returning 'int \\*' from a function with return type 'int' makes integer from pointer without a cast" } */
}

int *
fn3 (int *p, unsigned int *u)
{
  p = u; /* { dg-warning "pointer targets in assignment from 'unsigned int \\*' to 'int \\*' differ in signedness" } */
  int *q = u; /* { dg-warning "pointer targets in initialization of 'int \\*' from 'unsigned int \\*' differ in signedness" } */
  foo4 (u); /* { dg-warning "pointer targets in passing argument 1 of 'foo4' differ in signedness" } */
  return u; /* { dg-warning "pointer targets in returning 'unsigned int \\*' from a function with return type 'int \\*' differ in signedness" } */
}

enum E1
fn4 (void)
{
  foo_E (B); /* { dg-warning "enum conversion when passing argument" } */
  e = 0; /* { dg-warning "enum conversion from 'int' to 'enum E1' in assignment is invalid" } */
  enum E1 f = 0; /* { dg-warning "enum conversion from 'int' to 'enum E1' in initialization is invalid" } */
  return 0; /* { dg-warning "enum conversion from 'int' to 'enum E1' in return is invalid" } */
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_E1:[0-9]+]] E1 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_E2:[0-9]+]] E2 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] B = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: @type[[TYPE_E1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_E:[0-9]+]] @foo_E(%[[VALUE0:[0-9]+]] <unnamed>: @type[[TYPE_E1]]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE2:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4(%[[VALUE4:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn0:[0-9]+]] @fn0(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_q:[0-9]+]] q: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p]], pointer_cast<ptr<i32>, reason=assign>(read<ptr<i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<i8>>(%[[VALUE_q]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%[[VALUE_foo]], pointer_cast<ptr<i8>, reason=arg>(read<ptr<i32>>(%[[VALUE_r]])));
// DEFAULT-NEXT:         return pointer_cast<ptr<i8>, reason=return>(read<ptr<i32>>(%[[VALUE_p]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_2]], int_to_ptr<ptr<i32>, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_q_2:[0-9]+]] q: ptr<i32> [storage=automatic] = int_to_ptr<ptr<i32>, reason=assign>(const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_foo2]], int_to_ptr<ptr<i32>, reason=arg>(const<i32>(1)));
// DEFAULT-NEXT:         return int_to_ptr<ptr<i32>, reason=return>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], ptr_to_int<i32, reason=assign>(read<ptr<i32>>(%[[VALUE_p_3]])));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = ptr_to_int<i32, reason=assign>(read<ptr<i32>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo3]], ptr_to_int<i32, reason=arg>(read<ptr<i32>>(%[[VALUE_p_3]])));
// DEFAULT-NEXT:         return ptr_to_int<i32, reason=return>(read<ptr<i32>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3(%[[VALUE_p_4:[0-9]+]] p: ptr<i32>, %[[VALUE_u:[0-9]+]] u: ptr<u32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_4]], pointer_cast<ptr<i32>, reason=assign>(read<ptr<u32>>(%[[VALUE_u]])));
// DEFAULT-NEXT:         let %[[VALUE_q_3:[0-9]+]] q: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<u32>>(%[[VALUE_u]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_foo4]], pointer_cast<ptr<i32>, reason=arg>(read<ptr<u32>>(%[[VALUE_u]])));
// DEFAULT-NEXT:         return pointer_cast<ptr<i32>, reason=return>(read<ptr<u32>>(%[[VALUE_u]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4() -> @type[[TYPE_E1]] [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_E1]]) -> void>(%[[VALUE_foo_E]], int_to_enum<@type[[TYPE_E1]], reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<@type[[TYPE_E1]]>(%[[VALUE_e]], int_to_enum<@type[[TYPE_E1]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: @type[[TYPE_E1]] [storage=automatic] = int_to_enum<@type[[TYPE_E1]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return int_to_enum<@type[[TYPE_E1]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
