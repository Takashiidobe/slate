#if defined(FLOAT)
typedef double invalid;
#elif defined(POINTER)
typedef int *invalid;
#elif defined(ARRAY)
typedef int invalid[2];
#elif defined(ENUM)
typedef enum unknown invalid;
#else
typedef struct { int value; } invalid;
#endif
int maximum = _Maxof(invalid);
int minimum = _Minof(invalid);

// SLATE-FILECHECK-DEFINES FLOAT FLOAT
// SLATE-FILECHECK-STD FLOAT c2y
// SLATE-FILECHECK-ERROR FLOAT
// SLATE-FILECHECK-DEFINES POINTER POINTER
// SLATE-FILECHECK-STD POINTER c2y
// SLATE-FILECHECK-ERROR POINTER
// SLATE-FILECHECK-DEFINES ARRAY ARRAY
// SLATE-FILECHECK-STD ARRAY c2y
// SLATE-FILECHECK-ERROR ARRAY
// SLATE-FILECHECK-DEFINES ENUM ENUM
// SLATE-FILECHECK-STD ENUM c2y
// SLATE-FILECHECK-ERROR ENUM
// SLATE-FILECHECK-DEFINES RECORD RECORD
// SLATE-FILECHECK-STD RECORD c2y
// SLATE-FILECHECK-ERROR RECORD

