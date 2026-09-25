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
// DEFAULT-NEXT:     global %42 .str42: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%36 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @malloc(%37 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @free(%38 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @make(%5 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%5))), const<u64>(4))));
// DEFAULT-NEXT:         for %39
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %7 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%44));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%6), read<i32>(%7))), mul<i32, overflow=ub>(read<i32>(%7), read<i32>(%7)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i32>>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @make8() -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         for %40
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %10 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %45: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %46: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%45), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%46));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%10))), read<i32>(%10));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i32>>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @make_raw(%12 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%12))), const<u64>(4))));
// DEFAULT-NEXT:         for %41
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %14 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), read<i32>(%12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%47), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%48));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%13), read<i32>(%14))), add<i32, overflow=ub>(read<i32>(%14), const<i32>(1)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i32>>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @read_first(%16 p: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%16), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @make_shadow(%18 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%18))), const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%19), const<i32>(0))), const<i32>(9));
// DEFAULT-NEXT:         return read<ptr<i32>>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @custom_free(%21 p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<ptr<void>>(%21);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @maybe(%23 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%23), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return null<ptr<i32>>;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return pointer_cast<ptr<i32>, reason=return>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%23))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @allocfree(%25 n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26 p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%25))), const<u64>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%26)));
// DEFAULT-NEXT:         return pointer_cast<ptr<i32>, reason=return>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%25))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %28 q: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%4, const<i32>(4));
// DEFAULT-NEXT:         let %29 r: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn() -> ptr<i32>>(%8);
// DEFAULT-NEXT:         let %30 m: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%22, const<i32>(2));
// DEFAULT-NEXT:         let %31 a: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%24, const<i32>(3));
// DEFAULT-NEXT:         let %32 raw: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%11, const<i32>(2));
// DEFAULT-NEXT:         let %33 raw2: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%11, const<i32>(2));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%42)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%28), const<i32>(3)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%29), const<i32>(7)))), from_bool<i32, reason=explicit>(ne<ptr<i32>>(read<ptr<i32>>(%30), null<ptr<i32>>)), from_bool<i32, reason=explicit>(ne<ptr<i32>>(read<ptr<i32>>(%31), null<ptr<i32>>)), call<i32, signature=fn(ptr<i32>) -> i32>(%15, read<ptr<i32>>(%32)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%33), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%28)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%29)));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%30), null<ptr<i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%30)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%31)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%32)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%33)));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %34 free: ptr<fn(ptr<void>) -> void> [storage=automatic] = function_decay<ptr<fn(ptr<void>) -> void>>(%20);
// DEFAULT-NEXT:             let %35 shadow: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%17, const<i32>(1));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(read<ptr<fn(ptr<void>) -> void>>(%34), pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%35)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
