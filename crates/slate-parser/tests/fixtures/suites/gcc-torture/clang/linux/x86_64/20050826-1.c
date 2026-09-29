/* PR rtl-optimization/23561 */

struct A {
  char a1[1];
  char a2[5];
  char a3[1];
  char a4[2048 - 7];
} a;

typedef __SIZE_TYPE__ size_t;
extern void          *memset(void *, int, size_t);
extern void          *memcpy(void *, const void *, size_t);
extern int            memcmp(const void *, const void *, size_t);
extern void           abort(void);

void bar(struct A *x) {
  size_t i;
  if (memcmp(x, "\1HELLO\1", sizeof "\1HELLO\1"))
    abort();
  for (i = 0; i < sizeof(x->a4); i++)
    if (x->a4[i])
      abort();
}

int foo(void) {
  memset(&a, 0, sizeof(a));
  a.a1[0] = 1;
  memcpy(a.a2, "HELLO", sizeof "HELLO");
  a.a3[0] = 1;
  bar(&a);
  return 0;
}

int main(void) {
  foo();
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a1: array<i8, 1>;
// DEFAULT-NEXT:         field1 a2: array<i8, 5>;
// DEFAULT-NEXT:         field2 a3: array<i8, 1>;
// DEFAULT-NEXT:         field3 a4: array<i8, 2041>;
// DEFAULT-NEXT:     } [size=2048, align=1, offsets=[0, 1, 6, 7]];
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: @type[[TYPE_A]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([1, 72, 69, 76, 76, 79, 1, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([72, 69, 76, 76, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE6:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE7:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE8:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_A]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u64 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str]])), const<u64>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%[[VALUE_i]]), const<u64>(2041))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE10]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2041)>(field3(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]])))), read<u64>(%[[VALUE_i]])))), const<i8>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]])), const<i32>(0), const<u64>(2048));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(%[[VALUE_a]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(field1(%[[VALUE_a]]))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_2]])), const<u64>(6));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field2(%[[VALUE_a]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
