// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int f(int);
int getc(void *);
struct Pair { int n; };
void side_effects(struct Pair *p, int *a, int x, int y, int i, int c) {
    a[i++] = x;
    while ((c = getc(p)) != -1) f(c);
    x = y = 0;
    f(i++);
    if (p && p->n++) x++;
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
// IR-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_getc:[0-9]+]] @getc(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_side_effects:[0-9]+]] @side_effects(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Pair]]>, %[[VALUE_a:[0-9]+]] a: ptr<i32>, %[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_c:[0-9]+]] c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), read<i32>(%[[VALUE2]]))), read<i32>(%[[VALUE_x]]));
// IR-NEXT:         while %[[VALUE4:[0-9]+]] {
// IR-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_getc]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_p]])));
// IR-NEXT:             write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE5]]));
// IR-NEXT:             yield ne<i32>(read<i32>(%[[VALUE5]]), neg<i32, overflow=ub>(const<i32>(1)));
// IR-NEXT:         }
// IR-NEXT:             call<i32, signature=fn(i32) -> i32>(%[[VALUE_f]], read<i32>(%[[VALUE_c]]));
// IR-NEXT:         write<i32>(%[[VALUE_y]], const<i32>(0));
// IR-NEXT:         write<i32>(%[[VALUE_x]], const<i32>(0));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE7]]));
// IR-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_f]], read<i32>(%[[VALUE6]]));
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// IR-NEXT:         if ne<ptr<@type[[TYPE_Pair]]>>(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_p]]), null<ptr<@type[[TYPE_Pair]]>>)
// IR-NEXT:             let %[[VALUE9:[0-9]+]]: ptr<@type[[TYPE_Pair]]> [synthetic] = read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_p]]);
// IR-NEXT:             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE9]]))));
// IR-NEXT:             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// IR-NEXT:             write<i32>(field0(deref(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE9]]))), read<i32>(%[[VALUE11]]));
// IR-NEXT:             write<bool>(%[[VALUE8]], ne<i32>(read<i32>(%[[VALUE10]]), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%[[VALUE8]], const<bool>(false));
// IR-NEXT:         if read<bool>(%[[VALUE8]])
// IR-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// IR-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE13]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
