// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ISYSTEM tests/fixtures
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 HEADER_ENUM = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 inner = struct {
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 outer = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 in: @type1;
// DEFAULT-NEXT:         field2 arr: array<i32, 4>;
// DEFAULT-NEXT:     } [size=28, align=4, offsets=[0, 4, 12]];
// DEFAULT-NEXT:     type @type3 outer_t = @type2;
// DEFAULT-NEXT:     type @type4 = enum : u32 {
// DEFAULT-NEXT:         %0 SEVEN = const<i32>(7);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %0 header_global: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 matrix: array<array<i32, 3>, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 global_outer: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 file_static: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %19 function_static: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %3 @header_function() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @callee(%20 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @names(%14 param: i32, %15 pointer: ptr<@type2>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 local: i32 [storage=automatic];
// DEFAULT-NEXT:         let %17 local_array: array<i32, 3> [storage=automatic];
// DEFAULT-NEXT:         let %18 s: @type2 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "mov eax, param\nmov local, eax\nmov eax, local_array[4]\nmov eax, local_array[ebx * 4 + 4]\nmov eax, s.in.b\nmov eax, s.arr[8]\nmov eax, global_outer.in.b\nmov eax, file_static + 4\nmov eax, [function_static]\nmov eax, SEVEN[eax]\nmov eax, 2 * SEVEN + 3\nmov ecx, type matrix\nmov ecx, length matrix\nmov ecx, size matrix\nmov ecx, type s.in\nmov eax, pointer\nmov al, [eax] + outer_t.in\nmov eax, offset file_static\npush 1\ncall callee\nadd esp, 4\nmov eax, header_global\nmov eax, HEADER_ENUM\ncall header_function\njmp short later" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, " addr(%0) "\nmov " addr(%1) ", eax\nmov eax, " addr(%2 + 4) "\nmov eax, " addr(%2 + ebx*4 + 4) "\nmov eax, " addr(%3 + 8) "\nmov eax, " addr(%3 + 20) "\nmov eax, " addr(%4 + 8) "\nmov eax, " addr(%5 + 4) "\nmov eax, " addr(%6) "\nmov eax, [eax + 7]\nmov eax, 17\nmov ecx, 12\nmov ecx, 2\nmov ecx, 24\nmov ecx, 8\nmov eax, " addr(%7) "\nmov al, [eax + 4]\nmov eax, " %8 "\npush 1\ncall " %9 "\nadd esp, 4\nmov eax, " addr(%10) "\nmov eax, 3\ncall " %11 "\njmp short " label(later);
// DEFAULT-NEXT:             in 0 [param] mem<readwrite> place<i32>(%14);
// DEFAULT-NEXT:             in 1 [local] mem<readwrite> place<i32>(%16);
// DEFAULT-NEXT:             in 2 [local_array] mem<readwrite> place<array<i32, 3>>(%17);
// DEFAULT-NEXT:             in 3 [s] mem<readwrite> place<@type2>(%18);
// DEFAULT-NEXT:             in 4 [global_outer] mem<readwrite> place<@type2>(%10);
// DEFAULT-NEXT:             in 5 [file_static] mem<readwrite> place<i32>(%11);
// DEFAULT-NEXT:             in 6 [function_static] mem<readwrite> place<i32>(%19);
// DEFAULT-NEXT:             in 7 [pointer] mem<readwrite> place<ptr<@type2>>(%15);
// DEFAULT-NEXT:             in 8 [file_static] addr_of<ptr<i32>>(%11);
// DEFAULT-NEXT:             in 9 sym<offset=0>(%12);
// DEFAULT-NEXT:             in 10 [header_global] mem<readwrite> place<i32>(%0);
// DEFAULT-NEXT:             in 11 sym<offset=0>(%3);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "later:\nmov local, eax" [dialect=intel] {
// DEFAULT-NEXT:             template: label(later) ":\nmov " addr(%0) ", eax";
// DEFAULT-NEXT:             in 0 [local] mem<readwrite> place<i32>(%16);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
