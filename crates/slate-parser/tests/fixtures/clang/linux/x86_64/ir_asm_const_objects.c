// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
static const int k = 2;
const long wide = 1L << 40;
static const int chained = k;
static const int table[4] = {1, 2, 3, 4};
static const int ranged[4] = {[0 ... 3] = 7};
static const int filled[4] = {1};
static const char text[] = "abc";
static const struct { int x : 2; unsigned y : 3; int z[3]; } s = {3, 13, {4, 5, 6}};
static const int *const pointer = &k;
static const double real = 2.5;
static const volatile int shared = 2;
static int mutable_ = 2;
extern const int declared;
int arr[8];
int call(void);

void folded(void) {
    const int local = 3;
    static const int kept = 4;
    asm("# %0 %1 %2 %3" : : "i"(k), "i"(wide), "i"(chained), "i"(k * 3 + 1));
    asm("# %0 %1 %2 %3" : : "i"(table[1]), "i"(*(table + 2)), "i"(ranged[2]), "i"(filled[3]));
    asm("# %0 %1 %2 %3" : : "i"(text[1]), "i"(s.x), "i"(s.y), "i"(s.z[2]));
    asm("# %0 %1 %2 %3" : : "i"(*pointer), "i"((int)real), "i"(local), "i"(kept));
    asm("# %0 %1 %2 %3" : : "n"(k), "g"(k), "ri"(k), "i"(&arr[k]));
}

void rejected(int parameter) {
    const int from_call = call();
    const int from_parameter = parameter;
    asm("# %0 %1 %2" : : "i"(shared), "i"(mutable_), "i"(declared));
    asm("# %0 %1" : : "i"(from_call), "i"(from_parameter));
}

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
// IR-NEXT:     type @type0 = struct {
// IR-NEXT:         field0 x: i32 : 2;
// IR-NEXT:         field1 y: u32 : 3;
// IR-NEXT:         field2 z: array<i32, 3>;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 0, 4], bit_offsets=[Some(0), Some(2), None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), None]];
// IR-NEXT:     global %0 k: i32 [storage=static] [const] = const<i32>(2) [linkage=internal];
// IR-NEXT:     global %1 wide: i64 [storage=static] [const] = shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1), const<i32>(40)) [linkage=external];
// IR-NEXT:     global %2 chained: i32 [storage=static] [const] = read<i32>(%0) [linkage=internal];
// IR-NEXT:     global %3 table: array<i32, 4> [storage=static] [const] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4)) [linkage=internal];
// IR-NEXT:     global %4 ranged: array<i32, 4> [storage=static] [const] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0..=3 = const<i32>(7)) [linkage=internal];
// IR-NEXT:     global %5 filled: array<i32, 4> [storage=static] [const] [align=16] = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(1)) [linkage=internal];
// IR-NEXT:     global %6 text: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// IR-NEXT:     global %8 s: @type0 [storage=static] [const] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(13)), field2 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5), index2 = const<i32>(6))) [linkage=internal];
// IR-NEXT:     global %9 pointer: ptr<const i32> [storage=static] [const] = addr_of<ptr<const i32>>(%0) [linkage=internal];
// IR-NEXT:     global %10 real: f64 [storage=static] [const] = const<f64>(2.5) [linkage=internal];
// IR-NEXT:     global %11 shared: volatile i32 [storage=static] [const] = const<i32>(2) [linkage=internal];
// IR-NEXT:     global %12 mutable_: i32 [storage=static] = const<i32>(2) [linkage=internal];
// IR-NEXT:     extern %13 declared: i32 [storage=static] [const] [linkage=external];
// IR-NEXT:     global %14 arr: array<i32, 8> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %18 kept: i32 [storage=static] [const] = const<i32>(4) [linkage=internal];
// IR-NEXT:     fn %15 @call() -> i32 [linkage=external];
// IR-NEXT:     fn %16 @folded() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %17 local: i32 [storage=automatic] [const] = const<i32>(3);
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "i" [imm | sym] -> imm width 32 const<i32>(2);
// IR-NEXT:             in 1 "i" [imm | sym] -> imm width 64 const<i64>(1099511627776);
// IR-NEXT:             in 2 "i" [imm | sym] -> imm width 32 const<i32>(2);
// IR-NEXT:             in 3 "i" [imm | sym] -> imm width 32 const<i32>(7);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "i" [imm | sym] -> imm width 32 const<i32>(2);
// IR-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(3);
// IR-NEXT:             in 2 "i" [imm | sym] -> imm width 32 const<i32>(7);
// IR-NEXT:             in 3 "i" [imm | sym] -> imm width 32 const<i32>(0);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "i" [imm | sym] -> imm width 8 const<i8>(98);
// IR-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(-1);
// IR-NEXT:             in 2 "i" [imm | sym] -> imm width 32 const<i32>(5);
// IR-NEXT:             in 3 "i" [imm | sym] -> imm width 32 const<i32>(6);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "i" [imm | sym] -> imm width 32 const<i32>(2);
// IR-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(2);
// IR-NEXT:             in 2 "i" [imm | sym] -> imm width 32 const<i32>(3);
// IR-NEXT:             in 3 "i" [imm | sym] -> imm width 32 const<i32>(4);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "n" [imm] width 32 const<i32>(2);
// IR-NEXT:             in 1 "g" [reg | mem | imm | sym] -> imm width 32 const<i32>(2);
// IR-NEXT:             in 2 "ri" [reg | imm | sym] -> imm width 32 const<i32>(2);
// IR-NEXT:             in 3 "i" [imm | sym] -> sym width 64 sym<offset=8>(%14);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %19 @rejected(%20 parameter: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %21 from_call: i32 [storage=automatic] [const] = call<i32, signature=fn() -> i32>(%15);
// IR-NEXT:         let %22 from_parameter: i32 [storage=automatic] [const] = read<i32>(%20);
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             in 0 "i" [imm | sym] width 32 read<i32, volatile>(%11);
// IR-NEXT:             in 1 "i" [imm | sym] width 32 read<i32>(%12);
// IR-NEXT:             in 2 "i" [imm | sym] width 32 read<i32>(%13);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "i" [imm | sym] width 32 read<i32>(%21);
// IR-NEXT:             in 1 "i" [imm | sym] width 32 read<i32>(%22);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
