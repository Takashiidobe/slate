/* { dg-require-effective-target label_values } */
int code[] = {0, 0, 0, 0, 1};

void foo(int x) {
  volatile int b;
  b = 0xffffffff;
}

void bar(int *pc) {
  static const void *l[] = {&&lab0, &&end};

  foo(0);
  goto *l[*pc];
lab0:
  foo(0);
  pc++;
  goto *l[*pc];
end:
  return;
}

int main() {
  bar(code);
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
// DEFAULT-NEXT:     global %[[VALUE_code:[0-9]+]] code: array<i32, 5> [storage=static] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0), index2 = const<i32>(0), index3 = const<i32>(0), index4 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: array<ptr<const void>, 2> [storage=static] [align=16] = aggregate<array<ptr<const void>, 2>, zero_fill=false>(index0 = pointer_cast<ptr<const void>, reason=assign>(label_addr<ptr<void>>(%[[VALUE_lab0:[0-9]+]])), index1 = pointer_cast<ptr<const void>, reason=assign>(label_addr<ptr<void>>(%[[VALUE_end:[0-9]+]]))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: volatile i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(const<u32>(4294967295)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_pc:[0-9]+]] pc: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], const<i32>(0));
// DEFAULT-NEXT:         goto *read<ptr<const void>>(deref(ptr_offset<ptr<ptr<const void>>, subtract=false, element=ptr<const void>, overflow=ub>(array_decay<ptr<ptr<const void>>, length=Some(2)>(%[[VALUE_l]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_pc]]))))));
// DEFAULT-NEXT:         label %[[VALUE_lab0]] lab0:
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_pc]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_pc]], read<ptr<i32>>(%[[VALUE1]]));
// DEFAULT-NEXT:         goto *read<ptr<const void>>(deref(ptr_offset<ptr<ptr<const void>>, subtract=false, element=ptr<const void>, overflow=ub>(array_decay<ptr<ptr<const void>>, length=Some(2)>(%[[VALUE_l]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_pc]]))))));
// DEFAULT-NEXT:         label %[[VALUE_end]] end:
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_bar]], array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_code]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
