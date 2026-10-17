typedef enum { REPEAT_NONE, REPEAT_CHECK, REPEAT_VALID } FSE_repeat;

typedef struct {
  FSE_repeat mode;
} Entropy;

// @check-fn-begin
static unsigned select_type(FSE_repeat *repeatMode, unsigned count) {
  if (count == 0) {
    *repeatMode = REPEAT_NONE;
    return 0;
  }
  return (unsigned)*repeatMode + count;
}
// @check-fn-end

// @check-fn-begin
int run(Entropy *e, unsigned count) {
  return (int)select_type(&e->mode, count);
}
// @check-fn-end

int main(void) {
  Entropy e;
  e.mode = REPEAT_VALID;
  return run(&e, 0) + run(&e, 5);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn select_type(repeatMode: *mut u32, count: u32) -> u32 {
// CHECK-NEXT:     if count == 0 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *repeatMode = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     ((unsafe { *repeatMode }) as u32).wrapping_add(count)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn run(e: &mut {{__SlateRecord[0-9]+}}, count: u32) -> i32 {
// CHECK-NEXT:     select_type(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!((* (e as * mut [[SLATERECORD_0:__SlateRecord[0-9]+]])).mode) },
// CHECK-NEXT:         count,
// CHECK-NEXT:     ) as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
