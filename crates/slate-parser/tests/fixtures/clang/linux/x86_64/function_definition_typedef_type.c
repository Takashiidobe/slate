typedef int F(int);
static F f;
F *p;

static int f(int x) {
  p = f;
  return x;
}

int after(void) {
  p = f;
  return 0;
}

typedef void Processor(int *);
struct parser { Processor *processor; };
static Processor epilog;

static void epilog(int *next) {
  struct parser self;
  self.processor = epilog;
  *next = 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_F:[0-9]+]] F = fn(i32) -> i32;
// DEFAULT-NEXT:     type @type[[TYPE_Processor:[0-9]+]] Processor = fn(ptr<i32>) -> void;
// DEFAULT-NEXT:     type @type[[TYPE_parser:[0-9]+]] parser = struct {
// DEFAULT-NEXT:         field0 processor: ptr<fn(ptr<i32>) -> void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<fn(i32) -> i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<fn(i32) -> i32>>(%[[VALUE_p]], function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_after:[0-9]+]] @after() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<fn(i32) -> i32>>(%[[VALUE_p]], function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_epilog:[0-9]+]] @epilog(%[[VALUE_next:[0-9]+]] next: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_self:[0-9]+]] self: @type[[TYPE_parser]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(ptr<i32>) -> void>>(field0(%[[VALUE_self]]), function_decay<ptr<fn(ptr<i32>) -> void>>(%[[VALUE_epilog]]));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_next]])), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
