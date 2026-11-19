typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(256)));
typedef fixed_i32 again __attribute__((arm_sve_vector_bits(256)));

// SLATE-FILECHECK-ARGS -march=armv8.2-a+sve -msve-vector-bits=256
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × 'arm_sve_vector_bits' applied to a type which already has a size
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sve-fixed-already-sized.c:2:1]
// PARSE:  1 │ typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(256)));
// PARSE:  2 │ typedef fixed_i32 again __attribute__((arm_sve_vector_bits(256)));
// PARSE:    · ──────────────────────────────────────────────────────────────────
// PARSE:  3 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
