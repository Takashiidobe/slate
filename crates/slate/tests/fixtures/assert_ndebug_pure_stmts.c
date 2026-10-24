#define NDEBUG
#include <assert.h>
#include <stdio.h>

#define CHECK(x) do { assert(x); } while (0)

static int calls;

static int bump(void) {
  calls++;
  return calls;
}

// @check-fn-begin
static int pick(int key, int limit) {
  int result;
  assert(limit > 0);
  if (key < limit) {
    assert(key >= 0);
    result = key * 2;
  } else {
    assert(key >= limit);
    // the fallback keeps the key unchanged
    assert(limit != 0);
    result = key;
  }
  if (result > 1) {
    result--;
  } else {
    // a small result is already final
    assert(result <= 1);
  }
  if (result) {
    assert(result > 0);
  }
  {
    assert(key == key);
  }
  CHECK(result >= 0);
  (void)limit;
  (void)bump();
  key;
  return result;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d\n", pick(1, 4), pick(7, 4), calls);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn pick(key: i32, limit: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = if key < limit {
// CHECK-NEXT:         key * 2
// CHECK-NEXT:     } else {
// CHECK-NEXT:         // the fallback keeps the key unchanged
// CHECK-NEXT:         key
// CHECK-NEXT:     };
// CHECK-NEXT:     if result > 1 {
// CHECK-NEXT:         result -= 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     // a small result is already final
// CHECK-NEXT:     while false {}
// CHECK-NEXT:     bump();
// CHECK-NEXT:     result
// CHECK: {{^}}}
// SLATE-FILECHECK-END
