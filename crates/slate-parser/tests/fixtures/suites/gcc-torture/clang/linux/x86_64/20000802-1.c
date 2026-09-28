// SLATE-FILECHECK-DEFINES DEFAULT

struct foo {
  char a[3];
  char b;
  char c;
};

struct foo bs;
int x;
char y[3];

void bar(void)
{
    __builtin_memcpy(bs.a, y, 3);
    bs.a[1] = ((x ? &bs.b : &bs.c) - (char *)&bs) - 2;
}

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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 3>;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 3, 4]];
// DEFAULT-NEXT:     global %1 bs: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 y: array<i8, 3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @__builtin_memcpy(%5 <unnamed>: ptr<void>, %6 <unnamed>: ptr<const void>, %7 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%8, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(field0(%1))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%3)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(field0(%1)), const<i32>(1))), truncate<i8, reason=assign, fits=unknown>(sub<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(conditional<ptr<i8>>(ne<i32>(read<i32>(%2), const<i32>(0)), addr_of<ptr<i8>>(field1(%1)), addr_of<ptr<i8>>(field2(%1))), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%1))), widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
