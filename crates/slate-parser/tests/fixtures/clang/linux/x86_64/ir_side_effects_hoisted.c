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
// IR-NEXT:     type @type0 Pair = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     fn %0 @f(%10 <unnamed>: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %1 @getc(%11 <unnamed>: ptr<void>) -> i32 [linkage=external];
// IR-NEXT:     fn %3 @side_effects(%4 p: ptr<@type0>, %5 a: ptr<i32>, %6 x: i32, %7 y: i32, %8 i: i32, %9 c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %13: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// IR-NEXT:         write<i32>(%8, read<i32>(%14));
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), read<i32>(%13))), read<i32>(%6));
// IR-NEXT:         while %12 {
// IR-NEXT:             write<i32>(%9, call<i32, signature=fn(ptr<void>) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type0>>(%4))));
// IR-NEXT:             yield ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type0>>(%4))), neg<i32, overflow=ub>(const<i32>(1)));
// IR-NEXT:         }
// IR-NEXT:             call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%9));
// IR-NEXT:         write<i32>(%7, const<i32>(0));
// IR-NEXT:         write<i32>(%6, const<i32>(0));
// IR-NEXT:         let %15: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// IR-NEXT:         write<i32>(%8, read<i32>(%16));
// IR-NEXT:         call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%15));
// IR-NEXT:         let %17: bool [synthetic];
// IR-NEXT:         if ne<ptr<@type0>>(read<ptr<@type0>>(%4), null<ptr<@type0>>)
// IR-NEXT:             let %18: ptr<@type0> [synthetic] = read<ptr<@type0>>(%4);
// IR-NEXT:             let %19: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type0>>(%18))));
// IR-NEXT:             let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// IR-NEXT:             write<i32>(field0(deref(read<ptr<@type0>>(%18))), read<i32>(%20));
// IR-NEXT:             write<bool>(%17, ne<i32>(read<i32>(%19), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%17, const<bool>(false));
// IR-NEXT:         if read<bool>(%17)
// IR-NEXT:             let %21: i32 [synthetic] = read<i32>(%6);
// IR-NEXT:             let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// IR-NEXT:             write<i32>(%6, read<i32>(%22));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
