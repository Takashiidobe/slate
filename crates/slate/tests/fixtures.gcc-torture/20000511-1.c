void abort(void);
void exit(int);

void f(int value, int expect) {
  if (value != expect)
    abort();
}

int main() {
  int a = 7, b = 6, c = 4, d = 7, e = 2;

  f(a || b % c, 1);
  f(a ? b % c : 0, 2);
  f(a = b % c, 2);
  f(a *= b % c, 4);
  f(a /= b % c, 2);
  f(a %= b % c, 0);
  f(a += b % c, 2);
  f(d || c && e, 1);
  f(d ? c && e : 0, 1);
  f(d = c && e, 1);
  f(d *= c && e, 1);
  f(d %= c && e, 0);
  f(d += c && e, 1);
  f(d -= c && e, 0);
  f(d || c || e, 1);
  f(d ? c || e : 0, 0);
  f(d = c || e, 1);
  f(d *= c || e, 1);
  f(d %= c || e, 0);
  f(d += c || e, 1);
  f(d -= c || e, 0);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut value: i32, mut expect: i32) {
// CHECK-NEXT:     if value != expect {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 7 as i32;
// CHECK-NEXT:     let mut b: i32 = 6 as i32;
// CHECK-NEXT:     let mut c: i32 = 4 as i32;
// CHECK-NEXT:     let mut d: i32 = 7 as i32;
// CHECK-NEXT:     let mut e: i32 = 2 as i32;
// CHECK-NEXT:     f((a != (0 as i32) || b % c != (0 as i32)) as i32, 1 as i32);
// CHECK-NEXT:     f(if a != (0 as i32) { b % c } else { 0 as i32 }, 2 as i32);
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = b % c;
// CHECK-NEXT:     a = [[V_1]];
// CHECK-NEXT:     f([[V_1]], 2 as i32);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] * (b % c);
// CHECK-NEXT:     a = [[V_3]];
// CHECK-NEXT:     f([[V_3]], 4 as i32);
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] / (b % c);
// CHECK-NEXT:     a = [[V_5]];
// CHECK-NEXT:     f([[V_5]], 2 as i32);
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] % (b % c);
// CHECK-NEXT:     a = [[V_7]];
// CHECK-NEXT:     f([[V_7]], 0 as i32);
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + b % c;
// CHECK-NEXT:     a = [[V_9]];
// CHECK-NEXT:     f([[V_9]], 2 as i32);
// CHECK-NEXT:     f((d != (0 as i32) || c != (0 as i32) && e != (0 as i32)) as i32, 1 as i32);
// CHECK-NEXT:     f(
// CHECK-NEXT:         if d != (0 as i32) {
// CHECK-NEXT:             (c != (0 as i32) && e != (0 as i32)) as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0 as i32
// CHECK-NEXT:         },
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: i32 = (c != (0 as i32) && e != (0 as i32)) as i32;
// CHECK-NEXT:     d = [[V_10]];
// CHECK-NEXT:     f([[V_10]], 1 as i32);
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: i32 = [[V_11]] * ((c != (0 as i32) && e != (0 as i32)) as i32);
// CHECK-NEXT:     d = [[V_12]];
// CHECK-NEXT:     f([[V_12]], 1 as i32);
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: i32 = [[V_13]] % ((c != (0 as i32) && e != (0 as i32)) as i32);
// CHECK-NEXT:     d = [[V_14]];
// CHECK-NEXT:     f([[V_14]], 0 as i32);
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: i32 = [[V_15]] + ((c != (0 as i32) && e != (0 as i32)) as i32);
// CHECK-NEXT:     d = [[V_16]];
// CHECK-NEXT:     f([[V_16]], 1 as i32);
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:     let mut [[V_18:__v[0-9]+]]: i32 = [[V_17]] - ((c != (0 as i32) && e != (0 as i32)) as i32);
// CHECK-NEXT:     d = [[V_18]];
// CHECK-NEXT:     f([[V_18]], 0 as i32);
// CHECK-NEXT:     f((d != (0 as i32) || c != (0 as i32) || e != (0 as i32)) as i32, 1 as i32);
// CHECK-NEXT:     f(
// CHECK-NEXT:         if d != (0 as i32) {
// CHECK-NEXT:             (c != (0 as i32) || e != (0 as i32)) as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0 as i32
// CHECK-NEXT:         },
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: i32 = (c != (0 as i32) || e != (0 as i32)) as i32;
// CHECK-NEXT:     d = [[V_19]];
// CHECK-NEXT:     f([[V_19]], 1 as i32);
// CHECK-NEXT:     let mut [[V_20:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: i32 = [[V_20]] * ((c != (0 as i32) || e != (0 as i32)) as i32);
// CHECK-NEXT:     d = [[V_21]];
// CHECK-NEXT:     f([[V_21]], 1 as i32);
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:     let mut [[V_23:__v[0-9]+]]: i32 = [[V_22]] % ((c != (0 as i32) || e != (0 as i32)) as i32);
// CHECK-NEXT:     d = [[V_23]];
// CHECK-NEXT:     f([[V_23]], 0 as i32);
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:     let mut [[V_25:__v[0-9]+]]: i32 = [[V_24]] + ((c != (0 as i32) || e != (0 as i32)) as i32);
// CHECK-NEXT:     d = [[V_25]];
// CHECK-NEXT:     f([[V_25]], 1 as i32);
// CHECK-NEXT:     let mut [[V_26:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:     let mut [[V_27:__v[0-9]+]]: i32 = [[V_26]] - ((c != (0 as i32) || e != (0 as i32)) as i32);
// CHECK-NEXT:     d = [[V_27]];
// CHECK-NEXT:     f([[V_27]], 0 as i32);
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
