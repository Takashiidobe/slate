// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
volatile int g;
_Atomic int counter;
_Atomic(long) total;
typedef volatile unsigned vu;
struct regs { volatile int status; int data; };

int f(void) { g = 1; return g; }

void copy(int *restrict dst, const int *restrict src, int n) {
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

void bump(void) {
    counter = 1;
    counter += 2;
    counter++;
    total = counter;
}

int poll(volatile int *p, struct regs *r, vu *u, int a[restrict volatile 4]) {
    *p = 0;
    r->status = 1;
    r->data = 2;
    *u = 3u;
    a[0] = 4;
    volatile int arr[2];
    arr[1] = 5;
    int *restrict q = &a[0];
    int x = g++;
    return *p + r->status + arr[0] + *q + x;
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
// IR-NEXT:     type @type[[TYPE_vu:[0-9]+]] vu = u32;
// IR-NEXT:     type @type[[TYPE_regs:[0-9]+]] regs = struct {
// IR-NEXT:         field0 status: volatile i32;
// IR-NEXT:         field1 data: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     global %[[VALUE_g:[0-9]+]] g: volatile i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_counter:[0-9]+]] counter: atomic i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_total:[0-9]+]] total: atomic i64 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32, volatile>(%[[VALUE_g]], const<i32>(1));
// IR-NEXT:         return read<i32, volatile>(%[[VALUE_g]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_dst:[0-9]+]] dst: ptr<i32> [restrict], %[[VALUE_src:[0-9]+]] src: ptr<const i32> [restrict], %[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         for %[[VALUE0:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_dst]]), read<i32>(%[[VALUE_i]]))), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_src]]), read<i32>(%[[VALUE_i]])))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_bump:[0-9]+]] @bump() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<i32, atomic=seq_cst>(%[[VALUE_counter]], const<i32>(1));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_counter]], add<i32, overflow=ub>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%[[VALUE_counter]], add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// IR-NEXT:         write<i64, atomic=seq_cst>(%[[VALUE_total]], widen<i64, reason=assign>(read<i32, atomic=seq_cst>(%[[VALUE_counter]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_poll:[0-9]+]] @poll(%[[VALUE_p:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_r:[0-9]+]] r: ptr<@type[[TYPE_regs]]>, %[[VALUE_u:[0-9]+]] u: ptr<volatile u32>, %[[VALUE_a:[0-9]+]] a: volatile ptr<i32> [restrict] [array=4]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32, volatile>(deref(read<ptr<volatile i32>>(%[[VALUE_p]])), const<i32>(0));
// IR-NEXT:         write<i32, volatile>(field0(deref(read<ptr<@type[[TYPE_regs]]>>(%[[VALUE_r]]))), const<i32>(1));
// IR-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_regs]]>>(%[[VALUE_r]]))), const<i32>(2));
// IR-NEXT:         write<u32, volatile>(deref(read<ptr<volatile u32>>(%[[VALUE_u]])), const<u32>(3));
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>, volatile>(%[[VALUE_a]]), const<i32>(0))), const<i32>(4));
// IR-NEXT:         let %[[VALUE_arr:[0-9]+]] arr: volatile array<i32, 2> [storage=automatic];
// IR-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(1))), const<i32>(5));
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=automatic] [restrict] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>, volatile>(%[[VALUE_a]]), const<i32>(0))));
// IR-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32, volatile>(%[[VALUE_g]]);
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// IR-NEXT:         write<i32, volatile>(%[[VALUE_g]], read<i32>(%[[VALUE6]]));
// IR-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE5]]));
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32, volatile>(deref(read<ptr<volatile i32>>(%[[VALUE_p]]))), read<i32, volatile>(field0(deref(read<ptr<@type[[TYPE_regs]]>>(%[[VALUE_r]]))))), read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(2)>(%[[VALUE_arr]]), const<i32>(0))))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_q]])))), read<i32>(%[[VALUE_x]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
