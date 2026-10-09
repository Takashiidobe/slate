// SLATE-FILECHECK-ARGS --dump-ir -std=c2y
// SLATE-FILECHECK-ERROR SEMANTIC

void invalid(int x) {
outer:
    for (int i = 0; i < 4; ++i)
        break later;
choice:
    switch (x) {
    case 0:
        for (int i = 0; i < 4; ++i)
            continue choice;
        break choice;
    case 1:
        [[fallthrough]];
    skipped:
    case 2:
        [[fallthrough]];
    case 3:
        while (x)
            break skipped;
    }
empty:;
    while (x)
        continue empty;
later:
    while (x)
        continue outer;
}

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × `break` label `later` does not name an enclosing loop or switch
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/named_loop_invalid_targets.c:5:15]
// SEMANTIC: 4 │     for (int i = 0; i < 4; ++i)
// SEMANTIC: 5 │         break later;
// SEMANTIC: ·               ─────
// SEMANTIC: 6 │ choice:
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × `continue` label `choice` names a switch, not a loop
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/named_loop_invalid_targets.c:10:22]
// SEMANTIC: 9 │         for (int i = 0; i < 4; ++i)
// SEMANTIC: 10 │             continue choice;
// SEMANTIC: ·                      ──────
// SEMANTIC: 11 │         break choice;
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × `break` label `skipped` does not name an enclosing loop or switch
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/named_loop_invalid_targets.c:19:19]
// SEMANTIC: 18 │         while (x)
// SEMANTIC: 19 │             break skipped;
// SEMANTIC: ·                   ───────
// SEMANTIC: 20 │     }
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × `continue` label `empty` does not name an enclosing loop
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/named_loop_invalid_targets.c:23:18]
// SEMANTIC: 22 │     while (x)
// SEMANTIC: 23 │         continue empty;
// SEMANTIC: ·                  ─────
// SEMANTIC: 24 │ later:
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × `continue` label `outer` does not name an enclosing loop
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/named_loop_invalid_targets.c:26:18]
// SEMANTIC: 25 │     while (x)
// SEMANTIC: 26 │         continue outer;
// SEMANTIC: ·                  ─────
// SEMANTIC: 27 │ }
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
