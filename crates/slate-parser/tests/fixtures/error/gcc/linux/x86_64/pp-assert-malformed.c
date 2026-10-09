#ifdef NO_ANSWER
#assert pred
#endif
#ifdef EMPTY_ANSWER
#assert pred()
#endif
#ifdef UNTERMINATED
#assert pred(answer
#endif
#ifdef NOT_IDENTIFIER
#if #1(answer)
#endif
#endif
int value;

// SLATE-FILECHECK-DEFINES NO_ANSWER NO_ANSWER
// SLATE-FILECHECK-ERROR NO_ANSWER
// SLATE-FILECHECK-DEFINES EMPTY_ANSWER EMPTY_ANSWER
// SLATE-FILECHECK-ERROR EMPTY_ANSWER
// SLATE-FILECHECK-DEFINES UNTERMINATED UNTERMINATED
// SLATE-FILECHECK-ERROR UNTERMINATED
// SLATE-FILECHECK-DEFINES NOT_IDENTIFIER NOT_IDENTIFIER
// SLATE-FILECHECK-ERROR NOT_IDENTIFIER

// SLATE-FILECHECK-BEGIN NO_ANSWER
// NO_ANSWER: Error:   × missing `(` after predicate
// NO_ANSWER: ╰─▶ missing `(` after predicate
// NO_ANSWER: ╭─[tests/fixtures/error/gcc/linux/x86_64/pp-assert-malformed.c:2:9]
// NO_ANSWER: 1 │ #ifdef NO_ANSWER
// NO_ANSWER: 2 │ #assert pred
// NO_ANSWER: ·         ────
// NO_ANSWER: 3 │ #endif
// NO_ANSWER: ╰────
// SLATE-FILECHECK-END NO_ANSWER
// SLATE-FILECHECK-BEGIN EMPTY_ANSWER
// EMPTY_ANSWER: Error:   × predicate's answer is empty
// EMPTY_ANSWER: ╰─▶ predicate's answer is empty
// EMPTY_ANSWER: ╭─[tests/fixtures/error/gcc/linux/x86_64/pp-assert-malformed.c:5:14]
// EMPTY_ANSWER: 4 │ #ifdef EMPTY_ANSWER
// EMPTY_ANSWER: 5 │ #assert pred()
// EMPTY_ANSWER: ·              ─
// EMPTY_ANSWER: 6 │ #endif
// EMPTY_ANSWER: ╰────
// SLATE-FILECHECK-END EMPTY_ANSWER
// SLATE-FILECHECK-BEGIN UNTERMINATED
// UNTERMINATED: Error:   × missing `)` to complete answer
// UNTERMINATED: ╰─▶ missing `)` to complete answer
// UNTERMINATED: ╭─[tests/fixtures/error/gcc/linux/x86_64/pp-assert-malformed.c:8:20]
// UNTERMINATED: 7 │ #ifdef UNTERMINATED
// UNTERMINATED: 8 │ #assert pred(answer
// UNTERMINATED: ·                    ▲
// UNTERMINATED: 9 │ #endif
// UNTERMINATED: ╰────
// SLATE-FILECHECK-END UNTERMINATED
// SLATE-FILECHECK-BEGIN NOT_IDENTIFIER
// NOT_IDENTIFIER: Error:   × predicate must be an identifier
// NOT_IDENTIFIER: ╰─▶ predicate must be an identifier
// NOT_IDENTIFIER: ╭─[tests/fixtures/error/gcc/linux/x86_64/pp-assert-malformed.c:11:6]
// NOT_IDENTIFIER: 10 │ #ifdef NOT_IDENTIFIER
// NOT_IDENTIFIER: 11 │ #if #1(answer)
// NOT_IDENTIFIER: ·      ─
// NOT_IDENTIFIER: 12 │ #endif
// NOT_IDENTIFIER: ╰────
// SLATE-FILECHECK-END NOT_IDENTIFIER
