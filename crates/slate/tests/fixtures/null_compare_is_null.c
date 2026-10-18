#include <stddef.h>
#include <stdio.h>

struct Node {
  int v;
  struct Node *next;
};

static int (*hook)(int);

// @check-fn-begin
static int count(struct Node *head, const char *tag) {
  int n = 0;
  if (tag != NULL && NULL != head)
    n += 100;
  if (tag == NULL)
    n += 1000;
  while (head != NULL) {
    n += head->v;
    if (NULL == head->next)
      break;
    head = head->next;
  }
  if (!head || head->next == 0)
    n += 10;
  if (hook != NULL)
    n += hook(n);
  return n;
}
// @check-fn-end

int main(void) {
  struct Node c = {3, NULL};
  struct Node b = {2, &c};
  struct Node a = {1, &b};
  printf("%d %d %d\n", count(&a, "x"), count(NULL, NULL), count(&c, NULL));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn count(mut head: *mut Node, tag: *const i8) -> i32 {
// CHECK-NEXT:     let mut n: i32 = 0;
// CHECK-NEXT:     if !tag.is_null() && !head.is_null() {
// CHECK-NEXT:         n += 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     if tag.is_null() {
// CHECK-NEXT:         n += 1000;
// CHECK-NEXT:     }
// CHECK-NEXT:     while !head.is_null() {
// CHECK-NEXT:         n += unsafe { (*head).v };
// CHECK-NEXT:         if ((unsafe { (*head).next }) as *mut ()).is_null() {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         head = unsafe { (*head).next };
// CHECK-NEXT:     }
// CHECK-NEXT:     if head.is_null() || unsafe { (*head).next }.is_null() {
// CHECK-NEXT:         n += 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { hook }) != None {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe { unsafe { hook }.unwrap()(n) };
// CHECK-NEXT:         n += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     n
// CHECK: {{^}}}
// SLATE-FILECHECK-END
