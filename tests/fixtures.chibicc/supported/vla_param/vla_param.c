// Test VLA function parameters (C99 feature)
// VLA bounds in function parameters can reference earlier parameters.
// These array parameters decay to pointers.

#include "test.h"

// VLA with forward parameter reference (like regexec in regex.h)
int sum_array(int n, int arr[n]) {
  int sum = 0;
  for (int i = 0; i < n; i++)
    sum += arr[i];
  return sum;
}

// VLA with expression in bounds
int sum_doubled(int n, int arr[n * 2]) {
  int sum = 0;
  for (int i = 0; i < n * 2; i++)
    sum += arr[i];
  return sum;
}

// VLA with restrict qualifier
void copy_array(int n, int dst[restrict n], int src[restrict n]) {
  for (int i = 0; i < n; i++)
    dst[i] = src[i];
}

// VLA with static qualifier (indicates array has at least n elements)
int first_elem(int n, int arr[static n]) {
  return arr[0];
}

// Multiple VLA parameters with compound bounds
int dot_product(int n, int a[n], int b[n]) {
  int result = 0;
  for (int i = 0; i < n; i++)
    result += a[i] * b[i];
  return result;
}

// Function declaration (prototype) with VLA - should compile
void process_matrix(int rows, int cols, int mat[rows][cols]);

int main() {
  int arr5[] = {1, 2, 3, 4, 5};
  int arr10[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // Test basic VLA parameter
  ASSERT(15, sum_array(5, arr5));
  ASSERT(55, sum_array(10, arr10));

  // Test VLA with expression in bounds
  ASSERT(55, sum_doubled(5, arr10)); // n=5, so iterates n*2=10 elements

  // Test VLA with restrict
  int dst[5];
  copy_array(5, dst, arr5);
  ASSERT(1, dst[0]);
  ASSERT(5, dst[4]);

  // Test VLA with static
  ASSERT(1, first_elem(5, arr5));

  // Test multiple VLA params
  int a[] = {1, 2, 3};
  int b[] = {4, 5, 6};
  ASSERT(32, dot_product(3, a, b)); // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32

  printf("OK\n");
  return 0;
}
