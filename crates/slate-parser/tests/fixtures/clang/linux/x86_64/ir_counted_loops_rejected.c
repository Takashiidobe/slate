// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23

int limit;
int bound(void);
void take(int *pointer);

int rejected(int *p, int n, volatile int vn) {
    int s = 0;
    for (int a = 0; a < n; a++) {
        a += p[a];
    }
    for (int b = 0; b < n; b++) {
        take(&b);
    }
    int c;
    for (c = 0; c < n; c++) {
        s += p[c];
    }
    for (int d = 0; d < limit; d++) {
        s += p[d];
    }
    int m = n;
    for (int e = 0; e < m; e++) {
        m--;
    }
    for (unsigned f = 0; f <= 4294967295u; f++) {
        if (f == 3) {
            break;
        }
    }
    for (int g = 0; g <= n; g++) {
        s += p[g];
    }
    for (volatile int h = 0; h < n; h++) {
        s += p[h];
    }
    for (int i = 0; i < vn; i++) {
        s += p[i];
    }
    for (char j = 0; j < 4; j++) {
        s += p[j];
    }
    for (int k = 0; k < n; k += 2) {
        s += p[k];
    }
    for (int l = 0; l != n; l++) {
        s += p[l];
    }
    for (int q = 0; q < bound(); q++) {
        s += p[q];
    }
    for (int r = n; r > 0; r--) {
        s += p[r];
    }
    int taken = n;
    take(&taken);
    for (int t = 0; t < taken; t++) {
        s += p[t];
    }
    for (int u = 0; u < n; u++) {
        __asm__ volatile("" ::: "memory");
    }
    for (int v = 0; v < v + 5; v++) {
        if (v > 3) {
            break;
        }
    }
    return s;
}

