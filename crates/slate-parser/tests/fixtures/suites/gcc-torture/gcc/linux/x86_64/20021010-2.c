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
// DEFAULT-NEXT:     type @type[[TYPE_SInt16:[0-9]+]] SInt16 = i16;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 minx: i16;
// DEFAULT-NEXT:         field1 maxx: i16;
// DEFAULT-NEXT:         field2 miny: i16;
// DEFAULT-NEXT:         field3 maxy: i16;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0, 2, 4, 6]];
// DEFAULT-NEXT:     type @type[[TYPE_IOGBounds:[0-9]+]] IOGBounds = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_expectedwidth:[0-9]+]] expectedwidth: i32 [storage=static] = const<i32>(50) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_global_vramPtr:[0-9]+]] global_vramPtr: ptr<u32> [storage=static] = int_to_ptr<ptr<u32>, reason=explicit>(const<i32>(40960)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_global_bounds:[0-9]+]] global_bounds: @type[[TYPE0]] [storage=static] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(100)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(150)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(100)), field3 = truncate<i16, reason=assign, fits=always>(const<i32>(150))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_global_saveRect:[0-9]+]] global_saveRect: @type[[TYPE0]] [storage=static] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(75)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(175)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(75)), field3 = truncate<i16, reason=assign, fits=always>(const<i32>(175))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_vramPtr:[0-9]+]] vramPtr: ptr<u32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_width:[0-9]+]] width: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_saveRect:[0-9]+]] saveRect: @type[[TYPE0]] [storage=automatic] = copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(%[[VALUE_global_saveRect]]));
// DEFAULT-NEXT:         let %[[VALUE_bounds:[0-9]+]] bounds: @type[[TYPE0]] [storage=automatic] = copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(%[[VALUE_global_bounds]]));
// DEFAULT-NEXT:         if lt<i32>(widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_saveRect]]))), widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_bounds]]))))
// DEFAULT-NEXT:             write<i16>(field0(%[[VALUE_saveRect]]), read<i16>(field0(%[[VALUE_bounds]])));
// DEFAULT-NEXT:         if gt<i32>(widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_saveRect]]))), widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_bounds]]))))
// DEFAULT-NEXT:             write<i16>(field1(%[[VALUE_saveRect]]), read<i16>(field1(%[[VALUE_bounds]])));
// DEFAULT-NEXT:         write<ptr<u32>>(%[[VALUE_vramPtr]], ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_global_vramPtr]]), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_saveRect]]))), widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_bounds]]))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_width]], sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_saveRect]]))), widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_saveRect]])))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_width]]), read<i32>(%[[VALUE_expectedwidth]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
