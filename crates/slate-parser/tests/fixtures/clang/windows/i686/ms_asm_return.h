typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
typedef unsigned long DWORD;

ULONGLONG Int64ShllMod32(ULONGLONG Value, DWORD ShiftCount) {
    __asm {
        mov ecx, ShiftCount
        mov eax, dword ptr [Value]
        mov edx, dword ptr [Value+4]
        shld edx, eax, cl
        shl eax, cl
    }
}

LONGLONG Int64ShraMod32(LONGLONG Value, DWORD ShiftCount) {
    __asm {
        mov ecx, ShiftCount
        mov eax, dword ptr [Value]
        mov edx, dword ptr [Value+4]
        shrd eax, edx, cl
        sar edx, cl
    }
}

ULONGLONG Int64ShrlMod32(ULONGLONG Value, DWORD ShiftCount) {
    __asm {
        mov ecx, ShiftCount
        mov eax, dword ptr [Value]
        mov edx, dword ptr [Value+4]
        shrd eax, edx, cl
        shr edx, cl
    }
}

int word(int input) { __asm { mov eax, input } }
unsigned char byte(void) { __asm { mov al, 255 } }
short half(void) { __asm { mov ax, 32768 } }
_Bool boolean(void) { __asm { mov eax, 2 } }
void *pointer(void) { __asm { mov eax, 4096 } }
enum Result { First, Second };
enum Result enumeration(void) { __asm { mov eax, 1 } }
unsigned _BitInt(33) bits(void) { __asm { mov eax, 1 } }

int branches(int condition) {
    if (condition) { __asm { mov eax, 1 } }
    else { __asm { mov eax, 2 } }
}

int multiple(void) {
    __asm { mov eax, 3 }
    __asm { nop }
}

long long high_only(void) { __asm { mov edx, 4 } }
long long low_only(void) { __asm { mov eax, 5 } }
int explicit_return(void) { __asm { mov eax, 6 } return 7; }
int partial_return(int condition) {
    if (condition) return 8;
    __asm { mov eax, 9 }
}

void nothing(void) { __asm { nop } }
double floating(void) { __asm { fldz } }
struct Pair { int x, y; };
struct Pair aggregate(void) { __asm { mov eax, 1 } }
unsigned _BitInt(65) wide(void) { __asm { mov eax, 1 } }
__declspec(naked) int naked(void) { __asm { mov eax, 1 } }
__declspec(noreturn) int never(void) { __asm { int 3 } }
int no_asm(void) { }
int main(int argc, char **argv) {
    if (argc > 1) { __asm { mov eax, 10 } }
}
