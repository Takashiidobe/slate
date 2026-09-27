// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT
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
// DEFAULT: Error:   × unsupported in numeric IR lowering: MSVC `__asm` statement
// SLATE-FILECHECK-END DEFAULT
