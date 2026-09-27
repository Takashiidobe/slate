// SLATE-FILECHECK-ARGS --dump-ir

struct Placed {
  int value __attribute__((section("data")));
};

int size(void) { return sizeof(struct Placed); }

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × invalid in this context: 'section' attribute only applies to functions and
// SLATE-FILECHECK-END SEMANTIC
