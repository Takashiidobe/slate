typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(256)));
typedef int gnu_i32 __attribute__((vector_size(32)));
fixed_i32 mix(fixed_i32 a, gnu_i32 b) { return a + b; }

// SLATE-FILECHECK-ARGS -march=armv8.2-a+sve -msve-vector-bits=256
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × cannot combine GNU and SVE vectors in a binary operation
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sve-fixed-gnu-mix.c:3:48]
// PARSE:  2 │ typedef int gnu_i32 __attribute__((vector_size(32)));
// PARSE:  3 │ fixed_i32 mix(fixed_i32 a, gnu_i32 b) { return a + b; }
// PARSE:    ·                                                ─────
// PARSE:  4 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
