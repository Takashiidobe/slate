void foo(int x, int y, int z, int d, int *buf) {
  for (int i = z; i < y - z; ++i)
    for (int j = 0; j < d; ++j)
      /* buf[x(i+1) + j] = buf[x(i+1)-j-1] */
      buf[i * x + (x - z + j)] = buf[i * x + (x - z - 1 - j)];
}

void bar(int x, int y, int z, int d, int *buf) {
  for (int i = 0; i < d; ++i)
    for (int j = z; j < x - z; ++j)
      /* buf[j+(y+i)*x] = buf[j+(y-1-i)*x] */
      buf[j + (y - z + i) * x] = buf[j + (y - z - 1 - i) * x];
}

__attribute__((noipa)) void baz(int x, int y, int d, int *buf) {
  foo(x, y, 0, d, buf);
  bar(x, y, 0, d, buf);
}

int main(void) {
  int a[] = {1, 2, 3};
  baz(1, 2, 1, a);
  /* foo does:
     buf[1] = buf[0];
     buf[2] = buf[1];

     bar does:
     buf[2] = buf[1]; (no-op)
     so we should have { 1, 1, 1 }.  */
  for (int i = 0; i < 3; i++)
    if (a[i] != 1)
      __builtin_abort();
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i32, %2 y: i32, %3 z: i32, %4 d: i32, %5 buf: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 i: i32 [storage=automatic] = read<i32>(%3);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), sub<i32, overflow=ub>(read<i32>(%2), read<i32>(%3)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%31));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %25
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %7 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%7), read<i32>(%4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %32: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%7, read<i32>(%33));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%6), read<i32>(%1)), add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%1), read<i32>(%3)), read<i32>(%7))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%6), read<i32>(%1)), sub<i32, overflow=ub>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%1), read<i32>(%3)), const<i32>(1)), read<i32>(%7)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @bar(%9 x: i32, %10 y: i32, %11 z: i32, %12 d: i32, %13 buf: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %14 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), read<i32>(%12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%35));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %27
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %15 j: i32 [storage=automatic] = read<i32>(%11);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%15), sub<i32, overflow=ub>(read<i32>(%9), read<i32>(%11)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %36: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                         let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%15, read<i32>(%37));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%13), add<i32, overflow=ub>(read<i32>(%15), mul<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%10), read<i32>(%11)), read<i32>(%14)), read<i32>(%9))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%13), add<i32, overflow=ub>(read<i32>(%15), mul<i32, overflow=ub>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%10), read<i32>(%11)), const<i32>(1)), read<i32>(%14)), read<i32>(%9)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @baz(%17 x: i32, %18 y: i32, %19 d: i32, %20 buf: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ptr<i32>) -> void>(%0, read<i32>(%17), read<i32>(%18), const<i32>(0), read<i32>(%19), read<ptr<i32>>(%20));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ptr<i32>) -> void>(%8, read<i32>(%17), read<i32>(%18), const<i32>(0), read<i32>(%19), read<ptr<i32>>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %22 a: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, ptr<i32>) -> void>(%16, const<i32>(1), const<i32>(2), const<i32>(1), array_decay<ptr<i32>, length=Some(3)>(%22));
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %23 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%23), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%39));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%22), read<i32>(%23)))), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
