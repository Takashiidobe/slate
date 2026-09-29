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
// DEFAULT-NEXT:     type @type[[TYPE_hdr:[0-9]+]] hdr = struct {
// DEFAULT-NEXT:         field0 part1: array<i8, 9>;
// DEFAULT-NEXT:         field1 part2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=17, align=1, offsets=[0, 9]];
// DEFAULT-NEXT:     global %[[VALUE_headline:[0-9]+]] headline: array<i8, 256> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: @type[[TYPE_hdr]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([70, 79, 79, 66, 65, 82, 70, 79, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([83, 80, 69, 67, 32, 67, 80, 85, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_init:[0-9]+]] @init() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field0(%[[VALUE_p]]))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), const<u64>(9));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field1(%[[VALUE_p]]))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_2]])), const<u64>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_init]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_headline]]), const<i32>(0))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field0(%[[VALUE_p]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], const<i32>(9));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x]], addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_headline]]), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x]]), read<i32>(%[[VALUE_c]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_x]])), const<i32>(32), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(245))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_headline]]), const<i32>(10))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field1(%[[VALUE_p]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], const<i32>(18));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x]], addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_headline]]), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_x]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x]]), read<i32>(%[[VALUE_c]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_x]])), const<i32>(32), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(238))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_headline]]), const<i32>(10))))), const<i32>(83))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
