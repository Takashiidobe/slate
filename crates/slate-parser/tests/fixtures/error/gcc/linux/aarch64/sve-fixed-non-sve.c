typedef int fixed_int __attribute__((arm_sve_vector_bits(256)));

// SLATE-FILECHECK-ARGS -march=armv8.2-a+sve -msve-vector-bits=256
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × 'arm_sve_vector_bits' applied to non-SVE type
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sve-fixed-non-sve.c:1:1]
// PARSE:  1 │ typedef int fixed_int __attribute__((arm_sve_vector_bits(256)));
// PARSE:    · ────────────────────────────────────────────────────────────────
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
