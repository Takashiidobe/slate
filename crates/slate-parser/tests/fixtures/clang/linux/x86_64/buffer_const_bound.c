#include <stdio.h>
#include <stdlib.h>

static int sum_fixed(int *p) {
  int total = 0;
  for (int i = 0; i < 4; i++) {
    total += p[i];
  }
  return total;
}

static void scale_fixed(int *p) {
  for (int i = 0; i < 3; i++) {
    p[i] = p[i] * 2;
  }
}

static int sum3(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  return t;
}

static int mix(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  t += p[5];
  return t;
}

int main(void) {
  int arr[4] = {10, 20, 30, 40};
  int r      = sum_fixed(arr);

  int a[3] = {1, 2, 3};
  scale_fixed(a);

  int *m = malloc(3 * sizeof(int));
  m[0]   = 5;
  m[1]   = 6;
  m[2]   = 7;
  int r2 = sum3(m);
  free(m);

  int big[6] = {1, 2, 3, 4, 5, 6};
  int r3     = mix(big);

  printf("%d %d %d %d %d\n", r, a[0], a[2], r2, r3);
  return 0;
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %37 .str37: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%30 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%31 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @free(%32 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @sum_fixed(%8 p: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %33
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %10 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%39));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %40: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                     let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%8), read<i32>(%10)))));
// DEFAULT-NEXT:                     write<i32>(%9, read<i32>(%41));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @scale_fixed(%12 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %34
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %13 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %42: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%43));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%12), read<i32>(%13))), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%12), read<i32>(%13)))), const<i32>(2)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @sum3(%15 p: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 t: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %35
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %17 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%45));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %46: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                     let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), read<i32>(%17)))));
// DEFAULT-NEXT:                     write<i32>(%16, read<i32>(%47));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @mix(%19 p: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 t: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %36
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %21 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%21), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32>(%49));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %50: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                     let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%19), read<i32>(%21)))));
// DEFAULT-NEXT:                     write<i32>(%20, read<i32>(%51));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %52: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:         let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%19), const<i32>(5)))));
// DEFAULT-NEXT:         write<i32>(%20, read<i32>(%53));
// DEFAULT-NEXT:         return read<i32>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %23 arr: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(20), index2 = const<i32>(30), index3 = const<i32>(40));
// DEFAULT-NEXT:         let %24 r: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>) -> i32>(%7, array_decay<ptr<i32>, length=Some(4)>(%23));
// DEFAULT-NEXT:         let %25 a: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%11, array_decay<ptr<i32>, length=Some(3)>(%25));
// DEFAULT-NEXT:         let %26 m: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%26), const<i32>(0))), const<i32>(5));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%26), const<i32>(1))), const<i32>(6));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%26), const<i32>(2))), const<i32>(7));
// DEFAULT-NEXT:         let %27 r2: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>) -> i32>(%14, read<ptr<i32>>(%26));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%26)));
// DEFAULT-NEXT:         let %28 big: array<i32, 6> [storage=automatic] [align=16] = aggregate<array<i32, 6>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4), index4 = const<i32>(5), index5 = const<i32>(6));
// DEFAULT-NEXT:         let %29 r3: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>) -> i32>(%18, array_decay<ptr<i32>, length=Some(6)>(%28));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%37)), read<i32>(%24), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%25), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%25), const<i32>(2)))), read<i32>(%27), read<i32>(%29));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
