int *returns_pointer(void) { return 0; }
int (*returns_function_pointer(int x))(char) { return 0; }
void *(*returns_pointer_returning_function_pointer(int))(void) { return 0; }
int (parenthesized_name)(int x) { return x; }

void outer(void) {
  int (*nested(int x))(char) { return 0; }
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: module statement
// SLATE-FILECHECK-END DEFAULT
