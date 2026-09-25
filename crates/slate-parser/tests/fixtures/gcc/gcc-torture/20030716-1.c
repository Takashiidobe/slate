// SLATE-FILECHECK-DEFINES DEFAULT

void baz(int i);

void foo(int i, int A[i+1])
{
    int j=A[i];
    void bar() { baz(A[i]); }
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: module statement
// SLATE-FILECHECK-END DEFAULT
