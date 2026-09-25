void abort(void);
void exit(int);

struct s {
  char text[11];
  int  flag;
} cell;

int check(struct s p) {
  if (p.flag != 99)
    return 1;
  return __builtin_strcmp(p.text, "0123456789");
}

int main(void) {
  cell.flag = 99;
  __builtin_strcpy(cell.text, "0123456789");

  if (check(cell))
    abort();
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 text: array<i8, 11>;
// DEFAULT-NEXT:         field1 flag: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 12]];
// DEFAULT-NEXT:     global %3 cell: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @check(%5 p: @type0) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field1(%5)), const<i32>(99))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(__builtin_strcmp, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(field0(%5))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field1(%3), const<i32>(99));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(__builtin_strcpy, array_decay<ptr<i8>, length=Some(11)>(field0(%3)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%9)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(@type0) -> i32, abi=sysv64(native_c) -> scalar>(%4, copy<@type0, reason=arg>(read<@type0>(%3))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
