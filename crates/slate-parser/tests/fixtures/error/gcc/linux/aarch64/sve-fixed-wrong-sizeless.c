typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(256)));
svuint32_t convert(fixed_i32 value) { return value; }

// SLATE-FILECHECK-ARGS -march=armv8.2-a+sve -msve-vector-bits=256
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × incompatible SVE types
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sve-fixed-wrong-sizeless.c:2:46]
// PARSE:  1 │ typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(256)));
// PARSE:  2 │ svuint32_t convert(fixed_i32 value) { return value; }
// PARSE:    ·                                              ─────
// PARSE:  3 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
