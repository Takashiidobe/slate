int main() {
#ifdef _WIN32
  return 2;
#else
  return 3;
#endif
}

#ifdef _WIN32
typedef HANDLE Socket;
#else
typedef int Socket;
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WIN32 _WIN32

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: conditional
// DEFAULT-NEXT:     branch[0]: when=defined(_WIN32)
// DEFAULT-NEXT:       stmt[0]: return 2
// DEFAULT-NEXT:     branch[1]: when=not(defined(_WIN32))
// DEFAULT-NEXT:       stmt[0]: return 3
// DEFAULT-NEXT: decl[1]: conditional
// DEFAULT-NEXT:   branch[0]: when=defined(_WIN32)
// DEFAULT-NEXT:     decl[0]: typedef name=Socket type=HANDLE
// DEFAULT-NEXT:   branch[1]: when=not(defined(_WIN32))
// DEFAULT-NEXT:     decl[0]: typedef name=Socket type=int
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 3
// DEFAULT-NEXT: decl[1]: typedef name=Socket type=int
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN WIN32
// WIN32: polyvariant:
// WIN32-NEXT: decl[0]: function name=main return=int
// WIN32-NEXT:   stmt[0]: conditional
// WIN32-NEXT:     branch[0]: when=defined(_WIN32)
// WIN32-NEXT:       stmt[0]: return 2
// WIN32-NEXT:     branch[1]: when=not(defined(_WIN32))
// WIN32-NEXT:       stmt[0]: return 3
// WIN32-NEXT: decl[1]: conditional
// WIN32-NEXT:   branch[0]: when=defined(_WIN32)
// WIN32-NEXT:     decl[0]: typedef name=Socket type=HANDLE
// WIN32-NEXT:   branch[1]: when=not(defined(_WIN32))
// WIN32-NEXT:     decl[0]: typedef name=Socket type=int
// WIN32-NEXT: concrete:
// WIN32-NEXT: decl[0]: function name=main return=int
// WIN32-NEXT:   stmt[0]: return 2
// WIN32-NEXT: decl[1]: typedef name=Socket type=HANDLE
// SLATE-FILECHECK-END WIN32
