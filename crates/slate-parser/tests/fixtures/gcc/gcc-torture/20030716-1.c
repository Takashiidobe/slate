// SLATE-FILECHECK-DEFINES DEFAULT

void baz(int i);

void foo(int i, int A[i+1])
{
    int j=A[i];
    void bar() { baz(A[i]); }
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid in this context: function definition is not allowed here
// SLATE-FILECHECK-END DEFAULT
