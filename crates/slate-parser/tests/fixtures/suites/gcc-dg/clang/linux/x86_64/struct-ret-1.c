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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_point_t:[0-9]+]] point_t = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(@type[[TYPE0]], @type[[TYPE0]]) -> i32, abi=sysv64(native_c, native_c) -> scalar>(%[[VALUE_printPoints:[0-9]+]], copy<@type[[TYPE0]], reason=arg>(call<@type[[TYPE0]], signature=fn(i32, i32) -> @type[[TYPE0]], abi=sysv64(scalar, scalar) -> native_c>(%[[VALUE_toPoint:[0-9]+]], const<i32>(0), const<i32>(0))), copy<@type[[TYPE0]], reason=arg>(call<@type[[TYPE0]], signature=fn(i32, i32) -> @type[[TYPE0]], abi=sysv64(scalar, scalar) -> native_c>(%[[VALUE_toPoint]], const<i32>(1000), const<i32>(1000)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_printPoints]] @printPoints(%[[VALUE_a:[0-9]+]] a: @type[[TYPE0]], %[[VALUE_b:[0-9]+]] b: @type[[TYPE0]]) -> i32 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_a]])), const<i32>(0)), ne<i32>(read<i32>(field1(%[[VALUE_a]])), const<i32>(0))), ne<i32>(read<i32>(field0(%[[VALUE_b]])), const<i32>(1000))), ne<i32>(read<i32>(field1(%[[VALUE_b]])), const<i32>(1000)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_toPoint]] @toPoint(%[[VALUE_x1:[0-9]+]] x1: i32, %[[VALUE_y1:[0-9]+]] y1: i32) -> @type[[TYPE0]] [linkage=external] [abi=sysv64(scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_p]]), read<i32>(%[[VALUE_x1]]));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_p]]), read<i32>(%[[VALUE_y1]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_p]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
