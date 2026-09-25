#include <stdio.h>
#include <stdlib.h>
extern void abort();

typedef struct foo {
  int   uaattrid;
  char *name;
} FOO;

FOO Upgrade_items[] = {{1, "1"}, {2, "2"}, {0, NULL}};

int *Upgd_minor_ID = (int *)&((Upgrade_items + 1)->uaattrid);

int *Upgd_minor_ID1 = (int *)&((Upgrade_items)->uaattrid);

int main(int argc, char **argv) {
  if (*Upgd_minor_ID != 2)
    abort();

  if (*Upgd_minor_ID1 != 1)
    abort();
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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 uaattrid: i32;
// DEFAULT-NEXT:         field1 name: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 FOO = @type0;
// DEFAULT-NEXT:     global %9 .str9: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %3 Upgrade_items: array<@type0, 3> [storage=static] = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = array_decay<ptr<i8>, length=Some(2)>(%9)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(2), field1 = array_decay<ptr<i8>, length=Some(2)>(%10)), index2 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = null<ptr<i8>>)) [linkage=external];
// DEFAULT-NEXT:     global %4 Upgd_minor_ID: ptr<i32> [storage=static] = addr_of<ptr<i32>>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%3), const<i32>(1))))) [linkage=external];
// DEFAULT-NEXT:     global %5 Upgd_minor_ID1: ptr<i32> [storage=static] = addr_of<ptr<i32>>(field0(deref(array_decay<ptr<@type0>, length=Some(3)>(%3)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main(%7 argc: i32, %8 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(%4))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(%5))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
