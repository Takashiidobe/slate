/* cse.c failure on x86 target.
   Contributed by Stuart Hastings 10 Oct 2002 <stuart@apple.com> */
#include <stdlib.h>

typedef signed short SInt16;

typedef struct {
  SInt16 minx;
  SInt16 maxx;
  SInt16 miny;
  SInt16 maxy;
} IOGBounds;

int expectedwidth = 50;

unsigned int *global_vramPtr = (unsigned int *)0xa000;

IOGBounds global_bounds   = {100, 150, 100, 150};
IOGBounds global_saveRect = {75, 175, 75, 175};

int main(void) {
  unsigned int *vramPtr;
  int           width;
  IOGBounds     saveRect = global_saveRect;
  IOGBounds     bounds   = global_bounds;

  if (saveRect.minx < bounds.minx)
    saveRect.minx = bounds.minx;
  if (saveRect.maxx > bounds.maxx)
    saveRect.maxx = bounds.maxx;

  vramPtr = global_vramPtr + (saveRect.miny - bounds.miny);
  width   = saveRect.maxx - saveRect.minx;

  if (width != expectedwidth)
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
// DEFAULT-NEXT:     type @type0 SInt16 = i16;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 minx: i16;
// DEFAULT-NEXT:         field1 maxx: i16;
// DEFAULT-NEXT:         field2 miny: i16;
// DEFAULT-NEXT:         field3 maxy: i16;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0, 2, 4, 6]];
// DEFAULT-NEXT:     type @type2 IOGBounds = @type1;
// DEFAULT-NEXT:     global %5 expectedwidth: i32 [storage=static] = const<i32>(50) [linkage=external];
// DEFAULT-NEXT:     global %6 global_vramPtr: ptr<u32> [storage=static] = int_to_ptr<ptr<u32>, reason=explicit>(const<i32>(40960)) [linkage=external];
// DEFAULT-NEXT:     global %7 global_bounds: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(100)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(150)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(100)), field3 = truncate<i16, reason=assign, fits=always>(const<i32>(150))) [linkage=external];
// DEFAULT-NEXT:     global %8 global_saveRect: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(75)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(175)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(75)), field3 = truncate<i16, reason=assign, fits=always>(const<i32>(175))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%14 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 vramPtr: ptr<u32> [storage=automatic];
// DEFAULT-NEXT:         let %11 width: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 saveRect: @type1 [storage=automatic] = copy<@type1, reason=assign>(read<@type1>(%8));
// DEFAULT-NEXT:         let %13 bounds: @type1 [storage=automatic] = copy<@type1, reason=assign>(read<@type1>(%7));
// DEFAULT-NEXT:         if lt<i32>(widen<i32, reason=promotion>(read<i16>(field0(%12))), widen<i32, reason=promotion>(read<i16>(field0(%13))))
// DEFAULT-NEXT:             write<i16>(field0(%12), read<i16>(field0(%13)));
// DEFAULT-NEXT:         if gt<i32>(widen<i32, reason=promotion>(read<i16>(field1(%12))), widen<i32, reason=promotion>(read<i16>(field1(%13))))
// DEFAULT-NEXT:             write<i16>(field1(%12), read<i16>(field1(%13)));
// DEFAULT-NEXT:         write<ptr<u32>>(%10, ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%6), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(field2(%12))), widen<i32, reason=promotion>(read<i16>(field2(%13))))));
// DEFAULT-NEXT:         write<i32>(%11, sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(field1(%12))), widen<i32, reason=promotion>(read<i16>(field0(%12)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%11), read<i32>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
