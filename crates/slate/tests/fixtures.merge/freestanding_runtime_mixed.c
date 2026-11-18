static int counter;

int main(void) {
  counter += 7;
  return counter == 7 ? 0 : 1;
}

// merge-second-target: linux-i686 i686-unknown-linux-gnu clang
// merge-linux-args: -ffreestanding
// merge-expect-exit: 1

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}error: freestanding_runtime_mixed: hosted on i686-unknown-linux-gnu but freestanding on x86_64-unknown-linux-gnu; --merge needs every target hosted or every target freestanding
// CHECK-NEXT: exit: 1
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
