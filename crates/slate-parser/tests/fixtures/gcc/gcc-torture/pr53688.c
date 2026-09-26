char headline[256];
struct hdr {
  char part1[9];
  char part2[8];
} p;

void __attribute__((noinline, noclone)) init() {
  __builtin_memcpy(p.part1, "FOOBARFOO", sizeof(p.part1));
  __builtin_memcpy(p.part2, "SPEC CPU", sizeof(p.part2));
}

int main() {
  char *x;
  int   c;
  init();
  __builtin_memcpy(&headline[0], p.part1, 9);
  c = 9;
  x = &headline[0];
  x = x + c;
  __builtin_memset(x, ' ', 245);
  __builtin_memcpy(&headline[10], p.part2, 8);
  c = 18;
  x = &headline[0];
  x = x + c;
  __builtin_memset(x, ' ', 238);
  if (headline[10] != 'S')
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type0 hdr = struct {
// DEFAULT-NEXT:         field0 part1: array<i8, 9>;
// DEFAULT-NEXT:         field1 part2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=17, align=1, offsets=[0, 9]];
// DEFAULT-NEXT:     global %0 headline: array<i8, 256> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %2 p: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([70, 79, 79, 66, 65, 82, 70, 79, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([83, 80, 69, 67, 32, 67, 80, 85, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @init() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field0(%2))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%7)), const<u64>(9));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field1(%2))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%8)), const<u64>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 x: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %6 c: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%0), const<i32>(0))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field0(%2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))));
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(9));
// DEFAULT-NEXT:         write<ptr<i8>>(%5, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%0), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<i8>>(%5, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%5), read<i32>(%6)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%5)), const<i32>(32), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(245))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%0), const<i32>(10))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field1(%2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(18));
// DEFAULT-NEXT:         write<ptr<i8>>(%5, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%0), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<i8>>(%5, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%5), read<i32>(%6)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%5)), const<i32>(32), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(238))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%0), const<i32>(10))))), const<i32>(83))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
