// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-macros

void report(const char *message);
int printf(const char *format, ...);

#define LIMIT 10
#define NOTHING
#define ASSERT(cond, msg) { if (!(cond)) report(msg); }
#define TRACE(x)
#define PRINT(x) printf x
#define TWICE(x) x + x
#define WRAP(x) (x)
#define CALL_WITH(f, x) f(x)
#define ID(x) x
#define OBJECT_TO_FUNCTION ID
#define LOG(format, ...) printf(format, __VA_ARGS__)
#define NO_ARGS() 1
#define OPEN if (value) {
#define CLOSE }

#if LIMIT > 5
int above = LIMIT;
#endif

int use(int value) {
    ASSERT(value < LIMIT, "too big");
    TRACE(("skipped %d", value));
    NOTHING;
    PRINT(("%d\n", value));
    value = TWICE(value);
    value = WRAP(value + 1) + 2;
    value = CALL_WITH(ID, value);
    value = OBJECT_TO_FUNCTION(value);
    LOG("%d %d\n", value, LIMIT);
    value = NO_ARGS();
    value = ID(ID(value));
    value = ID() value;
    OPEN value = 3; CLOSE
    return value;
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
// IR-NEXT:     global %[[VALUE_above:[0-9]+]] above: i32 [storage=static] = const<i32>(10) [macro=LIMIT#0 body] [linkage=external];
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([116, 111, 111, 32, 98, 105, 103, 0]) [macro=ASSERT#1 arg1] [linkage=internal];
// IR-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [macro=PRINT#5 arg0] [linkage=internal];
// IR-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [macro=LOG#10 arg0] [linkage=internal];
// IR-NEXT:     fn %[[VALUE_report:[0-9]+]] @report(%[[VALUE_message:[0-9]+]] message: ptr<const i8>) -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE_format:[0-9]+]] format: ptr<const i8>, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         macro_body #[[#NODE0:]] { [macro=ASSERT#1 body]
// IR-NEXT:             { [macro=ASSERT#1 body]
// IR-NEXT:                 if not<bool>(macro_arg<bool>(#[[#NODE0]], 0, lt<i32>(read<i32>(%[[VALUE_value]]) [macro=ASSERT#1 arg0], macro_value<i32>(#[[#NODE1:]], statements=0, const<i32>(10) [macro=ASSERT#1 arg0] [macro=LIMIT#2 body]) [macro=ASSERT#1 arg0] [macro=LIMIT#2 body]) [macro=ASSERT#1 arg0]) [macro=ASSERT#1 arg0]) [macro=ASSERT#1 body] [macro=ASSERT#1 body]
// IR-NEXT:                     call<void>(%[[VALUE_report]], pointer_cast<ptr<const i8>>(macro_arg<ptr<i8>>(#[[#NODE0]], 1, array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str]]) [macro=ASSERT#1 arg1]) [macro=ASSERT#1 arg1]) [macro=ASSERT#1 arg1]) [macro=ASSERT#1 body] [macro=ASSERT#1 body];
// IR-NEXT:             }
// IR-NEXT:         }
// IR-NEXT:         ;
// IR-NEXT:         ;
// IR-NEXT:         ;
// IR-NEXT:         call<i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]]) [macro=PRINT#5 arg0]) [macro=PRINT#5 arg0], read<i32>(%[[VALUE_value]]) [macro=PRINT#5 arg0]) [macro=PRINT#5 body];
// IR-NEXT:         write<i32>(%[[VALUE_value]], macro_value<i32>(#[[#NODE2:]], statements=0, add<i32>(macro_arg<i32>(#{{[0-9]+}}, 0, read<i32>(%[[VALUE_value]]) [macro=TWICE#6 arg0]) [macro=TWICE#6 arg0], macro_arg<i32>(#{{[0-9]+}}, 0, read<i32>(%[[VALUE_value]]) [macro=TWICE#6 arg0]) [macro=TWICE#6 arg0]) [macro=TWICE#6 body]) [macro=TWICE#6 body]);
// IR-NEXT:         write<i32>(%[[VALUE_value]], add<i32>(macro_value<i32>(#[[#NODE3:]], statements=0, macro_arg<i32>(#{{[0-9]+}}, 0, add<i32>(read<i32>(%[[VALUE_value]]) [macro=WRAP#7 arg0], const<i32>(1) [macro=WRAP#7 arg0]) [macro=WRAP#7 arg0]) [macro=WRAP#7 arg0]) [macro=WRAP#7 body], const<i32>(2)));
// IR-NEXT:         write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE_value]]) [macro=CALL_WITH#8 arg1]);
// IR-NEXT:         write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE_value]]));
// IR-NEXT:         call<i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_3]]) [macro=LOG#10 arg0]) [macro=LOG#10 arg0], read<i32>(%[[VALUE_value]]) [macro=LOG#10 arg1], macro_value<i32>(#[[#NODE4:]], statements=0, const<i32>(10) [macro=LOG#10 arg1] [macro=LIMIT#11 body]) [macro=LOG#10 arg1] [macro=LIMIT#11 body]) [macro=LOG#10 body];
// IR-NEXT:         write<i32>(%[[VALUE_value]], macro_value<i32>(#[[#NODE5:]], statements=0, const<i32>(1) [macro=NO_ARGS#12 body]) [macro=NO_ARGS#12 body]);
// IR-NEXT:         write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE_value]]) [macro=ID#13 arg0] [macro=ID#14 arg0]);
// IR-NEXT:         write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE_value]]));
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_value]]) [macro=OPEN#16 body], const<i32>(0) [macro=OPEN#16 body]) [macro=OPEN#16 body]
// IR-NEXT:             {
// IR-NEXT:                 write<i32>(%[[VALUE_value]], const<i32>(3));
// IR-NEXT:             }
// IR-NEXT:         return read<i32>(%[[VALUE_value]]);
// IR-NEXT:     }
// IR-NEXT:     macro_definition #[[#NODE6:]] LIMIT [defined=7:4] = "10";
// IR-NEXT:     macro_definition #[[#NODE0]] ASSERT(cond, msg) [defined=7:6] = "{ if ( ! ( cond ) ) report ( msg ) ; }";
// IR-NEXT:     macro_definition #[[#NODE1]] TRACE(x) [defined=7:7] = "";
// IR-NEXT:     macro_definition #[[#NODE7:]] NOTHING [defined=7:5] = "";
// IR-NEXT:     macro_definition #[[#NODE8:]] PRINT(x) [defined=7:8] = "printf x";
// IR-NEXT:     macro_definition #[[#NODE9:]] TWICE(x) [defined=7:9] = "x + x";
// IR-NEXT:     macro_definition #[[#NODE2]] WRAP(x) [defined=7:10] = "( x )";
// IR-NEXT:     macro_definition #[[#NODE3]] CALL_WITH(f, x) [defined=7:11] = "f ( x )";
// IR-NEXT:     macro_definition #[[#NODE10:]] OBJECT_TO_FUNCTION [defined=7:13] = "ID";
// IR-NEXT:     macro_definition #[[#NODE11:]] LOG(format) [variadic] [defined=7:14] = "printf ( format , __VA_ARGS__ )";
// IR-NEXT:     macro_definition #[[#NODE12:]] NO_ARGS() [defined=7:15] = "1";
// IR-NEXT:     macro_definition #[[#NODE4]] ID(x) [defined=7:12] = "x";
// IR-NEXT:     macro_definition #[[#NODE5]] OPEN [defined=7:16] = "if ( value ) {";
// IR-NEXT:     macro_definition #[[#NODE13:]] CLOSE [defined=7:17] = "}";
// IR-NEXT:     macro_expansion #[[#NODE6]] LIMIT [definition=#[[#NODE6]]] [site=7:465+5] [covers=expr];
// IR-NEXT:     macro_expansion #[[#NODE0]] ASSERT [definition=#[[#NODE0]]] [site=7:505+32] [arguments=512+13, 527+9] [covers=statements] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE1]] LIMIT [definition=#[[#NODE6]]] [site=7:520+5] [parent=#[[#NODE0]]] [covers=expr];
// IR-NEXT:     macro_expansion #[[#NODE7]] TRACE [definition=#[[#NODE1]]] [site=7:543+28] [arguments=549+21] [empty];
// IR-NEXT:     macro_expansion #[[#NODE8]] NOTHING [definition=#[[#NODE7]]] [site=7:577+7] [empty];
// IR-NEXT:     macro_expansion #[[#NODE9]] PRINT [definition=#[[#NODE8]]] [site=7:590+22] [arguments=596+15] [covers=expr] [arg0=call_arguments];
// IR-NEXT:     macro_expansion #[[#NODE2]] TWICE [definition=#[[#NODE9]]] [site=7:626+12] [arguments=632+5] [covers=expr] [arg0=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE3]] WRAP [definition=#[[#NODE2]]] [site=7:652+15] [arguments=657+9] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE10]] CALL_WITH [definition=#[[#NODE3]]] [site=7:685+20] [arguments=695+2, 699+5] [covers=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE11]] OBJECT_TO_FUNCTION [definition=#[[#NODE10]]] [site=7:719+18];
// IR-NEXT:     macro_expansion #[[#NODE12]] LOG [definition=#[[#NODE11]]] [site=7:750+28] [arguments=754+9, 765+5, 772+5] [covers=expr] [arg0=expr] [arg1=split];
// IR-NEXT:     macro_expansion #[[#NODE4]] LIMIT [definition=#[[#NODE6]]] [site=7:772+5] [parent=#[[#NODE12]]] [covers=expr];
// IR-NEXT:     macro_expansion #[[#NODE5]] NO_ARGS [definition=#[[#NODE12]]] [site=7:792+9] [covers=expr];
// IR-NEXT:     macro_expansion #[[#NODE13]] ID [definition=#[[#NODE4]]] [site=7:815+13] [arguments=818+9] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE14:]] ID [definition=#[[#NODE4]]] [site=7:818+9] [arguments=821+5] [parent=#[[#NODE13]]] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE15:]] ID [definition=#[[#NODE4]]] [site=7:842+4] [arguments=845+0] [empty];
// IR-NEXT:     macro_expansion #[[#NODE16:]] OPEN [definition=#[[#NODE5]]] [site=7:858+4] [covers=split];
// IR-NEXT:     macro_expansion #[[#NODE17:]] CLOSE [definition=#[[#NODE13]]] [site=7:874+5] [covers=split];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
