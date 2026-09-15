// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-EXAMPLE ir_module

// SLATE-FILECHECK-BEGIN IR
// IR: without source metadata:
// IR-NEXT: module {
// IR-NEXT:     target [char_signed=true, short_width=16, int_width=32, long_width=64, long_long_width=64, pointer_width=64, wchar_signed=true, wchar_width=32, long_double=X87];
// IR-NEXT:     type @type0 word = i32;
// IR-NEXT:     type @type1 Node = struct {
// IR-NEXT:         field0 value: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type2 = ptr<@type1>;
// IR-NEXT:     type @type3 Choice = enum : i32 {
// IR-NEXT:         %3 FIRST = const<i32>(0);
// IR-NEXT:         %4 SECOND = const<i32>(1);
// IR-NEXT:     };
// IR-NEXT:     type @type4 Payload = union {
// IR-NEXT:         field0 number: i32;
// IR-NEXT:         field1 alias: @type0;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type5 Opaque = struct incomplete;
// IR-NEXT:     type @type6 = array<i32, 3>;
// IR-NEXT:     type @type7 = array<i32, incomplete>;
// IR-NEXT:     type @type8 FutureChoice = enum incomplete;
// IR-NEXT:     type @type9 = ptr<const i32>;
// IR-NEXT:     type @type10 = array<i8, 4>;
// IR-NEXT:     type @type11 = ptr<const i8>;
// IR-NEXT:     global %5 Node: i32 [storage=static] = const<i32>(0) [linkage=external];
// IR-NEXT:     extern %6 thread_value: i32 [storage=thread] [linkage=external];
// IR-NEXT:     global %14 format: @type10 [storage=static] = bytes<@type10>([37, 100, 10, 0]) [linkage=internal];
// IR-NEXT:     fn %10 @add(%0 a: i32, %1 b: i32) -> i32 [linkage=external] {
// IR-NEXT:         let %2 c: i32 [storage=automatic] = add<i32, overflow=undefined>(read<i32>(%0), read<i32>(%1));
// IR-NEXT:         return read<i32>(%2);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @log_values(%7 <unnamed>: i32, ...) -> void [linkage=external];
// IR-NEXT:     fn %12 @legacy(unprototyped) -> i32 [linkage=external];
// IR-NEXT:     fn %13 @update() -> void [linkage=internal] {
// IR-NEXT:         {
// IR-NEXT:             let %8 node: @type2 [storage=automatic] = null<@type2>;
// IR-NEXT:             write<i32>(%5, const<i32>(1));
// IR-NEXT:             addr_of<@type9>(%5);
// IR-NEXT:         }
// IR-NEXT:         return;
// IR-NEXT:     }
// IR-NEXT:     fn %15 @printf(%16 format: @type11, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %17 @main() -> i32 [linkage=external] {
// IR-NEXT:         call<i32>(%15, array_decay<@type11>(%14), call<i32>(%10, const<i32>(2), const<i32>(3)));
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT: }
// IR-NEXT: with source metadata:
// IR-NEXT: module {
// IR-NEXT:     target [char_signed=true, short_width=16, int_width=32, long_width=64, long_long_width=64, pointer_width=64, wchar_signed=true, wchar_width=32, long_double=X87];
// IR-NEXT:     type @type0 word = i32 [c="typedef int word"];
// IR-NEXT:     type @type1 Node = struct {
// IR-NEXT:         field0 value: i32 [c="int"];
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type2 = ptr<@type1>;
// IR-NEXT:     type @type3 Choice = enum : i32 {
// IR-NEXT:         %3 FIRST = const<i32>(0);
// IR-NEXT:         %4 SECOND = const<i32>(1);
// IR-NEXT:     };
// IR-NEXT:     type @type4 Payload = union {
// IR-NEXT:         field0 number: i32;
// IR-NEXT:         field1 alias: @type0;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type5 Opaque = struct incomplete;
// IR-NEXT:     type @type6 = array<i32, 3>;
// IR-NEXT:     type @type7 = array<i32, incomplete>;
// IR-NEXT:     type @type8 FutureChoice = enum incomplete;
// IR-NEXT:     type @type9 = ptr<const i32>;
// IR-NEXT:     type @type10 = array<i8, 4>;
// IR-NEXT:     type @type11 = ptr<const i8>;
// IR-NEXT:     global %5 Node: i32 [storage=static] = const<i32>(0) [linkage=external] [c="int Node"];
// IR-NEXT:     extern %6 thread_value: i32 [storage=thread] [linkage=external];
// IR-NEXT:     global %14 format: @type10 [storage=static] = bytes<@type10>([37, 100, 10, 0]) [linkage=internal];
// IR-NEXT:     fn %10 @add(%0 a: i32 [c="int"], %1 b: i32) -> i32 [linkage=external] [source="add.c"] {
// IR-NEXT:         let %2 c: i32 [storage=automatic] = add<i32, overflow=undefined>(read<i32>(%0) [c="a"], read<i32>(%1)) [source="a + b"] [c="int c"];
// IR-NEXT:         return read<i32>(%2);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @log_values(%7 <unnamed>: i32, ...) -> void [linkage=external];
// IR-NEXT:     fn %12 @legacy(unprototyped) -> i32 [linkage=external];
// IR-NEXT:     fn %13 @update() -> void [linkage=internal] {
// IR-NEXT:         { [source="nested block"]
// IR-NEXT:             let %8 node: @type2 [storage=automatic] = null<@type2>;
// IR-NEXT:             write<i32>(%5, const<i32>(1));
// IR-NEXT:             addr_of<@type9>(%5);
// IR-NEXT:         }
// IR-NEXT:         return;
// IR-NEXT:     }
// IR-NEXT:     fn %15 @printf(%16 format: @type11, ...) -> i32 [linkage=external] [origin="system:stdio.h"];
// IR-NEXT:     fn %17 @main() -> i32 [linkage=external] {
// IR-NEXT:         call<i32>(%15, array_decay<@type11>(%14) [c="char[4]"], call<i32>(%10, const<i32>(2), const<i32>(3)) [vararg_promotion="none"]);
// IR-NEXT:         return const<i32>(0) [implicit="main_return"];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
