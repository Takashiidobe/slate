// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ISYSTEM tests/fixtures/inputs
#include "ms_asm_names.h"

struct inner { short a; int b; };
struct outer { int x; struct inner in; int arr[4]; };
typedef struct outer outer_t;
enum { SEVEN = 7 };
int matrix[2][3];
struct outer global_outer;
static int file_static;
int callee(int);

int names(int param, struct outer *pointer) {
  int local;
  int local_array[3];
  struct outer s;
  static int function_static;
  __asm {
    mov eax, param
    mov local, eax
    mov eax, local_array[4]
    mov eax, local_array[ebx * 4 + 4]
    mov eax, s.in.b
    mov eax, s.arr[8]
    mov eax, global_outer.in.b
    mov eax, file_static + 4
    mov eax, [function_static]
    mov eax, SEVEN[eax]
    mov eax, 2 * SEVEN + 3
    mov ecx, TYPE matrix
    mov ecx, LENGTH matrix
    mov ecx, SIZE matrix
    mov ecx, TYPE s.in
    mov eax, pointer
    mov al, [eax]outer_t.in
    mov eax, offset file_static
    push 1
    call callee
    add esp, 4
    mov eax, header_global
    mov eax, HEADER_ENUM
    call header_function
    jmp short later
  }
  __asm {
  later:
    mov local, eax
  }
  return local;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_HEADER_ENUM:[0-9]+]] HEADER_ENUM = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_inner:[0-9]+]] inner = struct {
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 in: @type[[TYPE_inner]];
// DEFAULT-NEXT:         field2 arr: array<i32, 4>;
// DEFAULT-NEXT:     } [size=28, align=4, offsets=[0, 4, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_outer_t:[0-9]+]] outer_t = @type[[TYPE_outer]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_HEADER_ENUM]] SEVEN = const<i32>(7);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_HEADER_ENUM]] header_global: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_matrix:[0-9]+]] matrix: array<array<i32, 3>, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_global_outer:[0-9]+]] global_outer: @type[[TYPE_outer]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_file_static:[0-9]+]] file_static: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_function_static:[0-9]+]] function_static: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_header_function:[0-9]+]] @header_function() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_callee:[0-9]+]] @callee(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_names:[0-9]+]] @names(%[[VALUE_param:[0-9]+]] param: i32, %[[VALUE_pointer:[0-9]+]] pointer: ptr<@type[[TYPE_outer]]>) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE1:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE1]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_local_array:[0-9]+]] local_array: array<i32, 3> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_outer]] [storage=automatic];
// DEFAULT-NEXT:         asm volatile "mov eax, param\nmov local, eax\nmov eax, local_array[4]\nmov eax, local_array[ebx * 4 + 4]\nmov eax, s.in.b\nmov eax, s.arr[8]\nmov eax, global_outer.in.b\nmov eax, file_static + 4\nmov eax, [function_static]\nmov eax, SEVEN[eax]\nmov eax, 2 * SEVEN + 3\nmov ecx, type matrix\nmov ecx, length matrix\nmov ecx, size matrix\nmov ecx, type s.in\nmov eax, pointer\nmov al, [eax] + outer_t.in\nmov eax, offset file_static\npush 1\ncall callee\nadd esp, 4\nmov eax, header_global\nmov eax, HEADER_ENUM\ncall header_function\njmp short later" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, " addr(%1) "\nmov " addr(%2) ", eax\nmov eax, " addr(%3 + 4) "\nmov eax, " addr(%3 + ebx*4 + 4) "\nmov eax, " addr(%4 + 8) "\nmov eax, " addr(%4 + 20) "\nmov eax, " addr(%5 + 8) "\nmov eax, " addr(%6 + 4) "\nmov eax, " addr(%7) "\nmov eax, [eax + 7]\nmov eax, 17\nmov ecx, 12\nmov ecx, 2\nmov ecx, 24\nmov ecx, 8\nmov eax, " addr(%8) "\nmov al, [eax + 4]\nmov eax, " %9 "\npush 1\ncall " %10 "\nadd esp, 4\nmov eax, " addr(%11) "\nmov eax, 3\ncall " %12 "\njmp short " label(later);
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE1]]);
// DEFAULT-NEXT:             in 1 [param] mem<read> place<i32>(%[[VALUE_param]]);
// DEFAULT-NEXT:             in 2 [local] mem<write> place<i32>(%[[VALUE_local]]);
// DEFAULT-NEXT:             in 3 [local_array] mem<read> place<array<i32, 3>>(%[[VALUE_local_array]]);
// DEFAULT-NEXT:             in 4 [s] mem<read> place<@type[[TYPE_outer]]>(%[[VALUE_s]]);
// DEFAULT-NEXT:             in 5 [global_outer] mem<read> place<@type[[TYPE_outer]]>(%[[VALUE_global_outer]]);
// DEFAULT-NEXT:             in 6 [file_static] mem<read> place<i32>(%[[VALUE_file_static]]);
// DEFAULT-NEXT:             in 7 [function_static] mem<read> place<i32>(%[[VALUE_function_static]]);
// DEFAULT-NEXT:             in 8 [pointer] mem<read> place<ptr<@type[[TYPE_outer]]>>(%[[VALUE_pointer]]);
// DEFAULT-NEXT:             in 9 [file_static] addr_of<ptr<i32>>(%[[VALUE_file_static]]);
// DEFAULT-NEXT:             in 10 sym<offset=0>(%[[VALUE_callee]]);
// DEFAULT-NEXT:             in 11 [header_global] mem<read> place<i32>(%[[VALUE_HEADER_ENUM]]);
// DEFAULT-NEXT:             in 12 sym<offset=0>(%[[VALUE_header_function]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "later:\nmov local, eax" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: label(later) ":\nmov " addr(%1) ", eax";
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE1]]);
// DEFAULT-NEXT:             in 1 [local] mem<write> place<i32>(%[[VALUE_local]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_local]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
