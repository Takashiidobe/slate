typedef int aligned_type __attribute__((__aligned__(8 + 8)));
typedef int vector_type __attribute__((__vector_size__(sizeof(int) * 4)));
__attribute__((__const__, __may_alias__)) int aliased;

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: typedef alignment attribute
// SLATE-FILECHECK-END DEFAULT
