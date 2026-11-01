// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-macros

#define TIMES2(x) x * 2
#define ADD_ONE(x) ((x) + 1)
#define PLUS(a, b) a + b
#define SET(p, v) *(p) = (v)
#define SETS(a, b) a = 1; b = 2
#define BOTH(a, b) a = 1; b = 2;
#define WRAP(x) do { x = 1; } while (0)
#define CAST(T, x) ((T)(x))
#define LOCAL(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define CONST_LOCAL(T, a) do { const T t = a; (void)t; } while (0)
#define FIELD(p, name) ((p)->name)
#define CALL(f, x) f(x)
#define STATEMENT(s) s
#define TWICE(x) ((x) + (x))
#define OUTER(x) ADD_ONE(x)
#define COUNT 3
#define TRACE(x) callee x
#define SET_DOUBLE(p, x) { *(p) = x * 2; }
#define DECLARE(name) int name

struct pair {
  int first;
};

DECLARE(global);

int callee(int value);

int use(struct pair *pair, int i, int j) {
  int results = TIMES2(i);
  results += TIMES2(i + j);
  results += 3 * PLUS(i, j);
  results += ADD_ONE(i + j);
  SET(&results, i);
  SETS(i, j);
  BOTH(i, j);
  if (i)
    SETS(i, j);
  WRAP(results);
  results += CAST(long, i);
  results += CAST(char *, i) != 0;
  LOCAL(int, i, j);
  LOCAL(unsigned long, i, j);
  CONST_LOCAL(int, i);
  results += FIELD(pair, first);
  results += CALL(callee, i);
  STATEMENT(results = 1;);
  results += TWICE(i);
  results += OUTER(i + j);
  results += TRACE((i));
  SET_DOUBLE(&results, i);
  SET_DOUBLE(&results, i + j);
  return results + COUNT;
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
// IR-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// IR-NEXT:         field0 first: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     global %[[VALUE_global:[0-9]+]] global: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_callee:[0-9]+]] @callee(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_pair:[0-9]+]] pair: ptr<@type[[TYPE_pair]]>, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_j:[0-9]+]] j: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_results:[0-9]+]] results: i32 [storage=automatic] = macro_value<i32>(#[[#NODE0:]], statements=0, mul<i32>(macro_arg<i32>(#{{[0-9]+}}, 0, read<i32>(%[[VALUE_i]]) [macro=TIMES2#1 arg0]) [macro=TIMES2#1 arg0], const<i32>(2) [macro=TIMES2#1 body]) [macro=TIMES2#1 body]) [macro=TIMES2#1 body];
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE0]]), add<i32>(read<i32>(%[[VALUE_i]]) [macro=TIMES2#2 arg0], mul<i32>(read<i32>(%[[VALUE_j]]) [macro=TIMES2#2 arg0], const<i32>(2) [macro=TIMES2#2 body]) [macro=TIMES2#2 body]) [macro=TIMES2#2 body]);
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE2]]), add<i32>(mul<i32>(const<i32>(3), read<i32>(%[[VALUE_i]]) [macro=PLUS#3 arg0]), read<i32>(%[[VALUE_j]]) [macro=PLUS#3 arg1]));
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE3]]));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE4]]), macro_value<i32>(#[[#NODE1:]], statements=0, add<i32>(macro_arg<i32>(#{{[0-9]+}}, 0, add<i32>(read<i32>(%[[VALUE_i]]) [macro=ADD_ONE#4 arg0], read<i32>(%[[VALUE_j]]) [macro=ADD_ONE#4 arg0]) [macro=ADD_ONE#4 arg0]) [macro=ADD_ONE#4 arg0], const<i32>(1) [macro=ADD_ONE#4 body]) [macro=ADD_ONE#4 body]) [macro=ADD_ONE#4 body]);
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE5]]));
// IR-NEXT:         macro_body #[[#NODE2:]] {
// IR-NEXT:             write<i32>(deref(macro_arg<ptr<i32>>(#[[#NODE2]], 0, addr_of<ptr<i32>>(%[[VALUE_results]]))), macro_arg<i32>(#[[#NODE2]], 1, read<i32>(%[[VALUE_i]]) [macro=SET#5 arg1]) [macro=SET#5 arg1]) [macro=SET#5 body];
// IR-NEXT:         }
// IR-NEXT:         macro_body #[[#NODE3:]] { [macro=SETS#6 body]
// IR-NEXT:             write<i32>(macro_arg(#[[#NODE3]], 0, %[[VALUE_i]]), const<i32>(1) [macro=SETS#6 body]) [macro=SETS#6 body];
// IR-NEXT:             write<i32>(macro_arg(#[[#NODE3]], 1, %[[VALUE_j]]), const<i32>(2) [macro=SETS#6 body]) [macro=SETS#6 body];
// IR-NEXT:         }
// IR-NEXT:         macro_body #[[#NODE4:]] { [macro=BOTH#7 body]
// IR-NEXT:             write<i32>(macro_arg(#[[#NODE4]], 0, %[[VALUE_i]]), const<i32>(1) [macro=BOTH#7 body]) [macro=BOTH#7 body];
// IR-NEXT:             write<i32>(macro_arg(#[[#NODE4]], 1, %[[VALUE_j]]), const<i32>(2) [macro=BOTH#7 body]) [macro=BOTH#7 body];
// IR-NEXT:         }
// IR-NEXT:         ;
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// IR-NEXT:             write<i32>(%[[VALUE_i]], const<i32>(1) [macro=SETS#8 body]) [macro=SETS#8 body];
// IR-NEXT:         write<i32>(%[[VALUE_j]], const<i32>(2) [macro=SETS#8 body]) [macro=SETS#8 body];
// IR-NEXT:         macro_body #[[#NODE5:]] {
// IR-NEXT:             do %[[VALUE6:[0-9]+]]
// IR-NEXT:                 { [macro=WRAP#9 body]
// IR-NEXT:                     write<i32>(macro_arg(#[[#NODE5]], 0, %[[VALUE_results]]), const<i32>(1) [macro=WRAP#9 body]) [macro=WRAP#9 body];
// IR-NEXT:                 }
// IR-NEXT:             while ne<i32>(const<i32>(0) [macro=WRAP#9 body], const<i32>(0) [macro=WRAP#9 body]) [macro=WRAP#9 body];
// IR-NEXT:         }
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = truncate<i32>(add<i64>(widen<i64>(read<i32>(%[[VALUE7]])), widen<i64>(read<i32>(%[[VALUE_i]]) [macro=CAST#10 arg1]) [macro=CAST#10 body]));
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE8]]));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE9]]), from_bool<i32>(ne<ptr<i8>>(int_to_ptr<ptr<i8>>(read<i32>(%[[VALUE_i]]) [macro=CAST#11 arg1]) [macro=CAST#11 body], null<ptr<i8>>)));
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE10]]));
// IR-NEXT:         do %[[VALUE11:[0-9]+]]
// IR-NEXT:             { [macro=LOCAL#12 body]
// IR-NEXT:                 let %[[VALUE_t:[0-9]+]] t: i32 [storage=automatic] = read<i32>(%[[VALUE_i]]) [macro=LOCAL#12 arg1] [macro=LOCAL#12 body];
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE_j]]) [macro=LOCAL#12 arg2]) [macro=LOCAL#12 body];
// IR-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE_t]]) [macro=LOCAL#12 body]) [macro=LOCAL#12 body];
// IR-NEXT:             }
// IR-NEXT:         while ne<i32>(const<i32>(0) [macro=LOCAL#12 body], const<i32>(0) [macro=LOCAL#12 body]) [macro=LOCAL#12 body];
// IR-NEXT:         do %[[VALUE12:[0-9]+]]
// IR-NEXT:             { [macro=LOCAL#13 body]
// IR-NEXT:                 let %[[VALUE_t_2:[0-9]+]] t: u64 [storage=automatic] = reinterpret<u64>(widen<i64>(read<i32>(%[[VALUE_i]]) [macro=LOCAL#13 arg1]) [macro=LOCAL#13 arg1]) [macro=LOCAL#13 arg1] [macro=LOCAL#13 body];
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE_j]]) [macro=LOCAL#13 arg2]) [macro=LOCAL#13 body];
// IR-NEXT:                 write<i32>(%[[VALUE_j]], reinterpret<i32>(truncate<u32>(read<u64>(%[[VALUE_t_2]]) [macro=LOCAL#13 body]) [macro=LOCAL#13 body]) [macro=LOCAL#13 body]) [macro=LOCAL#13 body];
// IR-NEXT:             }
// IR-NEXT:         while ne<i32>(const<i32>(0) [macro=LOCAL#13 body], const<i32>(0) [macro=LOCAL#13 body]) [macro=LOCAL#13 body];
// IR-NEXT:         do %[[VALUE13:[0-9]+]]
// IR-NEXT:             { [macro=CONST_LOCAL#14 body]
// IR-NEXT:                 let %[[VALUE_t_3:[0-9]+]] t: i32 [storage=automatic] [const] = read<i32>(%[[VALUE_i]]) [macro=CONST_LOCAL#14 arg1] [macro=CONST_LOCAL#14 body];
// IR-NEXT:                 read<i32>(%[[VALUE_t_3]]) [macro=CONST_LOCAL#14 body] [macro=CONST_LOCAL#14 body];
// IR-NEXT:             }
// IR-NEXT:         while ne<i32>(const<i32>(0) [macro=CONST_LOCAL#14 body], const<i32>(0) [macro=CONST_LOCAL#14 body]) [macro=CONST_LOCAL#14 body];
// IR-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE14]]), read<i32>(field0(deref(read<ptr<@type[[TYPE_pair]]>>(%[[VALUE_pair]])))) [macro=FIELD#15 body]);
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE15]]));
// IR-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = call<i32>(%[[VALUE_callee]], read<i32>(%[[VALUE_i]]) [macro=CALL#16 arg1]) [macro=CALL#16 body];
// IR-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE17]]), read<i32>(%[[VALUE16]]));
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE18]]));
// IR-NEXT:         write<i32>(%[[VALUE_results]], const<i32>(1) [macro=STATEMENT#17 arg0]) [macro=STATEMENT#17 arg0];
// IR-NEXT:         ;
// IR-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE19]]), macro_value<i32>(#[[#NODE6:]], statements=0, add<i32>(macro_arg<i32>(#{{[0-9]+}}, 0, read<i32>(%[[VALUE_i]]) [macro=TWICE#18 arg0]) [macro=TWICE#18 arg0], macro_arg<i32>(#{{[0-9]+}}, 0, read<i32>(%[[VALUE_i]]) [macro=TWICE#18 arg0]) [macro=TWICE#18 arg0]) [macro=TWICE#18 body]) [macro=TWICE#18 body]);
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE20]]));
// IR-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE21]]), macro_value<i32>(#[[#NODE7:]], statements=0, macro_value<i32>(#[[#NODE8:]], statements=0, add<i32>(macro_arg<i32>(#{{[0-9]+}}, 0, macro_arg<i32>(#{{[0-9]+}}, 0, add<i32>(read<i32>(%[[VALUE_i]]) [macro=ADD_ONE#20 arg0] [macro=OUTER#19 arg0], read<i32>(%[[VALUE_j]]) [macro=ADD_ONE#20 arg0] [macro=OUTER#19 arg0]) [macro=ADD_ONE#20 arg0] [macro=OUTER#19 arg0]) [macro=ADD_ONE#20 arg0] [macro=OUTER#19 arg0]) [macro=ADD_ONE#20 arg0] [macro=OUTER#19 arg0], const<i32>(1) [macro=ADD_ONE#20 body] [macro=OUTER#19 body]) [macro=ADD_ONE#20 body] [macro=OUTER#19 body]) [macro=ADD_ONE#20 body] [macro=OUTER#19 body]) [macro=ADD_ONE#20 body] [macro=OUTER#19 body]);
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE22]]));
// IR-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = call<i32>(%[[VALUE_callee]], read<i32>(%[[VALUE_i]]) [macro=TRACE#21 arg0]) [macro=TRACE#21 body];
// IR-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_results]]);
// IR-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE24]]), read<i32>(%[[VALUE23]]));
// IR-NEXT:         write<i32>(%[[VALUE_results]], read<i32>(%[[VALUE25]]));
// IR-NEXT:         macro_body #[[#NODE9:]] { [macro=SET_DOUBLE#22 body]
// IR-NEXT:             { [macro=SET_DOUBLE#22 body]
// IR-NEXT:                 write<i32>(deref(macro_arg<ptr<i32>>(#[[#NODE9]], 0, addr_of<ptr<i32>>(%[[VALUE_results]]))), mul<i32>(macro_arg<i32>(#[[#NODE9]], 1, read<i32>(%[[VALUE_i]]) [macro=SET_DOUBLE#22 arg1]) [macro=SET_DOUBLE#22 arg1], const<i32>(2) [macro=SET_DOUBLE#22 body]) [macro=SET_DOUBLE#22 body]) [macro=SET_DOUBLE#22 body];
// IR-NEXT:             }
// IR-NEXT:         }
// IR-NEXT:         ;
// IR-NEXT:         { [macro=SET_DOUBLE#23 body]
// IR-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%[[VALUE_results]])), add<i32>(read<i32>(%[[VALUE_i]]) [macro=SET_DOUBLE#23 arg1], mul<i32>(read<i32>(%[[VALUE_j]]) [macro=SET_DOUBLE#23 arg1], const<i32>(2) [macro=SET_DOUBLE#23 body]) [macro=SET_DOUBLE#23 body]) [macro=SET_DOUBLE#23 body]) [macro=SET_DOUBLE#23 body];
// IR-NEXT:         }
// IR-NEXT:         ;
// IR-NEXT:         return add<i32>(read<i32>(%[[VALUE_results]]), macro_value<i32>(#[[#NODE10:]], statements=0, const<i32>(3) [macro=COUNT#24 body]) [macro=COUNT#24 body]);
// IR-NEXT:     }
// IR-NEXT:     macro_definition #[[#NODE11:]] DECLARE(name) [defined=7:19] = "int name";
// IR-NEXT:     macro_definition #[[#NODE0]] TIMES2(x) [defined=7:1] = "x * 2";
// IR-NEXT:     macro_definition #[[#NODE12:]] PLUS(a, b) [defined=7:3] = "a + b";
// IR-NEXT:     macro_definition #[[#NODE13:]] ADD_ONE(x) [defined=7:2] = "( ( x ) + 1 )";
// IR-NEXT:     macro_definition #[[#NODE1]] SET(p, v) [defined=7:4] = "* ( p ) = ( v )";
// IR-NEXT:     macro_definition #[[#NODE2]] SETS(a, b) [defined=7:5] = "a = 1 ; b = 2";
// IR-NEXT:     macro_definition #[[#NODE3]] BOTH(a, b) [defined=7:6] = "a = 1 ; b = 2 ;";
// IR-NEXT:     macro_definition #[[#NODE4]] WRAP(x) [defined=7:7] = "do { x = 1 ; } while ( 0 )";
// IR-NEXT:     macro_definition #[[#NODE14:]] CAST(T, x) [defined=7:8] = "( ( T ) ( x ) )";
// IR-NEXT:     macro_definition #[[#NODE5]] LOCAL(T, a, b) [defined=7:9] = "do { T t = a ; a = b ; b = t ; } while ( 0 )";
// IR-NEXT:     macro_definition #[[#NODE15:]] CONST_LOCAL(T, a) [defined=7:10] = "do { const T t = a ; ( void ) t ; } while ( 0 )";
// IR-NEXT:     macro_definition #[[#NODE16:]] FIELD(p, name) [defined=7:11] = "( ( p ) -> name )";
// IR-NEXT:     macro_definition #[[#NODE17:]] CALL(f, x) [defined=7:12] = "f ( x )";
// IR-NEXT:     macro_definition #[[#NODE18:]] STATEMENT(s) [defined=7:13] = "s";
// IR-NEXT:     macro_definition #[[#NODE19:]] TWICE(x) [defined=7:14] = "( ( x ) + ( x ) )";
// IR-NEXT:     macro_definition #[[#NODE20:]] OUTER(x) [defined=7:15] = "ADD_ONE ( x )";
// IR-NEXT:     macro_definition #[[#NODE21:]] TRACE(x) [defined=7:17] = "callee x";
// IR-NEXT:     macro_definition #[[#NODE22:]] SET_DOUBLE(p, x) [defined=7:18] = "{ * ( p ) = x * 2 ; }";
// IR-NEXT:     macro_definition #[[#NODE6]] COUNT [defined=7:16] = "3";
// IR-NEXT:     macro_expansion #[[#NODE11]] DECLARE [definition=#[[#NODE11]]] [site=7:658+15] [arguments=666+6] [covers=decl] [arg0=name];
// IR-NEXT:     macro_expansion #[[#NODE0]] TIMES2 [definition=#[[#NODE0]]] [site=7:759+9] [arguments=766+1] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE12]] TIMES2 [definition=#[[#NODE0]]] [site=7:783+13] [arguments=790+5] [covers=expr] [arg0=split];
// IR-NEXT:     macro_expansion #[[#NODE13]] PLUS [definition=#[[#NODE12]]] [site=7:815+10] [arguments=820+1, 823+1] [covers=split] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE1]] ADD_ONE [definition=#[[#NODE13]]] [site=7:840+14] [arguments=848+5] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE2]] SET [definition=#[[#NODE1]]] [site=7:858+16] [arguments=862+8, 872+1] [covers=expr] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE3]] SETS [definition=#[[#NODE2]]] [site=7:878+10] [arguments=883+1, 886+1] [covers=statements] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE4]] BOTH [definition=#[[#NODE3]]] [site=7:892+10] [arguments=897+1, 900+1] [covers=statements] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE14]] SETS [definition=#[[#NODE2]]] [site=7:917+10] [arguments=922+1, 925+1] [covers=split] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE5]] WRAP [definition=#[[#NODE4]]] [site=7:931+13] [arguments=936+7] [covers=statements] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE15]] CAST [definition=#[[#NODE14]]] [site=7:959+13] [arguments=964+4, 970+1] [covers=expr] [arg0=type] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE16]] CAST [definition=#[[#NODE14]]] [site=7:987+15] [arguments=992+6, 1000+1] [covers=expr] [arg0=type] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE17]] LOCAL [definition=#[[#NODE5]]] [site=7:1011+16] [arguments=1017+3, 1022+1, 1025+1] [covers=statements] [arg0=type] [arg1=expr] [arg1=expr] [arg2=expr] [arg2=expr];
// IR-NEXT:     macro_expansion #[[#NODE18]] LOCAL [definition=#[[#NODE5]]] [site=7:1031+26] [arguments=1037+13, 1052+1, 1055+1] [covers=statements] [arg0=type] [arg1=expr] [arg1=expr] [arg2=expr] [arg2=expr];
// IR-NEXT:     macro_expansion #[[#NODE19]] CONST_LOCAL [definition=#[[#NODE15]]] [site=7:1061+19] [arguments=1073+3, 1078+1] [covers=statements] [arg0=type] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE20]] FIELD [definition=#[[#NODE16]]] [site=7:1095+18] [arguments=1101+4, 1107+5] [covers=expr] [arg0=expr] [arg1=name];
// IR-NEXT:     macro_expansion #[[#NODE21]] CALL [definition=#[[#NODE17]]] [site=7:1128+15] [arguments=1133+6, 1141+1] [covers=expr] [arg0=callee] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE22]] STATEMENT [definition=#[[#NODE18]]] [site=7:1147+23] [arguments=1157+12] [covers=statements] [arg0=statements];
// IR-NEXT:     macro_expansion #[[#NODE6]] TWICE [definition=#[[#NODE19]]] [site=7:1185+8] [arguments=1191+1] [covers=expr] [arg0=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE7]] OUTER [definition=#[[#NODE20]]] [site=7:1208+12] [arguments=1214+5] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE8]] ADD_ONE [definition=#[[#NODE13]]] [parent=#[[#NODE7]]] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE23:]] TRACE [definition=#[[#NODE21]]] [site=7:1235+10] [arguments=1241+3] [covers=expr] [arg0=call_arguments];
// IR-NEXT:     macro_expansion #[[#NODE9]] SET_DOUBLE [definition=#[[#NODE22]]] [site=7:1249+23] [arguments=1260+8, 1270+1] [covers=statements] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE24:]] SET_DOUBLE [definition=#[[#NODE22]]] [site=7:1276+27] [arguments=1287+8, 1297+5] [covers=statements] [arg0=expr] [arg1=split];
// IR-NEXT:     macro_expansion #[[#NODE10]] COUNT [definition=#[[#NODE6]]] [site=7:1324+5] [covers=expr];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
