// { dg-do run }
// { dg-shouldfail "asan" }

int *bar(int *x, int *y) { return y; }

int foo(void) {
  char *p;
  {
    char a = 0;
    p      = &a;
  }

  if (*p)
    return 1;
  else
    return 0;
}

int main(void) {
  char *ptr;
  {
    char my_char[9];
    ptr = &my_char[0];
  }

  int  a[16];
  int *p, *q = a;
  {
    int b[16];
    p = bar(a, b);
  }
  bar(a, q);
  {
    int c[16];
    q = bar(a, c);
  }
  int v = *bar(a, q);
  return v;
}

// { dg-output "ERROR: AddressSanitizer: stack-use-after-scope on address.*(\n|\r\n|\r)" }
// { dg-output "READ of size 4 at.*" }
// { dg-output ".*'c' \\(line 37\\) <== Memory access at offset \[0-9\]* is inside this variable.*" }



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
// DEFAULT-NEXT:     fn %0 @bar(%1 x: ptr<i32>, %2 y: ptr<i32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<i32>>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %5 a: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             write<ptr<i8>>(%4, addr_of<ptr<i8>>(%5));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i8>(read<i8>(deref(read<ptr<i8>>(%4))), const<i8>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 ptr: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %8 my_char: array<i8, 9> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<i8>>(%7, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(%8), const<i32>(0)))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %9 a: array<i32, 16> [storage=automatic];
// DEFAULT-NEXT:         let %10 p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %11 q: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(16)>(%9);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %12 b: array<i32, 16> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<i32>>(%10, call<ptr<i32>, signature=fn(ptr<i32>, ptr<i32>) -> ptr<i32>>(%0, array_decay<ptr<i32>, length=Some(16)>(%9), array_decay<ptr<i32>, length=Some(16)>(%12)));
// DEFAULT-NEXT:             call<ptr<i32>, signature=fn(ptr<i32>, ptr<i32>) -> ptr<i32>>(%0, array_decay<ptr<i32>, length=Some(16)>(%9), array_decay<ptr<i32>, length=Some(16)>(%12));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<ptr<i32>, signature=fn(ptr<i32>, ptr<i32>) -> ptr<i32>>(%0, array_decay<ptr<i32>, length=Some(16)>(%9), read<ptr<i32>>(%11));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %13 c: array<i32, 16> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<i32>>(%11, call<ptr<i32>, signature=fn(ptr<i32>, ptr<i32>) -> ptr<i32>>(%0, array_decay<ptr<i32>, length=Some(16)>(%9), array_decay<ptr<i32>, length=Some(16)>(%13)));
// DEFAULT-NEXT:             call<ptr<i32>, signature=fn(ptr<i32>, ptr<i32>) -> ptr<i32>>(%0, array_decay<ptr<i32>, length=Some(16)>(%9), array_decay<ptr<i32>, length=Some(16)>(%13));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %14 v: i32 [storage=automatic] = read<i32>(deref(call<ptr<i32>, signature=fn(ptr<i32>, ptr<i32>) -> ptr<i32>>(%0, array_decay<ptr<i32>, length=Some(16)>(%9), read<ptr<i32>>(%11))));
// DEFAULT-NEXT:         return read<i32>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
