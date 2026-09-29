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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_make:[0-9]+]] @make(%[[VALUE_n:[0-9]+]] n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n]]))), const<u64>(4))));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_i]]))), mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_make8:[0-9]+]] @make8() -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(4))));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_2]]), read<i32>(%[[VALUE_i_2]]))), read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE_p_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_make_raw:[0-9]+]] @make_raw(%[[VALUE_n_2:[0-9]+]] n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_2]]))), const<u64>(4))));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), read<i32>(%[[VALUE_n_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_3]]), read<i32>(%[[VALUE_i_3]]))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(1)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE_p_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_read_first:[0-9]+]] @read_first(%[[VALUE_p_4:[0-9]+]] p: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_4]]), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_make_shadow:[0-9]+]] @make_shadow(%[[VALUE_n_3:[0-9]+]] n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p_5:[0-9]+]] p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_3]]))), const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_5]]), const<i32>(0))), const<i32>(9));
// DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE_p_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_custom_free:[0-9]+]] @custom_free(%[[VALUE_p_6:[0-9]+]] p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<ptr<void>>(%[[VALUE_p_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_maybe:[0-9]+]] @maybe(%[[VALUE_n_4:[0-9]+]] n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_n_4]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return null<ptr<i32>>;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return pointer_cast<ptr<i32>, reason=return>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_4]]))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_allocfree:[0-9]+]] @allocfree(%[[VALUE_n_5:[0-9]+]] n: i32) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p_7:[0-9]+]] p: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_5]]))), const<u64>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_p_7]])));
// DEFAULT-NEXT:         return pointer_cast<ptr<i32>, reason=return>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n_5]]))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%[[VALUE_make]], const<i32>(4));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE_make8]]);
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%[[VALUE_maybe]], const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%[[VALUE_allocfree]], const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_raw:[0-9]+]] raw: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%[[VALUE_make_raw]], const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_raw2:[0-9]+]] raw2: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%[[VALUE_make_raw]], const<i32>(2));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_q]]), const<i32>(3)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_r]]), const<i32>(7)))), from_bool<i32, reason=explicit>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_m]]), null<ptr<i32>>)), from_bool<i32, reason=explicit>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_a]]), null<ptr<i32>>)), call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_read_first]], read<ptr<i32>>(%[[VALUE_raw]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_raw2]]), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_q]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_r]])));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_m]]), null<ptr<i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_m]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_raw]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_raw2]])));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_free_2:[0-9]+]] free: ptr<fn(ptr<void>) -> void> [storage=automatic] = function_decay<ptr<fn(ptr<void>) -> void>>(%[[VALUE_custom_free]]);
// DEFAULT-NEXT:             let %[[VALUE_shadow:[0-9]+]] shadow: ptr<i32> [storage=automatic] = call<ptr<i32>, signature=fn(i32) -> ptr<i32>>(%[[VALUE_make_shadow]], const<i32>(1));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(read<ptr<fn(ptr<void>) -> void>>(%[[VALUE_free_2]]), pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_shadow]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
