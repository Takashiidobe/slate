typedef int T;

int compound(void) {
  return (enum {
    /* enum initializer must still see the typedef on the committed parse */
    T = sizeof(T), U = sizeof(T)
  }){0} + sizeof(T);
}

int sizeof_compound(void) {
  return sizeof(enum {
    /* sizeof first attempts a type-name operand */
    T = sizeof(T), U = sizeof(T)
  }){0} + sizeof(T);
}

int attribute_declaration(void) {
  __attribute__((aligned(sizeof(enum {
    /* preserve annotations across the standalone-attribute probe */
    T = sizeof(T), U = sizeof(T)
  })))) int value;
  return sizeof(T) + sizeof(value);
}

int compound_record(void) {
  return sizeof(struct {
    /* one record and one enum, with their comments and pragma retained */
#pragma pack(push, 1)
    enum { T = sizeof(T), U = sizeof(T) } value;
#pragma pack(pop)
  }){0} + sizeof(T);
}

int invalid_attribute(void) {
  __attribute__((alloc_size(sizeof(enum { T = 2 }), +))) int value;
  T after_invalid_attribute;
  return sizeof(T) + sizeof(value) + sizeof(after_invalid_attribute);
}

int initializers(void) {
  struct record { int values[4]; int tail; } local = {
    .values = { [0 ... 2] = 1, [3] = 2 }, tail: 3
  };
  return ((struct record){
    .values = { [0 ... 2] = 1, [3] = 2 }, tail: 3
  }).values[1] + local.tail;
}

T after_functions;

// SLATE-FILECHECK-ARGS --show-comments
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: invalid attribute
// DEFAULT: ╭─[tests/fixtures/parser_grammar_transactions.c:35:3]
// DEFAULT: 34 │ int invalid_attribute(void) {
// DEFAULT: 35 │   __attribute__((alloc_size(sizeof(enum { T = 2 }), +))) int value;
// DEFAULT: ·   ─────────────────────────────────────────────────────────────────
// DEFAULT: 36 │   T after_invalid_attribute;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
