// SLATE-FILECHECK-ARGS --dump-ir -std=c2y
// SLATE-FILECHECK-ERROR SEMANTIC

void invalid(int x) {
first:
second:
    for (int i = 0; i < 4; ++i) {
        if (i)
            continue second;
        break first;
    }
    switch (x) {
    named:
    default:
        while (x)
            break named;
    }
choice:
    switch (x) {
    case 0:
        while (x)
            continue choice;
    }
}

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × `break` label `first` does not name an enclosing loop or switch
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/named_loop_invalid_targets.c:8:15]
// SEMANTIC: 7 │             continue second;
// SEMANTIC: 8 │         break first;
// SEMANTIC: ·               ─────
// SEMANTIC: 9 │     }
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × `break` label `named` does not name an enclosing loop or switch
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/named_loop_invalid_targets.c:14:19]
// SEMANTIC: 13 │         while (x)
// SEMANTIC: 14 │             break named;
// SEMANTIC: ·                   ─────
// SEMANTIC: 15 │     }
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × `continue` label `choice` names a switch, not a loop
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/named_loop_invalid_targets.c:20:22]
// SEMANTIC: 19 │         while (x)
// SEMANTIC: 20 │             continue choice;
// SEMANTIC: ·                      ──────
// SEMANTIC: 21 │     }
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