int jumps(int *p, int n) {
    int s = 0;
    goto inside;
    for (int i = 0; i < n; i++) {
    inside:
        s += 1;
    }
    switch (n) {
        for (int j = 0; j < n; j++) {
        case 1:
            s += p[j];
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
// IR-NEXT:     global %[[VALUE_limit:[0-9]+]] limit: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_bound:[0-9]+]] @bound() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE_pointer:[0-9]+]] pointer: ptr<i32>) -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_rejected:[0-9]+]] @rejected(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_vn:[0-9]+]] vn: volatile i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_s:[0-9]+]] s: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         for %[[VALUE0:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_n]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE2]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_a]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE4]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE5:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_n]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// IR-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE7]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_take]], addr_of<ptr<i32>>(%[[VALUE_b]]));
// IR-NEXT:                 }
// IR-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// IR-NEXT:         for %[[VALUE8:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 write<i32>(%[[VALUE_c]], const<i32>(0));
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_c]]), read<i32>(%[[VALUE_n]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// IR-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE10]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_c]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE12]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE13:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_d]]), read<i32>(%[[VALUE_limit]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// IR-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE15]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_d]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE17]]));
// IR-NEXT:                 }
// IR-NEXT:         let %[[VALUE_m:[0-9]+]] m: i32 [storage=automatic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:         for %[[VALUE18:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e]]), read<i32>(%[[VALUE_m]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// IR-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE20]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_m]]);
// IR-NEXT:                     let %[[VALUE22:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// IR-NEXT:                     write<i32>(%[[VALUE_m]], read<i32>(%[[VALUE22]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE23:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_f:[0-9]+]] f: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// IR-NEXT:             condition: le<u32>(read<u32>(%[[VALUE_f]]), const<u32>(4294967295))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE24:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_f]]);
// IR-NEXT:                 let %[[VALUE25:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE24]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// IR-NEXT:                 write<u32>(%[[VALUE_f]], read<u32>(%[[VALUE25]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     if eq<u32>(read<u32>(%[[VALUE_f]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// IR-NEXT:                         {
// IR-NEXT:                             break %[[VALUE23]];
// IR-NEXT:                         }
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE26:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_g]]), read<i32>(%[[VALUE_n]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_g]]);
// IR-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_g]], read<i32>(%[[VALUE28]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE29:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE29]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_g]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE30]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE31:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_h:[0-9]+]] h: volatile i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32, volatile>(%[[VALUE_h]]), read<i32>(%[[VALUE_n]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32, volatile>(%[[VALUE_h]]);
// IR-NEXT:                 let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE32]]), const<i32>(1));
// IR-NEXT:                 write<i32, volatile>(%[[VALUE_h]], read<i32>(%[[VALUE33]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32, volatile>(%[[VALUE_h]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE35]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE36:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32, volatile>(%[[VALUE_vn]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:                 let %[[VALUE38:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE37]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE38]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE39:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE40:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE39]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_i]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE40]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE41:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_j:[0-9]+]] j: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// IR-NEXT:             condition: lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_j]])), const<i32>(4))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE42:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_j]]);
// IR-NEXT:                 let %[[VALUE43:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE42]])), const<i32>(1)));
// IR-NEXT:                 write<i8>(%[[VALUE_j]], read<i8>(%[[VALUE43]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE45:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE44]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE_j]]))))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE45]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE46:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_k]]), read<i32>(%[[VALUE_n]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE47:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// IR-NEXT:                 let %[[VALUE48:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE47]]), const<i32>(2));
// IR-NEXT:                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE48]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE49:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE50:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE49]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_k]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE50]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE51:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_l:[0-9]+]] l: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_l]]), read<i32>(%[[VALUE_n]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE52:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_l]]);
// IR-NEXT:                 let %[[VALUE53:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE52]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_l]], read<i32>(%[[VALUE53]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE54:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE55:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE54]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_l]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE55]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE56:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_q:[0-9]+]] q: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_bound]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE57:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_q]]);
// IR-NEXT:                 let %[[VALUE58:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE57]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_q]], read<i32>(%[[VALUE58]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE59:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE60:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE59]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_q]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE60]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE61:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:             condition: gt<i32>(read<i32>(%[[VALUE_r]]), const<i32>(0))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE62:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r]]);
// IR-NEXT:                 let %[[VALUE63:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE62]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_r]], read<i32>(%[[VALUE63]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE64:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE65:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE64]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_r]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE65]]));
// IR-NEXT:                 }
// IR-NEXT:         let %[[VALUE_taken:[0-9]+]] taken: i32 [storage=automatic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_take]], addr_of<ptr<i32>>(%[[VALUE_taken]]));
// IR-NEXT:         for %[[VALUE66:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_t:[0-9]+]] t: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_t]]), read<i32>(%[[VALUE_taken]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE67:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_t]]);
// IR-NEXT:                 let %[[VALUE68:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE67]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_t]], read<i32>(%[[VALUE68]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     let %[[VALUE69:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                     let %[[VALUE70:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE69]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p]]), read<i32>(%[[VALUE_t]])))));
// IR-NEXT:                     write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE70]]));
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE71:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_u:[0-9]+]] u: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_u]]), read<i32>(%[[VALUE_n]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE72:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_u]]);
// IR-NEXT:                 let %[[VALUE73:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE72]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_u]], read<i32>(%[[VALUE73]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     asm volatile "" [dialect=att] [options=nostack] {
// IR-NEXT:                         clobbers: memory;
// IR-NEXT:                     }
// IR-NEXT:                 }
// IR-NEXT:         for %[[VALUE74:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_v:[0-9]+]] v: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_v]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_v]]), const<i32>(5)))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE75:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_v]]);
// IR-NEXT:                 let %[[VALUE76:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE75]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_v]], read<i32>(%[[VALUE76]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     if gt<i32>(read<i32>(%[[VALUE_v]]), const<i32>(3))
// IR-NEXT:                         {
// IR-NEXT:                             break %[[VALUE74]];
// IR-NEXT:                         }
// IR-NEXT:                 }
// IR-NEXT:         return read<i32>(%[[VALUE_s]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_jumps:[0-9]+]] @jumps(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>, %[[VALUE_n_2:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         goto %[[VALUE_inside:[0-9]+]];
// IR-NEXT:         for %[[VALUE77:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_n_2]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE78:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// IR-NEXT:                 let %[[VALUE79:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE78]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE79]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     label %[[VALUE_inside]] inside:
// IR-NEXT:                         let %[[VALUE80:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s_2]]);
// IR-NEXT:                         let %[[VALUE81:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE80]]), const<i32>(1));
// IR-NEXT:                         write<i32>(%[[VALUE_s_2]], read<i32>(%[[VALUE81]]));
// IR-NEXT:                 }
// IR-NEXT:         switch %[[VALUE82:[0-9]+]] read<i32>(%[[VALUE_n_2]])
// IR-NEXT:             {
// IR-NEXT:                 for %[[VALUE83:[0-9]+]]
// IR-NEXT:                     init:
// IR-NEXT:                         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_j_2]]), read<i32>(%[[VALUE_n_2]]))
// IR-NEXT:                     increment: {
// IR-NEXT:                         let %[[VALUE84:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_2]]);
// IR-NEXT:                         let %[[VALUE85:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE84]]), const<i32>(1));
// IR-NEXT:                         write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE85]]));
// IR-NEXT:                         yield void;
// IR-NEXT:                     }
// IR-NEXT:                     body:
// IR-NEXT:                         {
// IR-NEXT:                             case %[[VALUE82]] const<i32>(1):
// IR-NEXT:                                 let %[[VALUE86:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s_2]]);
// IR-NEXT:                                 let %[[VALUE87:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE86]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_2]]), read<i32>(%[[VALUE_j_2]])))));
// IR-NEXT:                                 write<i32>(%[[VALUE_s_2]], read<i32>(%[[VALUE87]]));
// IR-NEXT:                         }
// IR-NEXT:             }
// IR-NEXT:         return read<i32>(%[[VALUE_s_2]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
