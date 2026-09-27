// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-ARGS -target=aarch64-unknown-linux-gnu -masm=intel
// SLATE-FILECHECK-ERROR PARSE

int value;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × all rules failed: all rules failed: GCC asm dialect: asm dialect is
// SLATE-FILECHECK-END PARSE
