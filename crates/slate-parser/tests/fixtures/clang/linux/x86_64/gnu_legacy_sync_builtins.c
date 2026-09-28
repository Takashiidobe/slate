#include <stdio.h>

int main(void) {
  int v;

  v = 5;
  printf("%d\n", __sync_fetch_and_add(&v, 3));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_fetch_and_sub(&v, 3));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_fetch_and_or(&v, 0xF0));
  printf("%d\n", v);

  v = 0xFF;
  printf("%d\n", __sync_fetch_and_and(&v, 0x0F));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_fetch_and_xor(&v, 0xFF));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_fetch_and_nand(&v, 3));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_sub_and_fetch(&v, 3));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_or_and_fetch(&v, 0xF0));
  printf("%d\n", v);

  v = 0xFF;
  printf("%d\n", __sync_and_and_fetch(&v, 0x0F));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_xor_and_fetch(&v, 0xFF));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_nand_and_fetch(&v, 3));
  printf("%d\n", v);

  __sync_synchronize();
  printf("ok\n");
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
// DEFAULT-NEXT:     global %5 .str5: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 107, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%4 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 v: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(5));
// DEFAULT-NEXT:         let %28: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), add<i32, overflow=wrap>(old<i32>, const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%5)), read<i32>(%28));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%6)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(5));
// DEFAULT-NEXT:         let %29: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), sub<i32, overflow=wrap>(old<i32>, const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%7)), read<i32>(%29));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(15));
// DEFAULT-NEXT:         let %30: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), or<i32>(old<i32>, const<i32>(240)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), read<i32>(%30));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(255));
// DEFAULT-NEXT:         let %31: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), and<i32>(old<i32>, const<i32>(15)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), read<i32>(%31));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%12)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(15));
// DEFAULT-NEXT:         let %32: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), xor<i32>(old<i32>, const<i32>(255)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13)), read<i32>(%32));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%14)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(5));
// DEFAULT-NEXT:         let %33: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), not<i32>(and<i32>(old<i32>, const<i32>(3))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%15)), read<i32>(%33));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%16)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(5));
// DEFAULT-NEXT:         let %34: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), sub<i32, overflow=wrap>(old<i32>, const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), read<i32>(%34));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(15));
// DEFAULT-NEXT:         let %35: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), or<i32>(old<i32>, const<i32>(240)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%19)), read<i32>(%35));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(255));
// DEFAULT-NEXT:         let %36: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), and<i32>(old<i32>, const<i32>(15)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%21)), read<i32>(%36));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%22)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(15));
// DEFAULT-NEXT:         let %37: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), xor<i32>(old<i32>, const<i32>(255)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%23)), read<i32>(%37));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%24)), read<i32>(%3));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(5));
// DEFAULT-NEXT:         let %38: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), not<i32>(and<i32>(old<i32>, const<i32>(3))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%25)), read<i32>(%38));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%26)), read<i32>(%3));
// DEFAULT-NEXT:         fence<scope=thread, order=seq_cst>;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%27)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
