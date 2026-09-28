/* { dg-do run } */
/* { dg-options { -O2 } } */
extern void abort (void);
extern void exit (int);
typedef struct {
        int             x;
        int             y;
}               point_t;

int main(int argc, char *argv[]);
int printPoints(point_t a, point_t b);
point_t toPoint(int x1, int y1);

int
main(int argc, char *argv[])
{

        if (printPoints(toPoint(0, 0), toPoint(1000, 1000)) != 1)
                abort();
        else
                exit(0);

        return 0;
}

int
printPoints(point_t a, point_t b)
{
        if (a.x != 0
            || a.y != 0
            || b.x != 1000
            || b.y != 1000)
                return 0;
        else
                return 1;
}

point_t
toPoint(int x1, int y1)
{
        point_t         p;

        p.x = x1;
        p.y = y1;

        return p;
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 point_t = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%20 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main(%13 argc: i32, %14 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(@type0, @type0) -> i32, abi=sysv64(native_c, native_c) -> scalar>(%9, copy<@type0, reason=arg>(call<@type0, signature=fn(i32, i32) -> @type0, abi=sysv64(scalar, scalar) -> native_c>(%12, const<i32>(0), const<i32>(0))), copy<@type0, reason=arg>(call<@type0, signature=fn(i32, i32) -> @type0, abi=sysv64(scalar, scalar) -> native_c>(%12, const<i32>(1000), const<i32>(1000)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @printPoints(%15 a: @type0, %16 b: @type0) -> i32 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%15)), const<i32>(0)), ne<i32>(read<i32>(field1(%15)), const<i32>(0))), ne<i32>(read<i32>(field0(%16)), const<i32>(1000))), ne<i32>(read<i32>(field1(%16)), const<i32>(1000)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @toPoint(%17 x1: i32, %18 y1: i32) -> @type0 [linkage=external] [abi=sysv64(scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 p: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%19), read<i32>(%17));
// DEFAULT-NEXT:         write<i32>(field1(%19), read<i32>(%18));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
