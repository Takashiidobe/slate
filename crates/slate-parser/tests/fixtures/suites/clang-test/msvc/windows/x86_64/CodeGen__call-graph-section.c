// Tests that we assign appropriate identifiers to indirect calls and targets.



void foo(void) {
}

void bar(void) {
  void (*fp)(void) = foo;
  fp();
}

int baz(char a, float b, double c) {
  return 1;
}

int *qux(char *a, float *b, double *c) {
  return 0;
}

void corge(void) {
  int (*fp_baz)(char, float, double) = baz;  
  fp_baz('a', .0f, .0);

  int *(*fp_qux)(char *, float *, double *) = qux;  
  fp_qux(0, 0, 0);
}

struct st1 {
  int *(*fp)(char *, float *, double *);
};

struct st2 {
  struct st1 m;
};

void stparam(struct st2 a, struct st2 *b) {}

void stf(void) {
  struct st1 St1;
  St1.fp = qux;  
  St1.fp(0, 0, 0);

  struct st2 St2;
  St2.m.fp = qux;  
  St2.m.fp(0, 0, 0);
  
  void (*fp_stparam)(struct st2, struct st2 *) = stparam;
  fp_stparam(St2, &St2);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 st1 = struct {
// DEFAULT-NEXT:         field0 fp: ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 st2 = struct {
// DEFAULT-NEXT:         field0 m: @type0;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 fp: ptr<fn() -> void> [storage=automatic] = function_decay<ptr<fn() -> void>>(%0);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @baz(%4 a: i8, %5 b: f32, %6 c: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @qux(%8 a: ptr<i8>, %9 b: ptr<f32>, %10 c: ptr<f64>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<i32>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @corge() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 fp_baz: ptr<fn(i8, f32, f64) -> i32> [storage=automatic] = function_decay<ptr<fn(i8, f32, f64) -> i32>>(%3);
// DEFAULT-NEXT:         call<i32, signature=fn(i8, f32, f64) -> i32>(read<ptr<fn(i8, f32, f64) -> i32>>(%12), truncate<i8, reason=arg, fits=always>(const<i32>(97)), const<f32>(0.0), const<f64>(0.0));
// DEFAULT-NEXT:         let %13 fp_qux: ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>> [storage=automatic] = function_decay<ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>>(%7);
// DEFAULT-NEXT:         call<ptr<i32>, signature=fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>(read<ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>>(%13), null<ptr<i8>>, null<ptr<f32>>, null<ptr<f64>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @stparam(%17 a: @type1, %18 b: ptr<@type1>) -> void [linkage=external] [abi=win64(coerce<i64>, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @stf() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 St1: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>>(field0(%20), function_decay<ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>>(%7));
// DEFAULT-NEXT:         call<ptr<i32>, signature=fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>(read<ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>>(field0(%20)), null<ptr<i8>>, null<ptr<f32>>, null<ptr<f64>>);
// DEFAULT-NEXT:         let %21 St2: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>>(field0(field0(%21)), function_decay<ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>>(%7));
// DEFAULT-NEXT:         call<ptr<i32>, signature=fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>(read<ptr<fn(ptr<i8>, ptr<f32>, ptr<f64>) -> ptr<i32>>>(field0(field0(%21))), null<ptr<i8>>, null<ptr<f32>>, null<ptr<f64>>);
// DEFAULT-NEXT:         let %22 fp_stparam: ptr<fn(@type1, ptr<@type1>) -> void> [storage=automatic] = function_decay<ptr<fn(@type1, ptr<@type1>) -> void>>(%16);
// DEFAULT-NEXT:         call<void, signature=fn(@type1, ptr<@type1>) -> void, abi=win64(coerce<i64>, scalar) -> void>(read<ptr<fn(@type1, ptr<@type1>) -> void>>(%22), copy<@type1, reason=arg>(read<@type1>(%21)), addr_of<ptr<@type1>>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
