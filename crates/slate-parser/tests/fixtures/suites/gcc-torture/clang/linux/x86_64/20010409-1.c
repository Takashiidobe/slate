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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_A_2:[0-9]+]] A = @type[[TYPE_A]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 a: ptr<ptr<@type[[TYPE_A]]>>;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_B_2:[0-9]+]] B = @type[[TYPE_B]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_A]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: array<@type[[TYPE_B]], 1> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE_s:[0-9]+]] s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_A]]>, %[[VALUE_y:[0-9]+]] y: ptr<const i8>, %[[VALUE_z:[0-9]+]] z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_y]]), const<i32>(4))))), mul<i32, overflow=ub>(read<i32>(%[[VALUE_z]]), const<i32>(25))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_v:[0-9]+]] v: ptr<const i8>, %[[VALUE_w:[0-9]+]] w: i32, %[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: ptr<const i8>, %[[VALUE_z_2:[0-9]+]] z: i32) -> ptr<@type[[TYPE_A]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_w]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_x_3:[0-9]+]] x: ptr<const i8>, %[[VALUE_y_3:[0-9]+]] y: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_A]]>, ptr<const i8>, i32) -> void>(%[[VALUE_foo]], read<ptr<@type[[TYPE_A]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_A]]>>, subtract=false, element=ptr<@type[[TYPE_A]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_A]]>>>(field0(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]])))), read<i32>(field1(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]]))))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]])), const<i32>(200));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_A]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_A]]>>, subtract=false, element=ptr<@type[[TYPE_A]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_A]]>>>(field0(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]])))), read<i32>(field1(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]])))))), call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<const i8>, i32, i32, ptr<const i8>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]], read<ptr<const i8>>(%[[VALUE_x_3]]), conditional<i32>(ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0)), const<i32>(0), const<i32>(65536)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE_x_3]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]])), const<i32>(201)));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<const i8>, i32, i32, ptr<const i8>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_bar]], read<ptr<const i8>>(%[[VALUE_x_3]]), conditional<i32>(ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0)), const<i32>(0), const<i32>(65536)), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE_x_3]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]])), const<i32>(201));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<@type[[TYPE_A]]> [synthetic] = read<ptr<@type[[TYPE_A]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_A]]>>, subtract=false, element=ptr<@type[[TYPE_A]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_A]]>>>(field0(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]])))), read<i32>(field1(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]])))))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE1]]))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE1]]))), read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_y_3]]), null<ptr<i32>>)
// DEFAULT-NEXT:             write<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_A]]>>, subtract=false, element=ptr<@type[[TYPE_A]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_A]]>>>(field0(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]])))), read<i32>(field1(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]]))))))))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_y_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field1(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]]))), const<i32>(0));
// DEFAULT-NEXT:         write<ptr<ptr<@type[[TYPE_A]]>>>(field0(deref(array_decay<ptr<@type[[TYPE_B]]>, length=Some(1)>(%[[VALUE_d]]))), addr_of<ptr<ptr<@type[[TYPE_A]]>>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i32>) -> void>(%[[VALUE_test]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]])), null<ptr<i32>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
