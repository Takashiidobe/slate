// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23

int sum(int *p, int n) {
    int s = 0;
    for (int i = 0; i < n; i++) {
        s += p[i];
    }
    for (unsigned j = 2; 10 > j; ++j) {
        s += p[j];
    }
    for (long k = 0; k <= 4; k += 1) {
        s += p[k];
    }
    for (int c = n / 2; c < n - 1; c = c + 1) {
        s += p[c];
    }
    return s;
}

int local_bound(int *p) {
    int m = 8;
    int s = 0;
    for (int i = 0; m > i; i++) {
        if (p[i] < 0) {
            continue;
        }
        if (p[i] > 100) {
            break;
        }
        s += p[i];
    }
    return s;
}

int nested(int rows, int cols, int (*grid)[4]) {
    int s = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            switch (grid[r][c]) {
            case 0:
                continue;
            default:
                s += grid[r][c];
                break;
            }
        }
    }
    return s;
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
// IR-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_s:[0-9]+]] s: i32 [storage=automatic] = const<i32>(0);
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
// IR-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..read<i32>(%[[VALUE_n]])
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_i]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE4]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE5:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_j:[0-9]+]] j: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(2));
// IR-NEXT:             condition: gt<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10)), read<u32>(%[[VALUE_j]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE6:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_j]]);
// IR-NEXT:                 let %[[VALUE7:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE6]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// IR-NEXT:                 write<u32>(%[[VALUE_j]], read<u32>(%[[VALUE7]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             counted: %[[VALUE_j]] in reinterpret<u32, reason=assign, fits=always>(const<i32>(2))..reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<u32>(%[[VALUE_j]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE9]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE10:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_k:[0-9]+]] k: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// IR-NEXT:             condition: le<i64>(read<i64>(%[[VALUE_k]]), widen<i64, reason=usual_arith>(const<i32>(4)))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE11:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_k]]);
// IR-NEXT:                 let %[[VALUE12:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE11]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// IR-NEXT:                 write<i64>(%[[VALUE_k]], read<i64>(%[[VALUE12]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             counted: %[[VALUE_k]] in widen<i64, reason=assign>(const<i32>(0))..=widen<i64, reason=usual_arith>(const<i32>(4))
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i64>(%[[VALUE_k]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE14]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE15:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_n]]), const<i32>(2));
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_c]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_n]]), const<i32>(1)))
// IR-NEXT:             increment: {
// IR-NEXT:                 write<i32>(%[[VALUE_c]], add<i32, overflow=ub>(read<i32>(%[[VALUE_c]]), const<i32>(1)));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             counted: %[[VALUE_c]] in div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_n]]), const<i32>(2))..sub<i32, overflow=ub>(read<i32>(%[[VALUE_n]]), const<i32>(1))
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_c]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE17]]));
// IR-NEXT:                 }
// IR-NEXT:         return read<i32>(%[[VALUE_s]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_local_bound:[0-9]+]] @local_bound(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_m:[0-9]+]] m: i32 [storage=automatic] = const<i32>(8);
// IR-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         for %[[VALUE18:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: gt<i32>(read<i32>(%[[VALUE_m]]), read<i32>(%[[VALUE_i_2]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// IR-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE20]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             counted: %[[VALUE_i_2]] in const<i32>(0)..read<i32>(%[[VALUE_m]])
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     if lt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_2]]), read<i32>(%[[VALUE_i_2]])))), const<i32>(0))
// IR-NEXT:                         {
// IR-NEXT:                             continue %[[VALUE18]];
// IR-NEXT:                         }
// IR-NEXT:                     if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_2]]), read<i32>(%[[VALUE_i_2]])))), const<i32>(100))
// IR-NEXT:                         {
// IR-NEXT:                             break %[[VALUE18]];
// IR-NEXT:                         }
// IR-NEXT:                     let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s_2]]);
// IR-NEXT:                     let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_2]]), read<i32>(%[[VALUE_i_2]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s_2]], read<i32>(%[[VALUE22]]));
// IR-NEXT:                 }
// IR-NEXT:         return read<i32>(%[[VALUE_s_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_rows:[0-9]+]] rows: i32, %[[VALUE_cols:[0-9]+]] cols: i32, %[[VALUE_grid:[0-9]+]] grid: ptr<array<i32, 4>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_s_3:[0-9]+]] s: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         for %[[VALUE23:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_r]]), read<i32>(%[[VALUE_rows]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r]]);
// IR-NEXT:                 let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_r]], read<i32>(%[[VALUE25]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             counted: %[[VALUE_r]] in const<i32>(0)..read<i32>(%[[VALUE_rows]])
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     for %[[VALUE26:[0-9]+]]
// IR-NEXT:                         init:
// IR-NEXT:                             let %[[VALUE_c_2:[0-9]+]] c: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_c_2]]), read<i32>(%[[VALUE_cols]]))
// IR-NEXT:                         increment: {
// IR-NEXT:                             let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c_2]]);
// IR-NEXT:                             let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// IR-NEXT:                             write<i32>(%[[VALUE_c_2]], read<i32>(%[[VALUE28]]));
// IR-NEXT:                             yield void;
// IR-NEXT:                         }
// IR-NEXT:                         counted: %[[VALUE_c_2]] in const<i32>(0)..read<i32>(%[[VALUE_cols]])
// IR-NEXT:                         body:
// IR-NEXT:                             {
// IR-NEXT:                                 switch %[[VALUE29:[0-9]+]] read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(read<ptr<array<i32, 4>>>(%[[VALUE_grid]]), read<i32>(%[[VALUE_r]])))), read<i32>(%[[VALUE_c_2]]))))
// IR-NEXT:                                     {
// IR-NEXT:                                         case %[[VALUE29]] const<i32>(0):
// IR-NEXT:                                             continue %[[VALUE26]];
// IR-NEXT:                                         default %[[VALUE29]]:
// IR-NEXT:                                             let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s_3]]);
// IR-NEXT:                                             let %[[VALUE31:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE30]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(read<ptr<array<i32, 4>>>(%[[VALUE_grid]]), read<i32>(%[[VALUE_r]])))), read<i32>(%[[VALUE_c_2]])))));
// IR-NEXT:                                             write<i32>(%[[VALUE_s_3]], read<i32>(%[[VALUE31]]));
// IR-NEXT:                                         break %[[VALUE29]];
// IR-NEXT:                                     }
// IR-NEXT:                             }
// IR-NEXT:                 }
// IR-NEXT:         return read<i32>(%[[VALUE_s_3]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
