void abort(void);
void exit(int);

typedef long mpt;

int f(mpt us, mpt vs) {
  long aus;
  long avs;

  aus = us >= 0 ? us : -us;
  avs = vs >= 0 ? vs : -vs;

  if (aus < avs) {
    long t = aus;
    aus    = avs;
    avs    = aus;
  }

  return avs;
}

int main(void) {
  if (f((mpt)3, (mpt)17) != 17)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut us: i64, mut vs: i64) -> i32 {
// CHECK-NEXT:     let mut aus: i64 = if us >= ((0 as i32) as i64) { us } else { -us };
// CHECK-NEXT:     let mut avs: i64 = if vs >= ((0 as i32) as i64) { vs } else { -vs };
// CHECK-NEXT:     if aus < avs {
// CHECK-NEXT:         aus = avs;
// CHECK-NEXT:         avs = aus;
// CHECK-NEXT:     }
// CHECK-NEXT:     avs as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f((3 as i32) as i64, (17 as i32) as i64) != (17 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
