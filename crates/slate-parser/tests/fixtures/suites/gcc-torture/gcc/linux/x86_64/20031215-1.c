/* PR middle-end/13400 */
/* The following test used to fail at run-time with a write to read-only
   memory, caused by if-conversion converting a conditional write into an
   unconditional write.  */

typedef struct {
  int  c, l;
  char ch[3];
} pstr;
const pstr        ao = {2, 2, "OK"};
const pstr *const a  = &ao;

void test1(void) {
  if (a->ch[a->l]) {
    ((char *)a->ch)[a->l] = 0;
  }
}

void test2(void) {
  if (a->ch[a->l]) {
    ((char *)a->ch)[a->l] = -1;
  }
}

void test3(void) {
  if (a->ch[a->l]) {
    ((char *)a->ch)[a->l] = 1;
  }
}

int main(void) {
  test1();
  test2();
  test3();
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:         field1 l: i32;
// DEFAULT-NEXT:         field2 ch: array<i8, 3>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_pstr:[0-9]+]] pstr = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_ao:[0-9]+]] ao: @type[[TYPE0]] [storage=static] [const] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(2), field1 = const<i32>(2), field2 = code_units<array<i8, 3>>([79, 75, 0])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<const @type[[TYPE0]]> [storage=static] [const] = addr_of<ptr<const @type[[TYPE0]]>>(%[[VALUE_ao]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i8>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(field2(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]])))), read<i32>(field1(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]]))))))), const<i8>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(array_decay<ptr<const i8>, length=Some(3)>(field2(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]]))))), read<i32>(field1(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]])))))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i8>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(field2(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]])))), read<i32>(field1(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]]))))))), const<i8>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(array_decay<ptr<const i8>, length=Some(3)>(field2(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]]))))), read<i32>(field1(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]])))))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i8>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(field2(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]])))), read<i32>(field1(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]]))))))), const<i8>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(array_decay<ptr<const i8>, length=Some(3)>(field2(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]]))))), read<i32>(field1(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_a]])))))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test3]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
