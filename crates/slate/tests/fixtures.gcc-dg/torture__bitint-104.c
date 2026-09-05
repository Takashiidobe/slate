/* PR tree-optimization/126490 */
/* { dg-do run { target bitint } } */

typedef unsigned _BitInt(1) T;

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] int
foo (T a, T b)
{
  return ((a & b) == (a ^ b)) + 1;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] int
bar (T a, T b)
{
  return ((a & b) == (a ^ b)) != 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] int
baz (T a, T b)
{
  return ((a & b) == (a ^ b)) == 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] int
qux (T a, T b)
{
  return ((a & b) == (a ^ b)) < 1;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] int
corge (int a, int b)
{
  return (a & b) == (a ^ b);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] int
garply (T a, T b)
{
  return ((a & b) ^ (a == b)) + 1;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] int
fred (T a, T b)
{
  return ((a & b) ^ (a == b)) != 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] int
xyzzy (T a, T b)
{
  return ((a & b) ^ (a == b)) == 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
[[gnu::noipa]] int
waldo (T a, T b)
{
  return ((a & b) ^ (a == b)) < 1;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int
main ()
{
  for (int i = 0; i < 4; ++i)
    {
      int a = i & 1;
      int b = i >> 1;
      int c = corge (a, b);
      if (foo (a, b) != c + 1
          || bar (a, b) != (c != 0)
          || baz (a, b) != (c == 0)
          || qux (a, b) != (c < 1)
	  || garply (a, b) != c + 1
          || fred (a, b) != (c != 0)
          || xyzzy (a, b) != (c == 0)
          || waldo (a, b) != (c < 1))
	__builtin_abort ();
    }
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn foo({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} ^ {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn bar({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} ^ {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn baz({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} ^ {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn qux({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} ^ {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn corge({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} & {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} ^ {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn garply({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn fred({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn xyzzy({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn waldo({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{_v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = 4;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:             if !{{_v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let mut a: i32 = 0;
// LOWERING-DAG:                 let mut b: i32 = 0;
// LOWERING-DAG:                 let mut c: i32 = 0;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-DAG:                 a = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} >> {{_v[0-9]+}};
// LOWERING-DAG:                 b = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = corge({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:                 c = {{_v[0-9]+}};
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG:                     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                         bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG:                     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                         bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = foo({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = bar({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = baz({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = qux({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = garply({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = fred({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = xyzzy({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = a;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = b;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> =
// LOWERING-DAG:                             bitint::BUint::<1, 1, 1>::from_i128({{_v[0-9]+}} as i128);
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = waldo({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = c;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                         {{_v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{_v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{_v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn foo({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// REWRITES-DAG:     (({{arg[0-9]+}} & {{arg[0-9]+}} == {{arg[0-9]+}} ^ {{arg[0-9]+}}) as i32) + 1
// REWRITES-DAG: }
// REWRITES-DAG: fn bar({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// REWRITES-DAG:     ((({{arg[0-9]+}} & {{arg[0-9]+}} == {{arg[0-9]+}} ^ {{arg[0-9]+}}) as i32) != 0) as i32
// REWRITES-DAG: }
// REWRITES-DAG: fn baz({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// REWRITES-DAG:     ((({{arg[0-9]+}} & {{arg[0-9]+}} == {{arg[0-9]+}} ^ {{arg[0-9]+}}) as i32) == 0) as i32
// REWRITES-DAG: }
// REWRITES-DAG: fn qux({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// REWRITES-DAG:     ((({{arg[0-9]+}} & {{arg[0-9]+}} == {{arg[0-9]+}} ^ {{arg[0-9]+}}) as i32) < 1) as i32
// REWRITES-DAG: }
// REWRITES-DAG: fn corge({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     ({{arg[0-9]+}} & {{arg[0-9]+}} == {{arg[0-9]+}} ^ {{arg[0-9]+}}) as i32
// REWRITES-DAG: }
// REWRITES-DAG: fn garply({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// REWRITES-DAG:     ({{_v[0-9]+}} ^ (({{arg[0-9]+}} == {{arg[0-9]+}}) as i32)) + 1
// REWRITES-DAG: }
// REWRITES-DAG: fn fred({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// REWRITES-DAG:     ({{_v[0-9]+}} ^ (({{arg[0-9]+}} == {{arg[0-9]+}}) as i32) != 0) as i32
// REWRITES-DAG: }
// REWRITES-DAG: fn xyzzy({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// REWRITES-DAG:     ({{_v[0-9]+}} ^ (({{arg[0-9]+}} == {{arg[0-9]+}}) as i32) == 0) as i32
// REWRITES-DAG: }
// REWRITES-DAG: fn waldo({{arg[0-9]+}}: bitint::BUint<1, 1, 1>, {{arg[0-9]+}}: bitint::BUint<1, 1, 1>) -> i32 {
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = {{arg[0-9]+}} & {{arg[0-9]+}};
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// REWRITES-DAG:     ({{_v[0-9]+}} ^ (({{arg[0-9]+}} == {{arg[0-9]+}}) as i32) < 1) as i32
// REWRITES-DAG: }
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     for i in 0..4 {
// REWRITES-DAG:         let mut a: i32 = i & 1;
// REWRITES-DAG:         let mut b: i32 = 0;
// REWRITES-DAG:         let mut c: i32 = 0;
// REWRITES-DAG:         let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         b = i >> {{_v[0-9]+}};
// REWRITES-DAG:         c = corge(a, b);
// REWRITES-DAG:         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(a as i128);
// REWRITES-DAG:         let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(b as i128);
// REWRITES-DAG:         let {{_v[0-9]+}}: i32 = foo({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != c + 1;
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(a as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(b as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: i32 = bar({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != ((c != 0) as i32);
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(a as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(b as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: i32 = baz({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != ((c == 0) as i32);
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(a as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(b as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: i32 = qux({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != ((c < 1) as i32);
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(a as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(b as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: i32 = garply({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != c + 1;
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(a as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(b as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: i32 = fred({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != ((c != 0) as i32);
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(a as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(b as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: i32 = xyzzy({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != ((c == 0) as i32);
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(a as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: bitint::BUint<1, 1, 1> = bitint::BUint::<1, 1, 1>::from_i128(b as i128);
// REWRITES-DAG:             let {{_v[0-9]+}}: i32 = waldo({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != ((c < 1) as i32);
// REWRITES-DAG:             {{_v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{_v[0-9]+}} {
// REWRITES-DAG:             unsafe { std::process::abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
