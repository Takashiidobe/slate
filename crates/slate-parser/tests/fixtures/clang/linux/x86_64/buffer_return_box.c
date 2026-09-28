#include <stdio.h>
#include <stdlib.h>

static int *make(int n) {
  int *p = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    p[i] = i * i;
  }
  return p;
}

static int *make8(void) {
  int *p = malloc(8 * sizeof(int));
  for (int i = 0; i < 8; i++) {
    p[i] = i;
  }
  return p;
}

static int *make_raw(int n) {
  int *p = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    p[i] = i + 1;
  }
  return p;
}

static int read_first(int *p) { return p[0]; }

static int *make_shadow(int n) {
  int *p = malloc(n * sizeof(int));
  p[0]   = 9;
  return p;
}

static void custom_free(void *p) { (void)p; }

static int *maybe(int n) {
  if (n < 0) {
    return NULL;
  }
  return malloc(n * sizeof(int));
}

static int *allocfree(int n) {
  int *p = malloc(n * sizeof(int));
  free(p);
  return malloc(n * sizeof(int));
}

int main(void) {
  int *q    = make(4);
  int *r    = make8();
  int *m    = maybe(2);
  int *a    = allocfree(3);
  int *raw  = make_raw(2);
  int *raw2 = make_raw(2);
  printf("%d %d %d %d %d %d\n", q[3], r[7], (int)(m != NULL), (int)(a != NULL),
         read_first(raw), raw2[1]);
  free(q);
  free(r);
  if (m) {
    free(m);
  }
  free(a);
  free(raw);
  free(raw2);
  {
    void (*free)(void *) = custom_free;
    int *shadow          = make_shadow(1);
    free(shadow);
  }
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
// DEFAULT-NEXT:     global %45 .str45: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%39 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%40 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @free(%41 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @make(%8 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%8))), const<u64>(4))));
// DEFAULT-NEXT:         for %42
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %10 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), read<i32>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %46: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%47));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%10))), mul<i32, overflow=ub>(read<i32>(%10), read<i32>(%10)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i32>>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @make8() -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         for %43
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %13 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%49));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%12), read<i32>(%13))), read<i32>(%13));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i32>>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @make_raw(%15 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%15))), const<u64>(4))));
// DEFAULT-NEXT:         for %44
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %17 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), read<i32>(%15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%51));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%16), read<i32>(%17))), add<i32, overflow=ub>(read<i32>(%17), const<i32>(1)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i32>>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @read_first(%19 p: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%19), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @make_shadow(%21 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %22 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%21))), const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%22), const<i32>(0))), const<i32>(9));
// DEFAULT-NEXT:         return read<ptr<i32>>(%22);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @custom_free(%24 p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<ptr<void>>(%24);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @maybe(%26 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%26), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return null<ptr<i32>>;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return pointer_cast<ptr<i32>, reason=return>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%26))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @allocfree(%28 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %29 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%28))), const<u64>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%29)));
// DEFAULT-NEXT:         return pointer_cast<ptr<i32>, reason=return>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%28))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %31 q: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%7, const<i32>(4));
// DEFAULT-NEXT:         let %32 r: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn() -> ptr<i32>>(%11);
// DEFAULT-NEXT:         let %33 m: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%25, const<i32>(2));
// DEFAULT-NEXT:         let %34 a: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%27, const<i32>(3));
// DEFAULT-NEXT:         let %35 raw: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%14, const<i32>(2));
// DEFAULT-NEXT:         let %36 raw2: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%14, const<i32>(2));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%45)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%31), const<i32>(3)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%32), const<i32>(7)))), from_bool<i32, reason=explicit>(ne<ptr<i32>>(read<ptr<i32>>(%33), null<ptr<i32>>)), from_bool<i32, reason=explicit>(ne<ptr<i32>>(read<ptr<i32>>(%34), null<ptr<i32>>)), call<i32, signature=fn(ptr<i32>) -> i32>(%18, read<ptr<i32>>(%35)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%36), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%31)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%32)));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%33), null<ptr<i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%33)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%34)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%35)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%36)));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %37 free: ptr<fn(ptr<void>) -> void> [storage=automatic] = function_decay<ptr<fn(ptr<void>) -> void>>(%23);
// DEFAULT-NEXT:             let %38 shadow: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%20, const<i32>(1));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(read<ptr<fn(ptr<void>) -> void>>(%37), pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%38)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
