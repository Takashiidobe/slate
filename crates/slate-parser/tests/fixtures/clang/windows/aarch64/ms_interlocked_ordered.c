// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wno-implicit-function-declaration

char And8_acq(char volatile *p, char v) { return _InterlockedAnd8_acq(p, v); }
char Or8_acq(char volatile *p, char v) { return _InterlockedOr8_acq(p, v); }
char Xor8_acq(char volatile *p, char v) { return _InterlockedXor8_acq(p, v); }
char ExchangeAdd8_acq(char volatile *p, char v) { return _InterlockedExchangeAdd8_acq(p, v); }
char Exchange8_acq(char volatile *p, char v) { return _InterlockedExchange8_acq(p, v); }
char CompareExchange8_acq(char volatile *p, char x, char c) { return _InterlockedCompareExchange8_acq(p, x, c); }
short And16_acq(short volatile *p, short v) { return _InterlockedAnd16_acq(p, v); }
short Or16_acq(short volatile *p, short v) { return _InterlockedOr16_acq(p, v); }
short Xor16_acq(short volatile *p, short v) { return _InterlockedXor16_acq(p, v); }
short ExchangeAdd16_acq(short volatile *p, short v) { return _InterlockedExchangeAdd16_acq(p, v); }
short Exchange16_acq(short volatile *p, short v) { return _InterlockedExchange16_acq(p, v); }
short CompareExchange16_acq(short volatile *p, short x, short c) { return _InterlockedCompareExchange16_acq(p, x, c); }
short Increment16_acq(short volatile *p) { return _InterlockedIncrement16_acq(p); }
short Decrement16_acq(short volatile *p) { return _InterlockedDecrement16_acq(p); }
long And_acq(long volatile *p, long v) { return _InterlockedAnd_acq(p, v); }
long Or_acq(long volatile *p, long v) { return _InterlockedOr_acq(p, v); }
long Xor_acq(long volatile *p, long v) { return _InterlockedXor_acq(p, v); }
long ExchangeAdd_acq(long volatile *p, long v) { return _InterlockedExchangeAdd_acq(p, v); }
long Exchange_acq(long volatile *p, long v) { return _InterlockedExchange_acq(p, v); }
long CompareExchange_acq(long volatile *p, long x, long c) { return _InterlockedCompareExchange_acq(p, x, c); }
long Increment_acq(long volatile *p) { return _InterlockedIncrement_acq(p); }
long Decrement_acq(long volatile *p) { return _InterlockedDecrement_acq(p); }
__int64 And64_acq(__int64 volatile *p, __int64 v) { return _InterlockedAnd64_acq(p, v); }
__int64 Or64_acq(__int64 volatile *p, __int64 v) { return _InterlockedOr64_acq(p, v); }
__int64 Xor64_acq(__int64 volatile *p, __int64 v) { return _InterlockedXor64_acq(p, v); }
__int64 ExchangeAdd64_acq(__int64 volatile *p, __int64 v) { return _InterlockedExchangeAdd64_acq(p, v); }
__int64 Exchange64_acq(__int64 volatile *p, __int64 v) { return _InterlockedExchange64_acq(p, v); }
__int64 CompareExchange64_acq(__int64 volatile *p, __int64 x, __int64 c) { return _InterlockedCompareExchange64_acq(p, x, c); }
__int64 Increment64_acq(__int64 volatile *p) { return _InterlockedIncrement64_acq(p); }
__int64 Decrement64_acq(__int64 volatile *p) { return _InterlockedDecrement64_acq(p); }
void *ExchangePointer_acq(void *volatile *p, void *v) { return _InterlockedExchangePointer_acq(p, v); }
void *CompareExchangePointer_acq(void *volatile *p, void *x, void *c) { return _InterlockedCompareExchangePointer_acq(p, x, c); }
long Add_acq(long volatile *p, long v) { return _InterlockedAdd_acq(p, v); }
__int64 Add64_acq(__int64 volatile *p, __int64 v) { return _InterlockedAdd64_acq(p, v); }
unsigned char Cmpxchg128_acq(__int64 volatile *d, __int64 h, __int64 l, __int64 *c) { return _InterlockedCompareExchange128_acq(d, h, l, c); }
char And8_rel(char volatile *p, char v) { return _InterlockedAnd8_rel(p, v); }
char Or8_rel(char volatile *p, char v) { return _InterlockedOr8_rel(p, v); }
char Xor8_rel(char volatile *p, char v) { return _InterlockedXor8_rel(p, v); }
char ExchangeAdd8_rel(char volatile *p, char v) { return _InterlockedExchangeAdd8_rel(p, v); }
char Exchange8_rel(char volatile *p, char v) { return _InterlockedExchange8_rel(p, v); }
char CompareExchange8_rel(char volatile *p, char x, char c) { return _InterlockedCompareExchange8_rel(p, x, c); }
short And16_rel(short volatile *p, short v) { return _InterlockedAnd16_rel(p, v); }
short Or16_rel(short volatile *p, short v) { return _InterlockedOr16_rel(p, v); }
short Xor16_rel(short volatile *p, short v) { return _InterlockedXor16_rel(p, v); }
short ExchangeAdd16_rel(short volatile *p, short v) { return _InterlockedExchangeAdd16_rel(p, v); }
short Exchange16_rel(short volatile *p, short v) { return _InterlockedExchange16_rel(p, v); }
short CompareExchange16_rel(short volatile *p, short x, short c) { return _InterlockedCompareExchange16_rel(p, x, c); }
short Increment16_rel(short volatile *p) { return _InterlockedIncrement16_rel(p); }
short Decrement16_rel(short volatile *p) { return _InterlockedDecrement16_rel(p); }
long And_rel(long volatile *p, long v) { return _InterlockedAnd_rel(p, v); }
long Or_rel(long volatile *p, long v) { return _InterlockedOr_rel(p, v); }
long Xor_rel(long volatile *p, long v) { return _InterlockedXor_rel(p, v); }
long ExchangeAdd_rel(long volatile *p, long v) { return _InterlockedExchangeAdd_rel(p, v); }
long Exchange_rel(long volatile *p, long v) { return _InterlockedExchange_rel(p, v); }
long CompareExchange_rel(long volatile *p, long x, long c) { return _InterlockedCompareExchange_rel(p, x, c); }
long Increment_rel(long volatile *p) { return _InterlockedIncrement_rel(p); }
long Decrement_rel(long volatile *p) { return _InterlockedDecrement_rel(p); }
__int64 And64_rel(__int64 volatile *p, __int64 v) { return _InterlockedAnd64_rel(p, v); }
__int64 Or64_rel(__int64 volatile *p, __int64 v) { return _InterlockedOr64_rel(p, v); }
__int64 Xor64_rel(__int64 volatile *p, __int64 v) { return _InterlockedXor64_rel(p, v); }
__int64 ExchangeAdd64_rel(__int64 volatile *p, __int64 v) { return _InterlockedExchangeAdd64_rel(p, v); }
__int64 Exchange64_rel(__int64 volatile *p, __int64 v) { return _InterlockedExchange64_rel(p, v); }
__int64 CompareExchange64_rel(__int64 volatile *p, __int64 x, __int64 c) { return _InterlockedCompareExchange64_rel(p, x, c); }
__int64 Increment64_rel(__int64 volatile *p) { return _InterlockedIncrement64_rel(p); }
__int64 Decrement64_rel(__int64 volatile *p) { return _InterlockedDecrement64_rel(p); }
void *ExchangePointer_rel(void *volatile *p, void *v) { return _InterlockedExchangePointer_rel(p, v); }
void *CompareExchangePointer_rel(void *volatile *p, void *x, void *c) { return _InterlockedCompareExchangePointer_rel(p, x, c); }
long Add_rel(long volatile *p, long v) { return _InterlockedAdd_rel(p, v); }
__int64 Add64_rel(__int64 volatile *p, __int64 v) { return _InterlockedAdd64_rel(p, v); }
unsigned char Cmpxchg128_rel(__int64 volatile *d, __int64 h, __int64 l, __int64 *c) { return _InterlockedCompareExchange128_rel(d, h, l, c); }
char And8_nf(char volatile *p, char v) { return _InterlockedAnd8_nf(p, v); }
char Or8_nf(char volatile *p, char v) { return _InterlockedOr8_nf(p, v); }
char Xor8_nf(char volatile *p, char v) { return _InterlockedXor8_nf(p, v); }
char ExchangeAdd8_nf(char volatile *p, char v) { return _InterlockedExchangeAdd8_nf(p, v); }
char Exchange8_nf(char volatile *p, char v) { return _InterlockedExchange8_nf(p, v); }
char CompareExchange8_nf(char volatile *p, char x, char c) { return _InterlockedCompareExchange8_nf(p, x, c); }
short And16_nf(short volatile *p, short v) { return _InterlockedAnd16_nf(p, v); }
short Or16_nf(short volatile *p, short v) { return _InterlockedOr16_nf(p, v); }
short Xor16_nf(short volatile *p, short v) { return _InterlockedXor16_nf(p, v); }
short ExchangeAdd16_nf(short volatile *p, short v) { return _InterlockedExchangeAdd16_nf(p, v); }
short Exchange16_nf(short volatile *p, short v) { return _InterlockedExchange16_nf(p, v); }
short CompareExchange16_nf(short volatile *p, short x, short c) { return _InterlockedCompareExchange16_nf(p, x, c); }
short Increment16_nf(short volatile *p) { return _InterlockedIncrement16_nf(p); }
short Decrement16_nf(short volatile *p) { return _InterlockedDecrement16_nf(p); }
long And_nf(long volatile *p, long v) { return _InterlockedAnd_nf(p, v); }
long Or_nf(long volatile *p, long v) { return _InterlockedOr_nf(p, v); }
long Xor_nf(long volatile *p, long v) { return _InterlockedXor_nf(p, v); }
long ExchangeAdd_nf(long volatile *p, long v) { return _InterlockedExchangeAdd_nf(p, v); }
long Exchange_nf(long volatile *p, long v) { return _InterlockedExchange_nf(p, v); }
long CompareExchange_nf(long volatile *p, long x, long c) { return _InterlockedCompareExchange_nf(p, x, c); }
long Increment_nf(long volatile *p) { return _InterlockedIncrement_nf(p); }
long Decrement_nf(long volatile *p) { return _InterlockedDecrement_nf(p); }
__int64 And64_nf(__int64 volatile *p, __int64 v) { return _InterlockedAnd64_nf(p, v); }
__int64 Or64_nf(__int64 volatile *p, __int64 v) { return _InterlockedOr64_nf(p, v); }
__int64 Xor64_nf(__int64 volatile *p, __int64 v) { return _InterlockedXor64_nf(p, v); }
__int64 ExchangeAdd64_nf(__int64 volatile *p, __int64 v) { return _InterlockedExchangeAdd64_nf(p, v); }
__int64 Exchange64_nf(__int64 volatile *p, __int64 v) { return _InterlockedExchange64_nf(p, v); }
__int64 CompareExchange64_nf(__int64 volatile *p, __int64 x, __int64 c) { return _InterlockedCompareExchange64_nf(p, x, c); }
__int64 Increment64_nf(__int64 volatile *p) { return _InterlockedIncrement64_nf(p); }
__int64 Decrement64_nf(__int64 volatile *p) { return _InterlockedDecrement64_nf(p); }
void *ExchangePointer_nf(void *volatile *p, void *v) { return _InterlockedExchangePointer_nf(p, v); }
void *CompareExchangePointer_nf(void *volatile *p, void *x, void *c) { return _InterlockedCompareExchangePointer_nf(p, x, c); }
long Add_nf(long volatile *p, long v) { return _InterlockedAdd_nf(p, v); }
__int64 Add64_nf(__int64 volatile *p, __int64 v) { return _InterlockedAdd64_nf(p, v); }
unsigned char Cmpxchg128_nf(__int64 volatile *d, __int64 h, __int64 l, __int64 *c) { return _InterlockedCompareExchange128_nf(d, h, l, c); }
long Add(long volatile *p, long v) { return _InterlockedAdd(p, v); }
__int64 Add64(__int64 volatile *p, __int64 v) { return _InterlockedAdd64(p, v); }
unsigned char Cmpxchg128(__int64 volatile *d, __int64 h, __int64 l, __int64 *c) { return _InterlockedCompareExchange128(d, h, l, c); }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And8_acq:[0-9]+]] @And8_acq(%[[VALUE_p:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i8>>(%[[VALUE_p]])), and<i8>(old<i8>, read<i8>(%[[VALUE_v]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or8_acq:[0-9]+]] @Or8_acq(%[[VALUE_p_2:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_2:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i8>>(%[[VALUE_p_2]])), or<i8>(old<i8>, read<i8>(%[[VALUE_v_2]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor8_acq:[0-9]+]] @Xor8_acq(%[[VALUE_p_3:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_3:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i8>>(%[[VALUE_p_3]])), xor<i8>(old<i8>, read<i8>(%[[VALUE_v_3]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd8_acq:[0-9]+]] @ExchangeAdd8_acq(%[[VALUE_p_4:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_4:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i8>>(%[[VALUE_p_4]])), add<i8>(old<i8>, read<i8>(%[[VALUE_v_4]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange8_acq:[0-9]+]] @Exchange8_acq(%[[VALUE_p_5:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_5:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i8>>(%[[VALUE_p_5]])), read<i8>(%[[VALUE_v_5]]));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange8_acq:[0-9]+]] @CompareExchange8_acq(%[[VALUE_p_6:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_x:[0-9]+]] x: i8, %[[VALUE_c:[0-9]+]] c: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i8 [synthetic] = compare_exchange<i8, volatile, form=old, weak=false, success=acquire, failure=acquire>(deref(read<ptr<volatile i8>>(%[[VALUE_p_6]])), read<i8>(%[[VALUE_c]]), read<i8>(%[[VALUE_x]]));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And16_acq:[0-9]+]] @And16_acq(%[[VALUE_p_7:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_6:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i16>>(%[[VALUE_p_7]])), and<i16>(old<i16>, read<i16>(%[[VALUE_v_6]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or16_acq:[0-9]+]] @Or16_acq(%[[VALUE_p_8:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_7:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i16>>(%[[VALUE_p_8]])), or<i16>(old<i16>, read<i16>(%[[VALUE_v_7]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor16_acq:[0-9]+]] @Xor16_acq(%[[VALUE_p_9:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_8:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i16>>(%[[VALUE_p_9]])), xor<i16>(old<i16>, read<i16>(%[[VALUE_v_8]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd16_acq:[0-9]+]] @ExchangeAdd16_acq(%[[VALUE_p_10:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_9:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i16>>(%[[VALUE_p_10]])), add<i16>(old<i16>, read<i16>(%[[VALUE_v_9]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange16_acq:[0-9]+]] @Exchange16_acq(%[[VALUE_p_11:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_10:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i16>>(%[[VALUE_p_11]])), read<i16>(%[[VALUE_v_10]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE10]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange16_acq:[0-9]+]] @CompareExchange16_acq(%[[VALUE_p_12:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_x_2:[0-9]+]] x: i16, %[[VALUE_c_2:[0-9]+]] c: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i16 [synthetic] = compare_exchange<i16, volatile, form=old, weak=false, success=acquire, failure=acquire>(deref(read<ptr<volatile i16>>(%[[VALUE_p_12]])), read<i16>(%[[VALUE_c_2]]), read<i16>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE11]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Increment16_acq:[0-9]+]] @Increment16_acq(%[[VALUE_p_13:[0-9]+]] p: ptr<volatile i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=acquire>(deref(read<ptr<volatile i16>>(%[[VALUE_p_13]])), add<i16>(old<i16>, const<i16>(1)));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE12]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Decrement16_acq:[0-9]+]] @Decrement16_acq(%[[VALUE_p_14:[0-9]+]] p: ptr<volatile i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=acquire>(deref(read<ptr<volatile i16>>(%[[VALUE_p_14]])), sub<i16>(old<i16>, const<i16>(1)));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE13]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And_acq:[0-9]+]] @And_acq(%[[VALUE_p_15:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_11:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i32>>(%[[VALUE_p_15]])), and<i32>(old<i32>, read<i32>(%[[VALUE_v_11]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE14]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or_acq:[0-9]+]] @Or_acq(%[[VALUE_p_16:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_12:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i32>>(%[[VALUE_p_16]])), or<i32>(old<i32>, read<i32>(%[[VALUE_v_12]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE15]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor_acq:[0-9]+]] @Xor_acq(%[[VALUE_p_17:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_13:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i32>>(%[[VALUE_p_17]])), xor<i32>(old<i32>, read<i32>(%[[VALUE_v_13]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE16]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd_acq:[0-9]+]] @ExchangeAdd_acq(%[[VALUE_p_18:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_14:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i32>>(%[[VALUE_p_18]])), add<i32>(old<i32>, read<i32>(%[[VALUE_v_14]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE17]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange_acq:[0-9]+]] @Exchange_acq(%[[VALUE_p_19:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_15:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i32>>(%[[VALUE_p_19]])), read<i32>(%[[VALUE_v_15]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE18]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange_acq:[0-9]+]] @CompareExchange_acq(%[[VALUE_p_20:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_c_3:[0-9]+]] c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = compare_exchange<i32, volatile, form=old, weak=false, success=acquire, failure=acquire>(deref(read<ptr<volatile i32>>(%[[VALUE_p_20]])), read<i32>(%[[VALUE_c_3]]), read<i32>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE19]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Increment_acq:[0-9]+]] @Increment_acq(%[[VALUE_p_21:[0-9]+]] p: ptr<volatile i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=acquire>(deref(read<ptr<volatile i32>>(%[[VALUE_p_21]])), add<i32>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE20]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Decrement_acq:[0-9]+]] @Decrement_acq(%[[VALUE_p_22:[0-9]+]] p: ptr<volatile i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=acquire>(deref(read<ptr<volatile i32>>(%[[VALUE_p_22]])), sub<i32>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE21]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And64_acq:[0-9]+]] @And64_acq(%[[VALUE_p_23:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_16:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i64>>(%[[VALUE_p_23]])), and<i64>(old<i64>, read<i64>(%[[VALUE_v_16]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE22]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or64_acq:[0-9]+]] @Or64_acq(%[[VALUE_p_24:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_17:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i64>>(%[[VALUE_p_24]])), or<i64>(old<i64>, read<i64>(%[[VALUE_v_17]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE23]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor64_acq:[0-9]+]] @Xor64_acq(%[[VALUE_p_25:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_18:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i64>>(%[[VALUE_p_25]])), xor<i64>(old<i64>, read<i64>(%[[VALUE_v_18]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE24]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd64_acq:[0-9]+]] @ExchangeAdd64_acq(%[[VALUE_p_26:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_19:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i64>>(%[[VALUE_p_26]])), add<i64>(old<i64>, read<i64>(%[[VALUE_v_19]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE25]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange64_acq:[0-9]+]] @Exchange64_acq(%[[VALUE_p_27:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_20:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile i64>>(%[[VALUE_p_27]])), read<i64>(%[[VALUE_v_20]]));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE26]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange64_acq:[0-9]+]] @CompareExchange64_acq(%[[VALUE_p_28:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_x_4:[0-9]+]] x: i64, %[[VALUE_c_4:[0-9]+]] c: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i64 [synthetic] = compare_exchange<i64, volatile, form=old, weak=false, success=acquire, failure=acquire>(deref(read<ptr<volatile i64>>(%[[VALUE_p_28]])), read<i64>(%[[VALUE_c_4]]), read<i64>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE27]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Increment64_acq:[0-9]+]] @Increment64_acq(%[[VALUE_p_29:[0-9]+]] p: ptr<volatile i64>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=acquire>(deref(read<ptr<volatile i64>>(%[[VALUE_p_29]])), add<i64>(old<i64>, const<i64>(1)));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE28]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Decrement64_acq:[0-9]+]] @Decrement64_acq(%[[VALUE_p_30:[0-9]+]] p: ptr<volatile i64>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=acquire>(deref(read<ptr<volatile i64>>(%[[VALUE_p_30]])), sub<i64>(old<i64>, const<i64>(1)));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE29]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangePointer_acq:[0-9]+]] @ExchangePointer_acq(%[[VALUE_p_31:[0-9]+]] p: ptr<volatile ptr<void>>, %[[VALUE_v_21:[0-9]+]] v: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: ptr<void> [synthetic] = update<ptr<void>, result=old, volatile, atomic=acquire>(deref(read<ptr<volatile ptr<void>>>(%[[VALUE_p_31]])), read<ptr<void>>(%[[VALUE_v_21]]));
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE30]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchangePointer_acq:[0-9]+]] @CompareExchangePointer_acq(%[[VALUE_p_32:[0-9]+]] p: ptr<volatile ptr<void>>, %[[VALUE_x_5:[0-9]+]] x: ptr<void>, %[[VALUE_c_5:[0-9]+]] c: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: ptr<void> [synthetic] = compare_exchange<ptr<void>, volatile, form=old, weak=false, success=acquire, failure=acquire>(deref(read<ptr<volatile ptr<void>>>(%[[VALUE_p_32]])), read<ptr<void>>(%[[VALUE_c_5]]), read<ptr<void>>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE31]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Add_acq:[0-9]+]] @Add_acq(%[[VALUE_p_33:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_22:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=acquire>(deref(read<ptr<volatile i32>>(%[[VALUE_p_33]])), add<i32>(old<i32>, read<i32>(%[[VALUE_v_22]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE32]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Add64_acq:[0-9]+]] @Add64_acq(%[[VALUE_p_34:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_23:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=acquire>(deref(read<ptr<volatile i64>>(%[[VALUE_p_34]])), add<i64>(old<i64>, read<i64>(%[[VALUE_v_23]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE33]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Cmpxchg128_acq:[0-9]+]] @Cmpxchg128_acq(%[[VALUE_d:[0-9]+]] d: ptr<volatile i64>, %[[VALUE_h:[0-9]+]] h: i64, %[[VALUE_l:[0-9]+]] l: i64, %[[VALUE_c_6:[0-9]+]] c: ptr<i64>) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: bool [synthetic] = compare_exchange<u128, volatile, form=write_back, weak=false, success=acquire, failure=acquire>(deref(pointer_cast<ptr<volatile u128>>(read<ptr<volatile i64>>(%[[VALUE_d]]))), pointer_cast<ptr<u128>>(read<ptr<i64>>(%[[VALUE_c_6]])), or<u128>(shl<u128>(widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_h]]))), const<u128>(64)), widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_l]])))));
// DEFAULT-NEXT:         return from_bool<u8>(read<bool>(%[[VALUE34]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And8_rel:[0-9]+]] @And8_rel(%[[VALUE_p_35:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_24:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=release>(deref(read<ptr<volatile i8>>(%[[VALUE_p_35]])), and<i8>(old<i8>, read<i8>(%[[VALUE_v_24]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE35]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or8_rel:[0-9]+]] @Or8_rel(%[[VALUE_p_36:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_25:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=release>(deref(read<ptr<volatile i8>>(%[[VALUE_p_36]])), or<i8>(old<i8>, read<i8>(%[[VALUE_v_25]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE36]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor8_rel:[0-9]+]] @Xor8_rel(%[[VALUE_p_37:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_26:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=release>(deref(read<ptr<volatile i8>>(%[[VALUE_p_37]])), xor<i8>(old<i8>, read<i8>(%[[VALUE_v_26]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE37]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd8_rel:[0-9]+]] @ExchangeAdd8_rel(%[[VALUE_p_38:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_27:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=release>(deref(read<ptr<volatile i8>>(%[[VALUE_p_38]])), add<i8>(old<i8>, read<i8>(%[[VALUE_v_27]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE38]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange8_rel:[0-9]+]] @Exchange8_rel(%[[VALUE_p_39:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_28:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=release>(deref(read<ptr<volatile i8>>(%[[VALUE_p_39]])), read<i8>(%[[VALUE_v_28]]));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE39]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange8_rel:[0-9]+]] @CompareExchange8_rel(%[[VALUE_p_40:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_x_6:[0-9]+]] x: i8, %[[VALUE_c_7:[0-9]+]] c: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i8 [synthetic] = compare_exchange<i8, volatile, form=old, weak=false, success=release, failure=relaxed>(deref(read<ptr<volatile i8>>(%[[VALUE_p_40]])), read<i8>(%[[VALUE_c_7]]), read<i8>(%[[VALUE_x_6]]));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE40]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And16_rel:[0-9]+]] @And16_rel(%[[VALUE_p_41:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_29:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=release>(deref(read<ptr<volatile i16>>(%[[VALUE_p_41]])), and<i16>(old<i16>, read<i16>(%[[VALUE_v_29]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE41]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or16_rel:[0-9]+]] @Or16_rel(%[[VALUE_p_42:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_30:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=release>(deref(read<ptr<volatile i16>>(%[[VALUE_p_42]])), or<i16>(old<i16>, read<i16>(%[[VALUE_v_30]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE42]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor16_rel:[0-9]+]] @Xor16_rel(%[[VALUE_p_43:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_31:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=release>(deref(read<ptr<volatile i16>>(%[[VALUE_p_43]])), xor<i16>(old<i16>, read<i16>(%[[VALUE_v_31]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE43]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd16_rel:[0-9]+]] @ExchangeAdd16_rel(%[[VALUE_p_44:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_32:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=release>(deref(read<ptr<volatile i16>>(%[[VALUE_p_44]])), add<i16>(old<i16>, read<i16>(%[[VALUE_v_32]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE44]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange16_rel:[0-9]+]] @Exchange16_rel(%[[VALUE_p_45:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_33:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=release>(deref(read<ptr<volatile i16>>(%[[VALUE_p_45]])), read<i16>(%[[VALUE_v_33]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE45]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange16_rel:[0-9]+]] @CompareExchange16_rel(%[[VALUE_p_46:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_x_7:[0-9]+]] x: i16, %[[VALUE_c_8:[0-9]+]] c: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i16 [synthetic] = compare_exchange<i16, volatile, form=old, weak=false, success=release, failure=relaxed>(deref(read<ptr<volatile i16>>(%[[VALUE_p_46]])), read<i16>(%[[VALUE_c_8]]), read<i16>(%[[VALUE_x_7]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE46]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Increment16_rel:[0-9]+]] @Increment16_rel(%[[VALUE_p_47:[0-9]+]] p: ptr<volatile i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=release>(deref(read<ptr<volatile i16>>(%[[VALUE_p_47]])), add<i16>(old<i16>, const<i16>(1)));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE47]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Decrement16_rel:[0-9]+]] @Decrement16_rel(%[[VALUE_p_48:[0-9]+]] p: ptr<volatile i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=release>(deref(read<ptr<volatile i16>>(%[[VALUE_p_48]])), sub<i16>(old<i16>, const<i16>(1)));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE48]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And_rel:[0-9]+]] @And_rel(%[[VALUE_p_49:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_34:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=release>(deref(read<ptr<volatile i32>>(%[[VALUE_p_49]])), and<i32>(old<i32>, read<i32>(%[[VALUE_v_34]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE49]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or_rel:[0-9]+]] @Or_rel(%[[VALUE_p_50:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_35:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=release>(deref(read<ptr<volatile i32>>(%[[VALUE_p_50]])), or<i32>(old<i32>, read<i32>(%[[VALUE_v_35]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE50]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor_rel:[0-9]+]] @Xor_rel(%[[VALUE_p_51:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_36:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=release>(deref(read<ptr<volatile i32>>(%[[VALUE_p_51]])), xor<i32>(old<i32>, read<i32>(%[[VALUE_v_36]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE51]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd_rel:[0-9]+]] @ExchangeAdd_rel(%[[VALUE_p_52:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_37:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=release>(deref(read<ptr<volatile i32>>(%[[VALUE_p_52]])), add<i32>(old<i32>, read<i32>(%[[VALUE_v_37]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE52]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange_rel:[0-9]+]] @Exchange_rel(%[[VALUE_p_53:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_38:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=release>(deref(read<ptr<volatile i32>>(%[[VALUE_p_53]])), read<i32>(%[[VALUE_v_38]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE53]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange_rel:[0-9]+]] @CompareExchange_rel(%[[VALUE_p_54:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_x_8:[0-9]+]] x: i32, %[[VALUE_c_9:[0-9]+]] c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = compare_exchange<i32, volatile, form=old, weak=false, success=release, failure=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_54]])), read<i32>(%[[VALUE_c_9]]), read<i32>(%[[VALUE_x_8]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE54]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Increment_rel:[0-9]+]] @Increment_rel(%[[VALUE_p_55:[0-9]+]] p: ptr<volatile i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=release>(deref(read<ptr<volatile i32>>(%[[VALUE_p_55]])), add<i32>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE55]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Decrement_rel:[0-9]+]] @Decrement_rel(%[[VALUE_p_56:[0-9]+]] p: ptr<volatile i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=release>(deref(read<ptr<volatile i32>>(%[[VALUE_p_56]])), sub<i32>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE56]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And64_rel:[0-9]+]] @And64_rel(%[[VALUE_p_57:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_39:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=release>(deref(read<ptr<volatile i64>>(%[[VALUE_p_57]])), and<i64>(old<i64>, read<i64>(%[[VALUE_v_39]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE57]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or64_rel:[0-9]+]] @Or64_rel(%[[VALUE_p_58:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_40:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=release>(deref(read<ptr<volatile i64>>(%[[VALUE_p_58]])), or<i64>(old<i64>, read<i64>(%[[VALUE_v_40]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE58]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor64_rel:[0-9]+]] @Xor64_rel(%[[VALUE_p_59:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_41:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=release>(deref(read<ptr<volatile i64>>(%[[VALUE_p_59]])), xor<i64>(old<i64>, read<i64>(%[[VALUE_v_41]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE59]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd64_rel:[0-9]+]] @ExchangeAdd64_rel(%[[VALUE_p_60:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_42:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=release>(deref(read<ptr<volatile i64>>(%[[VALUE_p_60]])), add<i64>(old<i64>, read<i64>(%[[VALUE_v_42]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE60]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange64_rel:[0-9]+]] @Exchange64_rel(%[[VALUE_p_61:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_43:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=release>(deref(read<ptr<volatile i64>>(%[[VALUE_p_61]])), read<i64>(%[[VALUE_v_43]]));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE61]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange64_rel:[0-9]+]] @CompareExchange64_rel(%[[VALUE_p_62:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_x_9:[0-9]+]] x: i64, %[[VALUE_c_10:[0-9]+]] c: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: i64 [synthetic] = compare_exchange<i64, volatile, form=old, weak=false, success=release, failure=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_62]])), read<i64>(%[[VALUE_c_10]]), read<i64>(%[[VALUE_x_9]]));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE62]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Increment64_rel:[0-9]+]] @Increment64_rel(%[[VALUE_p_63:[0-9]+]] p: ptr<volatile i64>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=release>(deref(read<ptr<volatile i64>>(%[[VALUE_p_63]])), add<i64>(old<i64>, const<i64>(1)));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE63]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Decrement64_rel:[0-9]+]] @Decrement64_rel(%[[VALUE_p_64:[0-9]+]] p: ptr<volatile i64>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=release>(deref(read<ptr<volatile i64>>(%[[VALUE_p_64]])), sub<i64>(old<i64>, const<i64>(1)));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE64]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangePointer_rel:[0-9]+]] @ExchangePointer_rel(%[[VALUE_p_65:[0-9]+]] p: ptr<volatile ptr<void>>, %[[VALUE_v_44:[0-9]+]] v: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: ptr<void> [synthetic] = update<ptr<void>, result=old, volatile, atomic=release>(deref(read<ptr<volatile ptr<void>>>(%[[VALUE_p_65]])), read<ptr<void>>(%[[VALUE_v_44]]));
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE65]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchangePointer_rel:[0-9]+]] @CompareExchangePointer_rel(%[[VALUE_p_66:[0-9]+]] p: ptr<volatile ptr<void>>, %[[VALUE_x_10:[0-9]+]] x: ptr<void>, %[[VALUE_c_11:[0-9]+]] c: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: ptr<void> [synthetic] = compare_exchange<ptr<void>, volatile, form=old, weak=false, success=release, failure=relaxed>(deref(read<ptr<volatile ptr<void>>>(%[[VALUE_p_66]])), read<ptr<void>>(%[[VALUE_c_11]]), read<ptr<void>>(%[[VALUE_x_10]]));
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE66]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Add_rel:[0-9]+]] @Add_rel(%[[VALUE_p_67:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_45:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=release>(deref(read<ptr<volatile i32>>(%[[VALUE_p_67]])), add<i32>(old<i32>, read<i32>(%[[VALUE_v_45]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE67]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Add64_rel:[0-9]+]] @Add64_rel(%[[VALUE_p_68:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_46:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=release>(deref(read<ptr<volatile i64>>(%[[VALUE_p_68]])), add<i64>(old<i64>, read<i64>(%[[VALUE_v_46]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE68]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Cmpxchg128_rel:[0-9]+]] @Cmpxchg128_rel(%[[VALUE_d_2:[0-9]+]] d: ptr<volatile i64>, %[[VALUE_h_2:[0-9]+]] h: i64, %[[VALUE_l_2:[0-9]+]] l: i64, %[[VALUE_c_12:[0-9]+]] c: ptr<i64>) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: bool [synthetic] = compare_exchange<u128, volatile, form=write_back, weak=false, success=release, failure=relaxed>(deref(pointer_cast<ptr<volatile u128>>(read<ptr<volatile i64>>(%[[VALUE_d_2]]))), pointer_cast<ptr<u128>>(read<ptr<i64>>(%[[VALUE_c_12]])), or<u128>(shl<u128>(widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_h_2]]))), const<u128>(64)), widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_l_2]])))));
// DEFAULT-NEXT:         return from_bool<u8>(read<bool>(%[[VALUE69]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And8_nf:[0-9]+]] @And8_nf(%[[VALUE_p_69:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_47:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i8>>(%[[VALUE_p_69]])), and<i8>(old<i8>, read<i8>(%[[VALUE_v_47]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE70]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or8_nf:[0-9]+]] @Or8_nf(%[[VALUE_p_70:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_48:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i8>>(%[[VALUE_p_70]])), or<i8>(old<i8>, read<i8>(%[[VALUE_v_48]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE71]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor8_nf:[0-9]+]] @Xor8_nf(%[[VALUE_p_71:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_49:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i8>>(%[[VALUE_p_71]])), xor<i8>(old<i8>, read<i8>(%[[VALUE_v_49]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE72]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd8_nf:[0-9]+]] @ExchangeAdd8_nf(%[[VALUE_p_72:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_50:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i8>>(%[[VALUE_p_72]])), add<i8>(old<i8>, read<i8>(%[[VALUE_v_50]])));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE73]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange8_nf:[0-9]+]] @Exchange8_nf(%[[VALUE_p_73:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_v_51:[0-9]+]] v: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i8>>(%[[VALUE_p_73]])), read<i8>(%[[VALUE_v_51]]));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE74]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange8_nf:[0-9]+]] @CompareExchange8_nf(%[[VALUE_p_74:[0-9]+]] p: ptr<volatile i8>, %[[VALUE_x_11:[0-9]+]] x: i8, %[[VALUE_c_13:[0-9]+]] c: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: i8 [synthetic] = compare_exchange<i8, volatile, form=old, weak=false, success=relaxed, failure=relaxed>(deref(read<ptr<volatile i8>>(%[[VALUE_p_74]])), read<i8>(%[[VALUE_c_13]]), read<i8>(%[[VALUE_x_11]]));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE75]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And16_nf:[0-9]+]] @And16_nf(%[[VALUE_p_75:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_52:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i16>>(%[[VALUE_p_75]])), and<i16>(old<i16>, read<i16>(%[[VALUE_v_52]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE76]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or16_nf:[0-9]+]] @Or16_nf(%[[VALUE_p_76:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_53:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i16>>(%[[VALUE_p_76]])), or<i16>(old<i16>, read<i16>(%[[VALUE_v_53]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE77]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor16_nf:[0-9]+]] @Xor16_nf(%[[VALUE_p_77:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_54:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i16>>(%[[VALUE_p_77]])), xor<i16>(old<i16>, read<i16>(%[[VALUE_v_54]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE78]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd16_nf:[0-9]+]] @ExchangeAdd16_nf(%[[VALUE_p_78:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_55:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i16>>(%[[VALUE_p_78]])), add<i16>(old<i16>, read<i16>(%[[VALUE_v_55]])));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE79]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange16_nf:[0-9]+]] @Exchange16_nf(%[[VALUE_p_79:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_v_56:[0-9]+]] v: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: i16 [synthetic] = update<i16, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i16>>(%[[VALUE_p_79]])), read<i16>(%[[VALUE_v_56]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE80]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange16_nf:[0-9]+]] @CompareExchange16_nf(%[[VALUE_p_80:[0-9]+]] p: ptr<volatile i16>, %[[VALUE_x_12:[0-9]+]] x: i16, %[[VALUE_c_14:[0-9]+]] c: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: i16 [synthetic] = compare_exchange<i16, volatile, form=old, weak=false, success=relaxed, failure=relaxed>(deref(read<ptr<volatile i16>>(%[[VALUE_p_80]])), read<i16>(%[[VALUE_c_14]]), read<i16>(%[[VALUE_x_12]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE81]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Increment16_nf:[0-9]+]] @Increment16_nf(%[[VALUE_p_81:[0-9]+]] p: ptr<volatile i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=relaxed>(deref(read<ptr<volatile i16>>(%[[VALUE_p_81]])), add<i16>(old<i16>, const<i16>(1)));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE82]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Decrement16_nf:[0-9]+]] @Decrement16_nf(%[[VALUE_p_82:[0-9]+]] p: ptr<volatile i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: i16 [synthetic] = update<i16, result=new, volatile, atomic=relaxed>(deref(read<ptr<volatile i16>>(%[[VALUE_p_82]])), sub<i16>(old<i16>, const<i16>(1)));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE83]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And_nf:[0-9]+]] @And_nf(%[[VALUE_p_83:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_57:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_83]])), and<i32>(old<i32>, read<i32>(%[[VALUE_v_57]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE84]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or_nf:[0-9]+]] @Or_nf(%[[VALUE_p_84:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_58:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_84]])), or<i32>(old<i32>, read<i32>(%[[VALUE_v_58]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE85]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor_nf:[0-9]+]] @Xor_nf(%[[VALUE_p_85:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_59:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_85]])), xor<i32>(old<i32>, read<i32>(%[[VALUE_v_59]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE86]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd_nf:[0-9]+]] @ExchangeAdd_nf(%[[VALUE_p_86:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_60:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_86]])), add<i32>(old<i32>, read<i32>(%[[VALUE_v_60]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE87]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange_nf:[0-9]+]] @Exchange_nf(%[[VALUE_p_87:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_61:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: i32 [synthetic] = update<i32, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_87]])), read<i32>(%[[VALUE_v_61]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE88]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange_nf:[0-9]+]] @CompareExchange_nf(%[[VALUE_p_88:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_x_13:[0-9]+]] x: i32, %[[VALUE_c_15:[0-9]+]] c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: i32 [synthetic] = compare_exchange<i32, volatile, form=old, weak=false, success=relaxed, failure=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_88]])), read<i32>(%[[VALUE_c_15]]), read<i32>(%[[VALUE_x_13]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE89]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Increment_nf:[0-9]+]] @Increment_nf(%[[VALUE_p_89:[0-9]+]] p: ptr<volatile i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_89]])), add<i32>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE90]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Decrement_nf:[0-9]+]] @Decrement_nf(%[[VALUE_p_90:[0-9]+]] p: ptr<volatile i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE91:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_90]])), sub<i32>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE91]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_And64_nf:[0-9]+]] @And64_nf(%[[VALUE_p_91:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_62:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE92:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_91]])), and<i64>(old<i64>, read<i64>(%[[VALUE_v_62]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE92]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Or64_nf:[0-9]+]] @Or64_nf(%[[VALUE_p_92:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_63:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_92]])), or<i64>(old<i64>, read<i64>(%[[VALUE_v_63]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE93]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Xor64_nf:[0-9]+]] @Xor64_nf(%[[VALUE_p_93:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_64:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE94:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_93]])), xor<i64>(old<i64>, read<i64>(%[[VALUE_v_64]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE94]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangeAdd64_nf:[0-9]+]] @ExchangeAdd64_nf(%[[VALUE_p_94:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_65:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_94]])), add<i64>(old<i64>, read<i64>(%[[VALUE_v_65]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE95]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Exchange64_nf:[0-9]+]] @Exchange64_nf(%[[VALUE_p_95:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_66:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE96:[0-9]+]]: i64 [synthetic] = update<i64, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_95]])), read<i64>(%[[VALUE_v_66]]));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE96]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchange64_nf:[0-9]+]] @CompareExchange64_nf(%[[VALUE_p_96:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_x_14:[0-9]+]] x: i64, %[[VALUE_c_16:[0-9]+]] c: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: i64 [synthetic] = compare_exchange<i64, volatile, form=old, weak=false, success=relaxed, failure=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_96]])), read<i64>(%[[VALUE_c_16]]), read<i64>(%[[VALUE_x_14]]));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE97]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Increment64_nf:[0-9]+]] @Increment64_nf(%[[VALUE_p_97:[0-9]+]] p: ptr<volatile i64>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE98:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_97]])), add<i64>(old<i64>, const<i64>(1)));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE98]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Decrement64_nf:[0-9]+]] @Decrement64_nf(%[[VALUE_p_98:[0-9]+]] p: ptr<volatile i64>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_98]])), sub<i64>(old<i64>, const<i64>(1)));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE99]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ExchangePointer_nf:[0-9]+]] @ExchangePointer_nf(%[[VALUE_p_99:[0-9]+]] p: ptr<volatile ptr<void>>, %[[VALUE_v_67:[0-9]+]] v: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: ptr<void> [synthetic] = update<ptr<void>, result=old, volatile, atomic=relaxed>(deref(read<ptr<volatile ptr<void>>>(%[[VALUE_p_99]])), read<ptr<void>>(%[[VALUE_v_67]]));
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE100]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CompareExchangePointer_nf:[0-9]+]] @CompareExchangePointer_nf(%[[VALUE_p_100:[0-9]+]] p: ptr<volatile ptr<void>>, %[[VALUE_x_15:[0-9]+]] x: ptr<void>, %[[VALUE_c_17:[0-9]+]] c: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: ptr<void> [synthetic] = compare_exchange<ptr<void>, volatile, form=old, weak=false, success=relaxed, failure=relaxed>(deref(read<ptr<volatile ptr<void>>>(%[[VALUE_p_100]])), read<ptr<void>>(%[[VALUE_c_17]]), read<ptr<void>>(%[[VALUE_x_15]]));
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE101]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Add_nf:[0-9]+]] @Add_nf(%[[VALUE_p_101:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_68:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=relaxed>(deref(read<ptr<volatile i32>>(%[[VALUE_p_101]])), add<i32>(old<i32>, read<i32>(%[[VALUE_v_68]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE102]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Add64_nf:[0-9]+]] @Add64_nf(%[[VALUE_p_102:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_69:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=relaxed>(deref(read<ptr<volatile i64>>(%[[VALUE_p_102]])), add<i64>(old<i64>, read<i64>(%[[VALUE_v_69]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE103]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Cmpxchg128_nf:[0-9]+]] @Cmpxchg128_nf(%[[VALUE_d_3:[0-9]+]] d: ptr<volatile i64>, %[[VALUE_h_3:[0-9]+]] h: i64, %[[VALUE_l_3:[0-9]+]] l: i64, %[[VALUE_c_18:[0-9]+]] c: ptr<i64>) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: bool [synthetic] = compare_exchange<u128, volatile, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(pointer_cast<ptr<volatile u128>>(read<ptr<volatile i64>>(%[[VALUE_d_3]]))), pointer_cast<ptr<u128>>(read<ptr<i64>>(%[[VALUE_c_18]])), or<u128>(shl<u128>(widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_h_3]]))), const<u128>(64)), widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_l_3]])))));
// DEFAULT-NEXT:         return from_bool<u8>(read<bool>(%[[VALUE104]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Add:[0-9]+]] @Add(%[[VALUE_p_103:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_v_70:[0-9]+]] v: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: i32 [synthetic] = update<i32, result=new, volatile, atomic=seq_cst>(deref(read<ptr<volatile i32>>(%[[VALUE_p_103]])), add<i32>(old<i32>, read<i32>(%[[VALUE_v_70]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE105]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Add64:[0-9]+]] @Add64(%[[VALUE_p_104:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_v_71:[0-9]+]] v: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: i64 [synthetic] = update<i64, result=new, volatile, atomic=seq_cst>(deref(read<ptr<volatile i64>>(%[[VALUE_p_104]])), add<i64>(old<i64>, read<i64>(%[[VALUE_v_71]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE106]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Cmpxchg128:[0-9]+]] @Cmpxchg128(%[[VALUE_d_4:[0-9]+]] d: ptr<volatile i64>, %[[VALUE_h_4:[0-9]+]] h: i64, %[[VALUE_l_4:[0-9]+]] l: i64, %[[VALUE_c_19:[0-9]+]] c: ptr<i64>) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: bool [synthetic] = compare_exchange<u128, volatile, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(pointer_cast<ptr<volatile u128>>(read<ptr<volatile i64>>(%[[VALUE_d_4]]))), pointer_cast<ptr<u128>>(read<ptr<i64>>(%[[VALUE_c_19]])), or<u128>(shl<u128>(widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_h_4]]))), const<u128>(64)), widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_l_4]])))));
// DEFAULT-NEXT:         return from_bool<u8>(read<bool>(%[[VALUE107]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
