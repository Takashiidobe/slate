/* Copyright (C) 2000  Free Software Foundation  */
/* by Alexandre Oliva <aoliva@redhat.com> */

#include <stdlib.h>

void bar(int);
void foo(int *);

int main() {
  static int a[] = {0, 1, 2};
  int       *i   = &a[sizeof(a) / sizeof(*a)];

  while (i-- > a)
    foo(i);

  exit(0);
}

void baz(int, int);

void bar(int i) { baz(i, i); }
void foo(int *i) { bar(*i); }

void baz(int i, int j) {
  if (i != j)
    abort();
}



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
// DEFAULT-NEXT:     global %5 a: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%12 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @bar(%8 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%7, read<i32>(%8), read<i32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo(%9 i: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, read<i32>(deref(read<ptr<i32>>(%9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 i: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%5), div<u64, by_zero=ub>(const<u64>(12), const<u64>(4)))));
// DEFAULT-NEXT:         while %15 {
// DEFAULT-NEXT:             let %18: ptr<i32> [synthetic] = read<ptr<i32>>(%6);
// DEFAULT-NEXT:             let %19: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i32>>(%6, read<ptr<i32>>(%19));
// DEFAULT-NEXT:             yield gt<ptr<i32>>(read<ptr<i32>>(%18), array_decay<ptr<i32>, length=Some(3)>(%5));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i32>) -> void>(%3, read<ptr<i32>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @baz(%10 i: i32, %11 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), read<i32>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
