#include <stdio.h>

typedef struct {
  int *start;
  int *end;
  int *pointer;
} buffer_t;

int main(void) {
  struct {
    int x;
    int y;
  } point = {3, 4};

  int      storage[4];
  buffer_t buf = {0, 0, 0};
  buf.start    = storage;
  buf.pointer  = storage;
  buf.end      = storage + 4;

  *buf.pointer = point.x + point.y;
  buf.pointer++;
  *buf.pointer = point.x * point.y;
  buf.pointer++;

  printf("%d %d\n", storage[0], storage[1]);
  printf("%ld\n", (long)(buf.pointer - buf.start));
  printf("%ld\n", (long)(buf.end - buf.start));
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 start: ptr<i32>;
// DEFAULT-NEXT:         field1 end: ptr<i32>;
// DEFAULT-NEXT:         field2 pointer: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type1 buffer_t = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 point: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4));
// DEFAULT-NEXT:         let %6 storage: array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %7 buf: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = null<ptr<i32>>, field1 = null<ptr<i32>>, field2 = null<ptr<i32>>);
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%7), array_decay<ptr<i32>, length=Some(4)>(%6));
// DEFAULT-NEXT:         write<ptr<i32>>(field2(%7), array_decay<ptr<i32>, length=Some(4)>(%6));
// DEFAULT-NEXT:         write<ptr<i32>>(field1(%7), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%6), const<i32>(4)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field2(%7))), add<i32, overflow=ub>(read<i32>(field0(%5)), read<i32>(field1(%5))));
// DEFAULT-NEXT:         let %12: ptr<i32> [synthetic] = read<ptr<i32>>(field2(%7));
// DEFAULT-NEXT:         let %13: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%12), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(field2(%7), read<ptr<i32>>(%13));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field2(%7))), mul<i32, overflow=ub>(read<i32>(field0(%5)), read<i32>(field1(%5))));
// DEFAULT-NEXT:         let %14: ptr<i32> [synthetic] = read<ptr<i32>>(field2(%7));
// DEFAULT-NEXT:         let %15: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(field2(%7), read<ptr<i32>>(%15));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%9)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%6), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%6), const<i32>(1)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%10)), ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<i32>>(field2(%7)), read<ptr<i32>>(field0(%7))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%11)), ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<i32>>(field1(%7)), read<ptr<i32>>(field0(%7))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
