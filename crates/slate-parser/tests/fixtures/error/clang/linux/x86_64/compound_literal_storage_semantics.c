struct pair { int member; };
#if defined(FILE_REGISTER)
int value = (register int){1};
#else
void invalid(int value) {
#if defined(THREAD)
    (thread_local int){1};
#elif defined(THREAD_REGISTER)
    (thread_local register int){1};
#elif defined(THREAD_CONSTEXPR)
    (thread_local static constexpr int){1};
#elif defined(ADDRESS)
    &(register int){1};
#elif defined(ARRAY_ADDRESS)
    &(register int[2]){1, 2}[0];
#elif defined(MEMBER_ADDRESS)
    &((register struct pair){1}.member);
#elif defined(STATIC_INIT)
    (static int){value};
#elif defined(THREAD_INIT)
    (static thread_local int){value};
#elif defined(CONSTEXPR_INIT)
    (constexpr int){value};
#elif defined(CONSTEXPR_VOLATILE)
    (constexpr volatile int){1};
#elif defined(CONSTEXPR_ATOMIC)
    (constexpr _Atomic int){1};
#elif defined(CONSTEXPR_ASSIGN)
    (constexpr int){1} = 2;
#else
    (static int){1 / 0};
#endif
}
#endif

// SLATE-FILECHECK-DEFINES FILE_REGISTER FILE_REGISTER
// SLATE-FILECHECK-PREFIX-ARGS FILE_REGISTER --dump-ir
// SLATE-FILECHECK-IR-ERROR FILE_REGISTER
// SLATE-FILECHECK-STD FILE_REGISTER c23
// SLATE-FILECHECK-DEFINES THREAD THREAD
// SLATE-FILECHECK-PREFIX-ARGS THREAD --dump-ir
// SLATE-FILECHECK-IR-ERROR THREAD
// SLATE-FILECHECK-STD THREAD c23
// SLATE-FILECHECK-DEFINES THREAD_REGISTER THREAD_REGISTER
// SLATE-FILECHECK-PREFIX-ARGS THREAD_REGISTER --dump-ir
// SLATE-FILECHECK-IR-ERROR THREAD_REGISTER
// SLATE-FILECHECK-STD THREAD_REGISTER c23
// SLATE-FILECHECK-DEFINES THREAD_CONSTEXPR THREAD_CONSTEXPR
// SLATE-FILECHECK-PREFIX-ARGS THREAD_CONSTEXPR --dump-ir
// SLATE-FILECHECK-IR-ERROR THREAD_CONSTEXPR
// SLATE-FILECHECK-STD THREAD_CONSTEXPR c23
// SLATE-FILECHECK-DEFINES ADDRESS ADDRESS
// SLATE-FILECHECK-PREFIX-ARGS ADDRESS --dump-ir
// SLATE-FILECHECK-IR-ERROR ADDRESS
// SLATE-FILECHECK-STD ADDRESS c23
// SLATE-FILECHECK-DEFINES MEMBER_ADDRESS MEMBER_ADDRESS
// SLATE-FILECHECK-PREFIX-ARGS MEMBER_ADDRESS --dump-ir
// SLATE-FILECHECK-IR-ERROR MEMBER_ADDRESS
// SLATE-FILECHECK-STD MEMBER_ADDRESS c23
// SLATE-FILECHECK-DEFINES STATIC_INIT STATIC_INIT
// SLATE-FILECHECK-PREFIX-ARGS STATIC_INIT --dump-ir
// SLATE-FILECHECK-IR-ERROR STATIC_INIT
// SLATE-FILECHECK-STD STATIC_INIT c23
// SLATE-FILECHECK-DEFINES THREAD_INIT THREAD_INIT
// SLATE-FILECHECK-PREFIX-ARGS THREAD_INIT --dump-ir
// SLATE-FILECHECK-IR-ERROR THREAD_INIT
// SLATE-FILECHECK-STD THREAD_INIT c23
// SLATE-FILECHECK-DEFINES CONSTEXPR_INIT CONSTEXPR_INIT
// SLATE-FILECHECK-PREFIX-ARGS CONSTEXPR_INIT --dump-ir
// SLATE-FILECHECK-IR-ERROR CONSTEXPR_INIT
// SLATE-FILECHECK-STD CONSTEXPR_INIT c23
// SLATE-FILECHECK-DEFINES CONSTEXPR_ASSIGN CONSTEXPR_ASSIGN
// SLATE-FILECHECK-PREFIX-ARGS CONSTEXPR_ASSIGN --dump-ir
// SLATE-FILECHECK-IR-ERROR CONSTEXPR_ASSIGN
// SLATE-FILECHECK-STD CONSTEXPR_ASSIGN c23
// SLATE-FILECHECK-DEFINES DIV_ZERO DIV_ZERO
// SLATE-FILECHECK-PREFIX-ARGS DIV_ZERO --dump-ir
// SLATE-FILECHECK-IR-ERROR DIV_ZERO
// SLATE-FILECHECK-STD DIV_ZERO c23
// SLATE-FILECHECK-DEFINES ARRAY_ADDRESS ARRAY_ADDRESS
// SLATE-FILECHECK-PREFIX-ARGS ARRAY_ADDRESS --dump-ir
// SLATE-FILECHECK-IR-ERROR ARRAY_ADDRESS
// SLATE-FILECHECK-STD ARRAY_ADDRESS c23
// SLATE-FILECHECK-DEFINES CONSTEXPR_VOLATILE CONSTEXPR_VOLATILE
// SLATE-FILECHECK-PREFIX-ARGS CONSTEXPR_VOLATILE --dump-ir
// SLATE-FILECHECK-IR-ERROR CONSTEXPR_VOLATILE
// SLATE-FILECHECK-STD CONSTEXPR_VOLATILE c23
// SLATE-FILECHECK-DEFINES CONSTEXPR_ATOMIC CONSTEXPR_ATOMIC
// SLATE-FILECHECK-PREFIX-ARGS CONSTEXPR_ATOMIC --dump-ir
// SLATE-FILECHECK-IR-ERROR CONSTEXPR_ATOMIC
// SLATE-FILECHECK-STD CONSTEXPR_ATOMIC c23