// SLATE-FILECHECK-BEGIN FLOAT
// FLOAT: Error:   × semantic analysis failed
// FLOAT: Error:
// FLOAT: × integer type limit requires an integer type
// FLOAT: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// FLOAT: 11 │ #endif
// FLOAT: 12 │ int maximum = _Maxof(invalid);
// FLOAT: ·               ───────────────
// FLOAT: 13 │ int minimum = _Minof(invalid);
// FLOAT: ╰────
// FLOAT: Error:
// FLOAT: × integer type limit requires an integer type
// FLOAT: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// FLOAT: 11 │ #endif
// FLOAT: 12 │ int maximum = _Maxof(invalid);
// FLOAT: ·               ───────────────
// FLOAT: 13 │ int minimum = _Minof(invalid);
// FLOAT: ╰────
// FLOAT: Error:
// FLOAT: × integer type limit requires an integer type
// FLOAT: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// FLOAT: 12 │ int maximum = _Maxof(invalid);
// FLOAT: 13 │ int minimum = _Minof(invalid);
// FLOAT: ·               ───────────────
// FLOAT: 14 │
// FLOAT: ╰────
// FLOAT: Error:
// FLOAT: × integer type limit requires an integer type
// FLOAT: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// FLOAT: 12 │ int maximum = _Maxof(invalid);
// FLOAT: 13 │ int minimum = _Minof(invalid);
// FLOAT: ·               ───────────────
// FLOAT: 14 │
// FLOAT: ╰────
// SLATE-FILECHECK-END FLOAT
// SLATE-FILECHECK-BEGIN POINTER
// POINTER: Error:   × semantic analysis failed
// POINTER: Error:
// POINTER: × integer type limit requires an integer type
// POINTER: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// POINTER: 11 │ #endif
// POINTER: 12 │ int maximum = _Maxof(invalid);
// POINTER: ·               ───────────────
// POINTER: 13 │ int minimum = _Minof(invalid);
// POINTER: ╰────
// POINTER: Error:
// POINTER: × integer type limit requires an integer type
// POINTER: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// POINTER: 11 │ #endif
// POINTER: 12 │ int maximum = _Maxof(invalid);
// POINTER: ·               ───────────────
// POINTER: 13 │ int minimum = _Minof(invalid);
// POINTER: ╰────
// POINTER: Error:
// POINTER: × integer type limit requires an integer type
// POINTER: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// POINTER: 12 │ int maximum = _Maxof(invalid);
// POINTER: 13 │ int minimum = _Minof(invalid);
// POINTER: ·               ───────────────
// POINTER: 14 │
// POINTER: ╰────
// POINTER: Error:
// POINTER: × integer type limit requires an integer type
// POINTER: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// POINTER: 12 │ int maximum = _Maxof(invalid);
// POINTER: 13 │ int minimum = _Minof(invalid);
// POINTER: ·               ───────────────
// POINTER: 14 │
// POINTER: ╰────
// SLATE-FILECHECK-END POINTER
// SLATE-FILECHECK-BEGIN ARRAY
// ARRAY: Error:   × semantic analysis failed
// ARRAY: Error:
// ARRAY: × integer type limit requires an integer type
// ARRAY: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// ARRAY: 11 │ #endif
// ARRAY: 12 │ int maximum = _Maxof(invalid);
// ARRAY: ·               ───────────────
// ARRAY: 13 │ int minimum = _Minof(invalid);
// ARRAY: ╰────
// ARRAY: Error:
// ARRAY: × integer type limit requires an integer type
// ARRAY: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// ARRAY: 11 │ #endif
// ARRAY: 12 │ int maximum = _Maxof(invalid);
// ARRAY: ·               ───────────────
// ARRAY: 13 │ int minimum = _Minof(invalid);
// ARRAY: ╰────
// ARRAY: Error:
// ARRAY: × integer type limit requires an integer type
// ARRAY: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// ARRAY: 12 │ int maximum = _Maxof(invalid);
// ARRAY: 13 │ int minimum = _Minof(invalid);
// ARRAY: ·               ───────────────
// ARRAY: 14 │
// ARRAY: ╰────
// ARRAY: Error:
// ARRAY: × integer type limit requires an integer type
// ARRAY: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// ARRAY: 12 │ int maximum = _Maxof(invalid);
// ARRAY: 13 │ int minimum = _Minof(invalid);
// ARRAY: ·               ───────────────
// ARRAY: 14 │
// ARRAY: ╰────
// SLATE-FILECHECK-END ARRAY
// SLATE-FILECHECK-BEGIN ENUM
// ENUM: Error:   × semantic analysis failed
// ENUM: Error:
// ENUM: × integer type limit of an incomplete enum
// ENUM: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// ENUM: 11 │ #endif
// ENUM: 12 │ int maximum = _Maxof(invalid);
// ENUM: ·               ───────────────
// ENUM: 13 │ int minimum = _Minof(invalid);
// ENUM: ╰────
// ENUM: Error:
// ENUM: × integer type limit of an incomplete enum
// ENUM: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// ENUM: 11 │ #endif
// ENUM: 12 │ int maximum = _Maxof(invalid);
// ENUM: ·               ───────────────
// ENUM: 13 │ int minimum = _Minof(invalid);
// ENUM: ╰────
// ENUM: Error:
// ENUM: × integer type limit of an incomplete enum
// ENUM: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// ENUM: 12 │ int maximum = _Maxof(invalid);
// ENUM: 13 │ int minimum = _Minof(invalid);
// ENUM: ·               ───────────────
// ENUM: 14 │
// ENUM: ╰────
// ENUM: Error:
// ENUM: × integer type limit of an incomplete enum
// ENUM: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// ENUM: 12 │ int maximum = _Maxof(invalid);
// ENUM: 13 │ int minimum = _Minof(invalid);
// ENUM: ·               ───────────────
// ENUM: 14 │
// ENUM: ╰────
// SLATE-FILECHECK-END ENUM
// SLATE-FILECHECK-BEGIN RECORD
// RECORD: Error:   × semantic analysis failed
// RECORD: Error:
// RECORD: × integer type limit requires an integer type
// RECORD: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// RECORD: 11 │ #endif
// RECORD: 12 │ int maximum = _Maxof(invalid);
// RECORD: ·               ───────────────
// RECORD: 13 │ int minimum = _Minof(invalid);
// RECORD: ╰────
// RECORD: Error:
// RECORD: × integer type limit requires an integer type
// RECORD: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:12:15]
// RECORD: 11 │ #endif
// RECORD: 12 │ int maximum = _Maxof(invalid);
// RECORD: ·               ───────────────
// RECORD: 13 │ int minimum = _Minof(invalid);
// RECORD: ╰────
// RECORD: Error:
// RECORD: × integer type limit requires an integer type
// RECORD: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// RECORD: 12 │ int maximum = _Maxof(invalid);
// RECORD: 13 │ int minimum = _Minof(invalid);
// RECORD: ·               ───────────────
// RECORD: 14 │
// RECORD: ╰────
// RECORD: Error:
// RECORD: × integer type limit requires an integer type
// RECORD: ╭─[tests/fixtures/error/msvc/windows/x86_64/integer_type_limits.c:13:15]
// RECORD: 12 │ int maximum = _Maxof(invalid);
// RECORD: 13 │ int minimum = _Minof(invalid);
// RECORD: ·               ───────────────
// RECORD: 14 │
// RECORD: ╰────
// SLATE-FILECHECK-END RECORD
