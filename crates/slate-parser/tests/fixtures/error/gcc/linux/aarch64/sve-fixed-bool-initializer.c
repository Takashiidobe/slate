typedef svbool_t fixed_bool __attribute__((arm_sve_vector_bits(256)));
fixed_bool predicate = {1, 0, 1, 1};

// SLATE-FILECHECK-ARGS -march=armv8.2-a+sve -msve-vector-bits=256
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × braced initializer for a fixed-length svbool_t, whose lanes gcc packs one
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sve-fixed-bool-initializer.c:2:12]
// PARSE:  1 │ typedef svbool_t fixed_bool __attribute__((arm_sve_vector_bits(256)));
// PARSE:  2 │ fixed_bool predicate = {1, 0, 1, 1};
// PARSE:    ·            ────────────────────────
// PARSE:  3 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
