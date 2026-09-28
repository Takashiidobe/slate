typedef int lowered __attribute__((aligned(1)));
typedef char aligned_buffer[32] __attribute__((aligned(4)));
struct twenty { char c[20]; };

long long sixteen[2];
char fifteen[15];
lowered lowered_elements[4];
long long requested[2] __attribute__((aligned(4)));
_Alignas(4) char alignas_requested[32];
aligned_buffer typedef_buffer;
struct twenty record;
struct twenty records[1];
char completed[] = "0123456789abcdefXYZ";
extern char declared[32];
extern char incomplete[];

void sink(void *);

void locals(int n) {
  long long local_sixteen[2];
  char local_fifteen[15];
  static char local_static[32];
  char vla[n];
  sink(local_sixteen);
  sink(local_fifteen);
  sink(local_static);
  sink(vla);
  sink(declared);
  sink(incomplete);
  sink("0123456789abcdefghij");
}

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES IR

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 lowered = i32;
// IR-NEXT:     type @type1 aligned_buffer = array<i8, 32>;
// IR-NEXT:     type @type2 twenty = struct {
// IR-NEXT:         field0 c: array<i8, 20>;
// IR-NEXT:     } [size=20, align=1, offsets=[0]];
// IR-NEXT:     global %3 sixteen: array<i64, 2> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %4 fifteen: array<i8, 15> [storage=static] [linkage=external];
// IR-NEXT:     global %5 lowered_elements: array<i32, 4> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %6 requested: array<i64, 2> [storage=static] [align=4] [linkage=external];
// IR-NEXT:     global %7 alignas_requested: array<i8, 32> [storage=static] [align=4] [linkage=external];
// IR-NEXT:     global %8 typedef_buffer: array<i8, 32> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %9 record: @type2 [storage=static] [linkage=external];
// IR-NEXT:     global %10 records: array<@type2, 1> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %11 completed: array<i8, 20> [storage=static] [align=16] = code_units<array<i8, 20>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 88, 89, 90, 0]) [linkage=external];
// IR-NEXT:     extern %12 declared: array<i8, 32> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     extern %13 incomplete: array<i8, incomplete> [storage=static] [linkage=external];
// IR-NEXT:     global %19 local_static: array<i8, 32> [storage=static] [align=16] [linkage=internal];
// IR-NEXT:     global %23 .str23: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 0]) [linkage=internal];
// IR-NEXT:     fn %14 @sink(%21 <unnamed>: ptr<void>) -> void [linkage=external];
// IR-NEXT:     fn %15 @locals(%16 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %17 local_sixteen: array<i64, 2> [storage=automatic] [align=16];
// IR-NEXT:         let %18 local_fifteen: array<i8, 15> [storage=automatic];
// IR-NEXT:         let %22: u64 [synthetic] = reinterpret<u64>(widen<i64>(read<i32>(%16)));
// IR-NEXT:         let %20 vla: vla<i8, %22> [storage=automatic];
// IR-NEXT:         call<void>(%14, pointer_cast<ptr<void>>(array_decay<ptr<i64>, length=Some(2)>(%17)));
// IR-NEXT:         call<void>(%14, pointer_cast<ptr<void>>(array_decay<ptr<i8>, length=Some(15)>(%18)));
// IR-NEXT:         call<void>(%14, pointer_cast<ptr<void>>(array_decay<ptr<i8>, length=Some(32)>(%19)));
// IR-NEXT:         call<void>(%14, pointer_cast<ptr<void>>(array_decay<ptr<i8>, length=None>(%20)));
// IR-NEXT:         call<void>(%14, pointer_cast<ptr<void>>(array_decay<ptr<i8>, length=Some(32)>(%12)));
// IR-NEXT:         call<void>(%14, pointer_cast<ptr<void>>(array_decay<ptr<i8>, length=None>(%13)));
// IR-NEXT:         call<void>(%14, pointer_cast<ptr<void>>(array_decay<ptr<i8>, length=Some(21)>(%23)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
