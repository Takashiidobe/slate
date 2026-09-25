/* { dg-require-effective-target int32plus } */
#pragma pack(1)
struct S {
  int f0 : 15;
  int f1 : 29;
};

int             e = 1, i;
static struct S d[6];

int main(void) {
  if (e) {
    d[i].f0 = 1;
    d[i].f1 = 1;
  }
  if (d[0].f1 != 1)
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 f0: i32 : 15;
// DEFAULT-NEXT:         field1 f1: i32 : 29;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0, 1], bit_offsets=[Some(0), Some(15)], bit_units=[(0, 6)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %1 e: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %2 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: array<@type0, 6> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(bitfield0<unit=0, bytes=0..6, bits=0..15>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%3), read<i32>(%2)))), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(bitfield1<unit=0, bytes=0..6, bits=15..44>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%3), read<i32>(%2)))), const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(bitfield1<unit=0, bytes=0..6, bits=15..44>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%3), const<i32>(0))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
