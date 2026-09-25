#define OPTIONAL(base, ...) base __VA_OPT__(+ __VA_ARGS__)
#define WRAP(...) OPTIONAL(1, __VA_ARGS__)
#define STRINGIFY(value) #value
#define TEXT(...) __VA_OPT__(STRINGIFY(__VA_ARGS__))
#define SELF SELF
#define RESCAN(...) __VA_OPT__(SELF)

int omitted = OPTIONAL(1);
int empty = OPTIONAL(1,);
int one = OPTIONAL(1, 2);
int nested_empty = WRAP();
int nested_value = WRAP(3);
char *text_empty = TEXT() "empty";
char *text_many = TEXT(alpha, beta);
int recursive __attribute__((slate_macro(RESCAN(value))));

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unresolved ordinary name `STRINGIFY`
// SLATE-FILECHECK-END DEFAULT
