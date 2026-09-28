// SLATE-FILECHECK-DEFINES DEFAULT

void baz(int i);

void foo(int i, int A[i+1])
{
    int j=A[i];
    void bar() { baz(A[i]); }
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: function definition is not allowed here
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20030716-1.c:7:5]
// DEFAULT: 6 │     int j=A[i];
// DEFAULT: 7 │     void bar() { baz(A[i]); }
// DEFAULT: ·     ─────────────────────────
// DEFAULT: 8 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
