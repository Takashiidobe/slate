/* { dg-do run } */
/* { dg-options "-std=c23 -Wunused-value" } */

#include <stdarg.h>

const unsigned char a[] = {
#embed __FILE__     limit(128)
};

// @lowering-fn-begin
// @rewrite-fn-begin
int foo(...) {
  va_list ap;
  va_start(ap);
  for (int i = 0; i < 128; ++i)
    if (va_arg(ap, int) != a[i]) {
      va_end(ap);
      return 1;
    }
  va_end(ap);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

int b, c;

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  if (foo(
#embed __FILE__ limit(128)
          ))
    __builtin_abort();
  b = (
#embed __FILE__ limit(128) prefix(c = 2 *) suffix(                             \
    +6) /* { dg-warning "right-hand operand of comma expression has no effect" } */
  );
  if (b != a[127] + 6 || c != 2 * a[0])
    __builtin_abort();
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: unsafe fn foo(mut __slate_va_args: __SlateVaArgs) -> i32 {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         ap = __slate_va_args.clone();
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 128;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { ap.next_arg::<i32>() };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                 let {{__v[0-9]+}}: u8 = unsafe { (*a)[({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                 if {{__v[0-9]+}} {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                     __retval = {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                     return {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     __retval = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 47;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 42;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 123;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 103;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 45;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 111;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 114;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 117;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 110;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 125;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 42;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 47;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 10;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 47;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 42;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 123;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 103;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 45;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 111;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 112;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 116;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 105;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 111;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 110;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 115;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 34;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 45;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 115;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 116;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 61;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 99;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 50;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 51;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 45;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 87;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 117;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 110;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 117;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 115;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 101;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 45;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 118;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 97;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 108;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 117;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 101;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 34;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 125;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 42;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 47;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 10;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 10;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 35;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 105;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 110;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 99;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 108;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 117;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 101;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 60;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 115;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 116;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 97;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 114;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 103;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 46;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 104;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 62;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 10;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 10;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 99;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 111;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 110;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 115;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 116;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 117;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 110;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 115;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 105;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 103;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 110;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 101;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 99;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 104;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 97;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 114;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 97;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 91;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 93;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 61;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 123;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 10;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 35;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 101;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 109;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 98;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 101;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 100;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 95;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 95;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:             foo(__SlateVaArgs::new(vec![
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-DAG:             ]))
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 47;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 95;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 6;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { b };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 127;
// LOWERING-DAG:         let {{__v[0-9]+}}: u8 = unsafe { (*a)[({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 6;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: u8 = unsafe { (*a)[({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe fn foo(mut __slate_va_args: __SlateVaArgs) -> i32 {
// REWRITES-DAG:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         ap = __slate_va_args.clone();
// REWRITES-DAG:     }
// REWRITES-DAG:     for i in 0..128 {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { ap.next_arg::<i32>() };
// REWRITES-DAG:         if {{__v[0-9]+}} != ((unsafe { (*a)[((i as i64) as usize)] }) as i32) {
// REWRITES-DAG:             return 1;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     0
// REWRITES-DAG: }
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         foo(__SlateVaArgs::new(vec![
// REWRITES-DAG:             __SlateVaArg::new(47 as i32),
// REWRITES-DAG:             __SlateVaArg::new(42 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(123 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(100 as i32),
// REWRITES-DAG:             __SlateVaArg::new(103 as i32),
// REWRITES-DAG:             __SlateVaArg::new(45 as i32),
// REWRITES-DAG:             __SlateVaArg::new(100 as i32),
// REWRITES-DAG:             __SlateVaArg::new(111 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(114 as i32),
// REWRITES-DAG:             __SlateVaArg::new(117 as i32),
// REWRITES-DAG:             __SlateVaArg::new(110 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(125 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(42 as i32),
// REWRITES-DAG:             __SlateVaArg::new(47 as i32),
// REWRITES-DAG:             __SlateVaArg::new(10 as i32),
// REWRITES-DAG:             __SlateVaArg::new(47 as i32),
// REWRITES-DAG:             __SlateVaArg::new(42 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(123 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(100 as i32),
// REWRITES-DAG:             __SlateVaArg::new(103 as i32),
// REWRITES-DAG:             __SlateVaArg::new(45 as i32),
// REWRITES-DAG:             __SlateVaArg::new(111 as i32),
// REWRITES-DAG:             __SlateVaArg::new(112 as i32),
// REWRITES-DAG:             __SlateVaArg::new(116 as i32),
// REWRITES-DAG:             __SlateVaArg::new(105 as i32),
// REWRITES-DAG:             __SlateVaArg::new(111 as i32),
// REWRITES-DAG:             __SlateVaArg::new(110 as i32),
// REWRITES-DAG:             __SlateVaArg::new(115 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(34 as i32),
// REWRITES-DAG:             __SlateVaArg::new(45 as i32),
// REWRITES-DAG:             __SlateVaArg::new(115 as i32),
// REWRITES-DAG:             __SlateVaArg::new(116 as i32),
// REWRITES-DAG:             __SlateVaArg::new(100 as i32),
// REWRITES-DAG:             __SlateVaArg::new(61 as i32),
// REWRITES-DAG:             __SlateVaArg::new(99 as i32),
// REWRITES-DAG:             __SlateVaArg::new(50 as i32),
// REWRITES-DAG:             __SlateVaArg::new(51 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(45 as i32),
// REWRITES-DAG:             __SlateVaArg::new(87 as i32),
// REWRITES-DAG:             __SlateVaArg::new(117 as i32),
// REWRITES-DAG:             __SlateVaArg::new(110 as i32),
// REWRITES-DAG:             __SlateVaArg::new(117 as i32),
// REWRITES-DAG:             __SlateVaArg::new(115 as i32),
// REWRITES-DAG:             __SlateVaArg::new(101 as i32),
// REWRITES-DAG:             __SlateVaArg::new(100 as i32),
// REWRITES-DAG:             __SlateVaArg::new(45 as i32),
// REWRITES-DAG:             __SlateVaArg::new(118 as i32),
// REWRITES-DAG:             __SlateVaArg::new(97 as i32),
// REWRITES-DAG:             __SlateVaArg::new(108 as i32),
// REWRITES-DAG:             __SlateVaArg::new(117 as i32),
// REWRITES-DAG:             __SlateVaArg::new(101 as i32),
// REWRITES-DAG:             __SlateVaArg::new(34 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(125 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(42 as i32),
// REWRITES-DAG:             __SlateVaArg::new(47 as i32),
// REWRITES-DAG:             __SlateVaArg::new(10 as i32),
// REWRITES-DAG:             __SlateVaArg::new(10 as i32),
// REWRITES-DAG:             __SlateVaArg::new(35 as i32),
// REWRITES-DAG:             __SlateVaArg::new(105 as i32),
// REWRITES-DAG:             __SlateVaArg::new(110 as i32),
// REWRITES-DAG:             __SlateVaArg::new(99 as i32),
// REWRITES-DAG:             __SlateVaArg::new(108 as i32),
// REWRITES-DAG:             __SlateVaArg::new(117 as i32),
// REWRITES-DAG:             __SlateVaArg::new(100 as i32),
// REWRITES-DAG:             __SlateVaArg::new(101 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(60 as i32),
// REWRITES-DAG:             __SlateVaArg::new(115 as i32),
// REWRITES-DAG:             __SlateVaArg::new(116 as i32),
// REWRITES-DAG:             __SlateVaArg::new(100 as i32),
// REWRITES-DAG:             __SlateVaArg::new(97 as i32),
// REWRITES-DAG:             __SlateVaArg::new(114 as i32),
// REWRITES-DAG:             __SlateVaArg::new(103 as i32),
// REWRITES-DAG:             __SlateVaArg::new(46 as i32),
// REWRITES-DAG:             __SlateVaArg::new(104 as i32),
// REWRITES-DAG:             __SlateVaArg::new(62 as i32),
// REWRITES-DAG:             __SlateVaArg::new(10 as i32),
// REWRITES-DAG:             __SlateVaArg::new(10 as i32),
// REWRITES-DAG:             __SlateVaArg::new(99 as i32),
// REWRITES-DAG:             __SlateVaArg::new(111 as i32),
// REWRITES-DAG:             __SlateVaArg::new(110 as i32),
// REWRITES-DAG:             __SlateVaArg::new(115 as i32),
// REWRITES-DAG:             __SlateVaArg::new(116 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(117 as i32),
// REWRITES-DAG:             __SlateVaArg::new(110 as i32),
// REWRITES-DAG:             __SlateVaArg::new(115 as i32),
// REWRITES-DAG:             __SlateVaArg::new(105 as i32),
// REWRITES-DAG:             __SlateVaArg::new(103 as i32),
// REWRITES-DAG:             __SlateVaArg::new(110 as i32),
// REWRITES-DAG:             __SlateVaArg::new(101 as i32),
// REWRITES-DAG:             __SlateVaArg::new(100 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(99 as i32),
// REWRITES-DAG:             __SlateVaArg::new(104 as i32),
// REWRITES-DAG:             __SlateVaArg::new(97 as i32),
// REWRITES-DAG:             __SlateVaArg::new(114 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(97 as i32),
// REWRITES-DAG:             __SlateVaArg::new(91 as i32),
// REWRITES-DAG:             __SlateVaArg::new(93 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(61 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(123 as i32),
// REWRITES-DAG:             __SlateVaArg::new(10 as i32),
// REWRITES-DAG:             __SlateVaArg::new(35 as i32),
// REWRITES-DAG:             __SlateVaArg::new(101 as i32),
// REWRITES-DAG:             __SlateVaArg::new(109 as i32),
// REWRITES-DAG:             __SlateVaArg::new(98 as i32),
// REWRITES-DAG:             __SlateVaArg::new(101 as i32),
// REWRITES-DAG:             __SlateVaArg::new(100 as i32),
// REWRITES-DAG:             __SlateVaArg::new(32 as i32),
// REWRITES-DAG:             __SlateVaArg::new(95 as i32),
// REWRITES-DAG:             __SlateVaArg::new(95 as i32),
// REWRITES-DAG:             __SlateVaArg::new(70 as i32),
// REWRITES-DAG:             __SlateVaArg::new(73 as i32),
// REWRITES-DAG:         ]))
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 47;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         c = 2 * {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 6;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         b = 73 + {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if (unsafe { b }) != ((unsafe { (*a)[127] }) as i32) + 6 {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { c }) != 2 * ((unsafe { (*a)[0] }) as i32);
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
