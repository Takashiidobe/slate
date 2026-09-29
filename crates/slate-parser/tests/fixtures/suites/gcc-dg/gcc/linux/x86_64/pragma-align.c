/* Prove that pragma alignment handling works somewhat. */

/* { dg-do run { target { ! default_packed } } } */

extern void abort (void);

struct {
        char one;
        long two;
} defaultalign;

#if defined(__LP64__)
#pragma pack(8)
#else
#pragma pack(4)
#endif
struct {
        char one;
        long two;
} sixteen;

#pragma pack(1)
struct {
        char one;
        long two;
} two;

#pragma pack(2)
struct {
        char one;
        long two;
} three;

#pragma pack()
struct {
        char one;
        long two;
} resetalign;

main()
{
        if(sizeof(sixteen) < sizeof(defaultalign)) abort();
        if(sizeof(two) >= sizeof(defaultalign)) abort();
        if(sizeof(three) <= sizeof(two)) abort();
        if(sizeof(resetalign) != sizeof(defaultalign)) abort();
	return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:         field0 one: i8;
// DEFAULT-NEXT:         field1 two: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 one: i8;
// DEFAULT-NEXT:         field1 two: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 one: i8;
// DEFAULT-NEXT:         field1 two: i64;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 one: i8;
// DEFAULT-NEXT:         field1 two: i64;
// DEFAULT-NEXT:     } [size=10, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE4:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 one: i8;
// DEFAULT-NEXT:         field1 two: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_defaultalign:[0-9]+]] defaultalign: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sixteen:[0-9]+]] sixteen: @type[[TYPE1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_two:[0-9]+]] two: @type[[TYPE2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_three:[0-9]+]] three: @type[[TYPE3]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_resetalign:[0-9]+]] resetalign: @type[[TYPE4]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ge<u64>(const<u64>(9), const<u64>(16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if le<u64>(const<u64>(10), const<u64>(9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
