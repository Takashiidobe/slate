// { dg-additional-options "-O2" }
#include <stdio.h>

typedef struct {
  int x;
} FIO_Dict_t;

typedef struct {
  int y;
} FIO_SyncCompressIO;

typedef struct {
  FIO_Dict_t         dict;
  int                cctx;
  FIO_SyncCompressIO io;
} cRess_t;

static void freeDict(FIO_Dict_t *dict) { dict->x = 0; }

static void syncDestroy(FIO_SyncCompressIO *io) { io->y = 0; }

static void freeCResources(cRess_t *const ress) {
  freeDict(&(ress->dict));
  syncDestroy(&ress->io);
}

int main(void) {
  cRess_t r = {.dict = {.x = 5}, .cctx = 9, .io = {.y = 7}};
  freeCResources(&r);
  printf("%d %d %d\n", r.dict.x, r.io.y, r.cctx);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_FIO_Dict_t:[0-9]+]] FIO_Dict_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_FIO_SyncCompressIO:[0-9]+]] FIO_SyncCompressIO = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 dict: @type[[TYPE0]];
// DEFAULT-NEXT:         field1 cctx: i32;
// DEFAULT-NEXT:         field2 io: @type[[TYPE1]];
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_cRess_t:[0-9]+]] cRess_t = @type[[TYPE2]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_freeDict:[0-9]+]] @freeDict(%[[VALUE_dict:[0-9]+]] dict: ptr<@type[[TYPE0]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_dict]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_syncDestroy:[0-9]+]] @syncDestroy(%[[VALUE_io:[0-9]+]] io: ptr<@type[[TYPE1]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_io]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_freeCResources:[0-9]+]] @freeCResources(%[[VALUE_ress:[0-9]+]] ress: ptr<@type[[TYPE2]]> [const]) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE0]]>) -> void>(%[[VALUE_freeDict]], addr_of<ptr<@type[[TYPE0]]>>(field0(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_ress]])))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE1]]>) -> void>(%[[VALUE_syncDestroy]], addr_of<ptr<@type[[TYPE1]]>>(field2(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_ress]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE2]] [storage=automatic] = aggregate<@type[[TYPE2]], zero_fill=false>(field0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(5)), field1 = const<i32>(9), field2 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(7)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE2]]>) -> void>(%[[VALUE_freeCResources]], addr_of<ptr<@type[[TYPE2]]>>(%[[VALUE_r]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(field0(field0(%[[VALUE_r]]))), read<i32>(field0(field2(%[[VALUE_r]]))), read<i32>(field1(%[[VALUE_r]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
