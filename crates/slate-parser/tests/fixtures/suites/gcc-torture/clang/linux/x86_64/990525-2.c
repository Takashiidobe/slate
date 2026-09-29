void abort(void);
void exit(int);

typedef struct {
  int v[4];
} Test1;

Test1 func2();

int func1() {
  Test1 test;
  test = func2();

  if (test.v[0] != 10)
    abort();
  if (test.v[1] != 20)
    abort();
  if (test.v[2] != 30)
    abort();
  if (test.v[3] != 40)
    abort();
}

Test1 func2() {
  Test1 tmp;
  tmp.v[0] = 10;
  tmp.v[1] = 20;
  tmp.v[2] = 30;
  tmp.v[3] = 40;
  return tmp;
}

int main() {
  func1();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 v: array<i32, 4>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Test1:[0-9]+]] Test1 = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_func2:[0-9]+]] @func2() -> @type[[TYPE0]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field0(%[[VALUE_tmp]])), const<i32>(0))), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field0(%[[VALUE_tmp]])), const<i32>(1))), const<i32>(20));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field0(%[[VALUE_tmp]])), const<i32>(2))), const<i32>(30));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field0(%[[VALUE_tmp]])), const<i32>(3))), const<i32>(40));
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_tmp]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func1:[0-9]+]] @func1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_test:[0-9]+]] test: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_test]], copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn() -> @type[[TYPE0]], abi=sysv64() -> native_c>(%[[VALUE_func2]])));
// DEFAULT-NEXT:         copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn() -> @type[[TYPE0]], abi=sysv64() -> native_c>(%[[VALUE_func2]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field0(%[[VALUE_test]])), const<i32>(0)))), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field0(%[[VALUE_test]])), const<i32>(1)))), const<i32>(20))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field0(%[[VALUE_test]])), const<i32>(2)))), const<i32>(30))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field0(%[[VALUE_test]])), const<i32>(3)))), const<i32>(40))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_func1]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
