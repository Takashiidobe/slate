void abort(void);
void exit(int);

typedef __SIZE_TYPE__ size_t;
extern size_t         strlen(const char *s);

typedef struct A {
  int a, b;
} A;

typedef struct B {
  struct A **a;
  int        b;
} B;

A  *a;
int b = 1, c;
B   d[1];

void foo(A *x, const char *y, int z) { c = y[4] + z * 25; }

A *bar(const char *v, int w, int x, const char *y, int z) {
  if (w)
    abort();
  exit(0);
}

void test(const char *x, int *y) {
  foo(d->a[d->b], "test", 200);
  d->a[d->b] = bar(x, b ? 0 : 65536, strlen(x), "test", 201);
  d->a[d->b]->a++;
  if (y)
    d->a[d->b]->b = *y;
}

int main() {
  d->b = 0;
  d->a = &a;
  test("", 0);
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
// DEFAULT-NEXT:     type @type1 A = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 A = @type1;
// DEFAULT-NEXT:     type @type3 B = struct {
// DEFAULT-NEXT:         field0 a: ptr<ptr<@type1>>;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type4 B = @type3;
// DEFAULT-NEXT:     global %8 a: ptr<@type1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 b: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %10 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 d: array<@type3, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%26 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @strlen(%27 s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %12 @foo(%13 x: ptr<@type1>, %14 y: ptr<const i8>, %15 z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%10, add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%14), const<i32>(4))))), mul<i32, overflow=ub>(read<i32>(%15), const<i32>(25))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @bar(%17 v: ptr<const i8>, %18 w: i32, %19 x: i32, %20 y: ptr<const i8>, %21 z: i32) -> ptr<@type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%18), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test(%23 x: ptr<const i8>, %24 y: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, ptr<const i8>, i32) -> void>(%12, read<ptr<@type1>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(read<ptr<ptr<@type1>>>(field0(deref(array_decay<ptr<@type3>, length=Some(1)>(%11)))), read<i32>(field1(deref(array_decay<ptr<@type3>, length=Some(1)>(%11))))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%28)), const<i32>(200));
// DEFAULT-NEXT:         write<ptr<@type1>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(read<ptr<ptr<@type1>>>(field0(deref(array_decay<ptr<@type3>, length=Some(1)>(%11)))), read<i32>(field1(deref(array_decay<ptr<@type3>, length=Some(1)>(%11)))))), call<ptr<@type1>, signature=fn(ptr<const i8>, i32, i32, ptr<const i8>, i32) -> ptr<@type1>>(%16, read<ptr<const i8>>(%23), conditional<i32>(ne<i32>(read<i32>(%9), const<i32>(0)), const<i32>(0), const<i32>(65536)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%3, read<ptr<const i8>>(%23)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%29)), const<i32>(201)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(ptr<const i8>, i32, i32, ptr<const i8>, i32) -> ptr<@type1>>(%16, read<ptr<const i8>>(%23), conditional<i32>(ne<i32>(read<i32>(%9), const<i32>(0)), const<i32>(0), const<i32>(65536)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%3, read<ptr<const i8>>(%23)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%29)), const<i32>(201));
// DEFAULT-NEXT:         let %31: ptr<@type1> [synthetic] = read<ptr<@type1>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(read<ptr<ptr<@type1>>>(field0(deref(array_decay<ptr<@type3>, length=Some(1)>(%11)))), read<i32>(field1(deref(array_decay<ptr<@type3>, length=Some(1)>(%11)))))));
// DEFAULT-NEXT:         let %32: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type1>>(%31))));
// DEFAULT-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type1>>(%31))), read<i32>(%33));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%24), null<ptr<i32>>)
// DEFAULT-NEXT:             write<i32>(field1(deref(read<ptr<@type1>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(read<ptr<ptr<@type1>>>(field0(deref(array_decay<ptr<@type3>, length=Some(1)>(%11)))), read<i32>(field1(deref(array_decay<ptr<@type3>, length=Some(1)>(%11))))))))), read<i32>(deref(read<ptr<i32>>(%24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field1(deref(array_decay<ptr<@type3>, length=Some(1)>(%11))), const<i32>(0));
// DEFAULT-NEXT:         write<ptr<ptr<@type1>>>(field0(deref(array_decay<ptr<@type3>, length=Some(1)>(%11))), addr_of<ptr<ptr<@type1>>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%22, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%30)), null<ptr<i32>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