// SLATE-FILECHECK-BEGIN FILE_REGISTER
// FILE_REGISTER: Error:   × semantic analysis failed
// FILE_REGISTER: Error:
// FILE_REGISTER: × register compound literal at file scope
// FILE_REGISTER: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:3:13]
// FILE_REGISTER: 2 │ #if defined(FILE_REGISTER)
// FILE_REGISTER: 3 │ int value = (register int){1};
// FILE_REGISTER: ·             ─────────────────
// FILE_REGISTER: 4 │ #else
// FILE_REGISTER: ╰────
// SLATE-FILECHECK-END FILE_REGISTER
// SLATE-FILECHECK-BEGIN THREAD
// THREAD: Error:   × semantic analysis failed
// THREAD: Error:
// THREAD: × invalid storage-class combination for thread-local compound literal
// THREAD: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:7:5]
// THREAD: 6 │ #if defined(THREAD)
// THREAD: 7 │     (thread_local int){1};
// THREAD: ·     ─────────────────────
// THREAD: 8 │ #elif defined(THREAD_REGISTER)
// THREAD: ╰────
// SLATE-FILECHECK-END THREAD
// SLATE-FILECHECK-BEGIN THREAD_REGISTER
// THREAD_REGISTER: Error:   × semantic analysis failed
// THREAD_REGISTER: Error:
// THREAD_REGISTER: × invalid storage-class combination for thread-local compound literal
// THREAD_REGISTER: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:9:5]
// THREAD_REGISTER: 8 │ #elif defined(THREAD_REGISTER)
// THREAD_REGISTER: 9 │     (thread_local register int){1};
// THREAD_REGISTER: ·     ──────────────────────────────
// THREAD_REGISTER: 10 │ #elif defined(THREAD_CONSTEXPR)
// THREAD_REGISTER: ╰────
// SLATE-FILECHECK-END THREAD_REGISTER
// SLATE-FILECHECK-BEGIN THREAD_CONSTEXPR
// THREAD_CONSTEXPR: Error:   × semantic analysis failed
// THREAD_CONSTEXPR: Error:
// THREAD_CONSTEXPR: × invalid storage-class combination for thread-local compound literal
// THREAD_CONSTEXPR: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:11:5]
// THREAD_CONSTEXPR: 10 │ #elif defined(THREAD_CONSTEXPR)
// THREAD_CONSTEXPR: 11 │     (thread_local static constexpr int){1};
// THREAD_CONSTEXPR: ·     ──────────────────────────────────────
// THREAD_CONSTEXPR: 12 │ #elif defined(ADDRESS)
// THREAD_CONSTEXPR: ╰────
// SLATE-FILECHECK-END THREAD_CONSTEXPR
// SLATE-FILECHECK-BEGIN ADDRESS
// ADDRESS: Error:   × semantic analysis failed
// ADDRESS: Error:
// ADDRESS: × address of register variable requested
// ADDRESS: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:13:5]
// ADDRESS: 12 │ #elif defined(ADDRESS)
// ADDRESS: 13 │     &(register int){1};
// ADDRESS: ·     ──────────────────
// ADDRESS: 14 │ #elif defined(ARRAY_ADDRESS)
// ADDRESS: ╰────
// SLATE-FILECHECK-END ADDRESS
// SLATE-FILECHECK-BEGIN MEMBER_ADDRESS
// MEMBER_ADDRESS: Error:   × semantic analysis failed
// MEMBER_ADDRESS: Error:
// MEMBER_ADDRESS: × address of register variable requested
// MEMBER_ADDRESS: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:17:5]
// MEMBER_ADDRESS: 16 │ #elif defined(MEMBER_ADDRESS)
// MEMBER_ADDRESS: 17 │     &((register struct pair){1}.member);
// MEMBER_ADDRESS: ·     ───────────────────────────────────
// MEMBER_ADDRESS: 18 │ #elif defined(STATIC_INIT)
// MEMBER_ADDRESS: ╰────
// SLATE-FILECHECK-END MEMBER_ADDRESS
// SLATE-FILECHECK-BEGIN STATIC_INIT
// STATIC_INIT: Error:   × semantic analysis failed
// STATIC_INIT: Error:
// STATIC_INIT: × compound literal initializer is not a constant expression
// STATIC_INIT: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:19:18]
// STATIC_INIT: 18 │ #elif defined(STATIC_INIT)
// STATIC_INIT: 19 │     (static int){value};
// STATIC_INIT: ·                  ─────
// STATIC_INIT: 20 │ #elif defined(THREAD_INIT)
// STATIC_INIT: ╰────
// SLATE-FILECHECK-END STATIC_INIT
// SLATE-FILECHECK-BEGIN THREAD_INIT
// THREAD_INIT: Error:   × semantic analysis failed
// THREAD_INIT: Error:
// THREAD_INIT: × compound literal initializer is not a constant expression
// THREAD_INIT: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:21:31]
// THREAD_INIT: 20 │ #elif defined(THREAD_INIT)
// THREAD_INIT: 21 │     (static thread_local int){value};
// THREAD_INIT: ·                               ─────
// THREAD_INIT: 22 │ #elif defined(CONSTEXPR_INIT)
// THREAD_INIT: ╰────
// SLATE-FILECHECK-END THREAD_INIT
// SLATE-FILECHECK-BEGIN CONSTEXPR_INIT
// CONSTEXPR_INIT: Error:   × semantic analysis failed
// CONSTEXPR_INIT: Error:
// CONSTEXPR_INIT: × compound literal initializer is not a constant expression
// CONSTEXPR_INIT: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:23:21]
// CONSTEXPR_INIT: 22 │ #elif defined(CONSTEXPR_INIT)
// CONSTEXPR_INIT: 23 │     (constexpr int){value};
// CONSTEXPR_INIT: ·                     ─────
// CONSTEXPR_INIT: 24 │ #elif defined(CONSTEXPR_VOLATILE)
// CONSTEXPR_INIT: ╰────
// SLATE-FILECHECK-END CONSTEXPR_INIT
// SLATE-FILECHECK-BEGIN CONSTEXPR_ASSIGN
// CONSTEXPR_ASSIGN: Error:   × semantic analysis failed
// CONSTEXPR_ASSIGN: Error:
// CONSTEXPR_ASSIGN: × cannot assign to a const-qualified lvalue
// CONSTEXPR_ASSIGN: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:29:5]
// CONSTEXPR_ASSIGN: 28 │ #elif defined(CONSTEXPR_ASSIGN)
// CONSTEXPR_ASSIGN: 29 │     (constexpr int){1} = 2;
// CONSTEXPR_ASSIGN: ·     ──────────────────────
// CONSTEXPR_ASSIGN: 30 │ #else
// CONSTEXPR_ASSIGN: ╰────
// SLATE-FILECHECK-END CONSTEXPR_ASSIGN
// SLATE-FILECHECK-BEGIN DIV_ZERO
// DIV_ZERO: Error:   × semantic analysis failed
// DIV_ZERO: Error:
// DIV_ZERO: × initializer element is not a compile-time constant: division by zero
// DIV_ZERO: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:31:18]
// DIV_ZERO: 30 │ #else
// DIV_ZERO: 31 │     (static int){1 / 0};
// DIV_ZERO: ·                  ─────
// DIV_ZERO: 32 │ #endif
// DIV_ZERO: ╰────
// SLATE-FILECHECK-END DIV_ZERO
// SLATE-FILECHECK-BEGIN ARRAY_ADDRESS
// ARRAY_ADDRESS: Error:   × semantic analysis failed
// ARRAY_ADDRESS: Error:
// ARRAY_ADDRESS: × address of register variable requested
// ARRAY_ADDRESS: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:15:5]
// ARRAY_ADDRESS: 14 │ #elif defined(ARRAY_ADDRESS)
// ARRAY_ADDRESS: 15 │     &(register int[2]){1, 2}[0];
// ARRAY_ADDRESS: ·     ───────────────────────────
// ARRAY_ADDRESS: 16 │ #elif defined(MEMBER_ADDRESS)
// ARRAY_ADDRESS: ╰────
// SLATE-FILECHECK-END ARRAY_ADDRESS
// SLATE-FILECHECK-BEGIN CONSTEXPR_VOLATILE
// CONSTEXPR_VOLATILE: Error:   × semantic analysis failed
// CONSTEXPR_VOLATILE: Error:
// CONSTEXPR_VOLATILE: × constexpr compound literal has volatile, atomic, or restrict-qualified
// CONSTEXPR_VOLATILE: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:25:5]
// CONSTEXPR_VOLATILE: 24 │ #elif defined(CONSTEXPR_VOLATILE)
// CONSTEXPR_VOLATILE: 25 │     (constexpr volatile int){1};
// CONSTEXPR_VOLATILE: ·     ───────────────────────────
// CONSTEXPR_VOLATILE: 26 │ #elif defined(CONSTEXPR_ATOMIC)
// CONSTEXPR_VOLATILE: ╰────
// SLATE-FILECHECK-END CONSTEXPR_VOLATILE
// SLATE-FILECHECK-BEGIN CONSTEXPR_ATOMIC
// CONSTEXPR_ATOMIC: Error:   × semantic analysis failed
// CONSTEXPR_ATOMIC: Error:
// CONSTEXPR_ATOMIC: × constexpr compound literal has volatile, atomic, or restrict-qualified
// CONSTEXPR_ATOMIC: ╭─[tests/fixtures/error/clang/linux/x86_64/compound_literal_storage_semantics.c:27:5]
// CONSTEXPR_ATOMIC: 26 │ #elif defined(CONSTEXPR_ATOMIC)
// CONSTEXPR_ATOMIC: 27 │     (constexpr _Atomic int){1};
// CONSTEXPR_ATOMIC: ·     ──────────────────────────
// CONSTEXPR_ATOMIC: 28 │ #elif defined(CONSTEXPR_ASSIGN)
// CONSTEXPR_ATOMIC: ╰────
// SLATE-FILECHECK-END CONSTEXPR_ATOMIC
