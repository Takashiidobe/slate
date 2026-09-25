void abort(void);

typedef int new_int __attribute__((aligned(16)));
struct S {
  int x;
};

int main() {
  if (sizeof(struct S) != sizeof(int))
    abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: typedef alignment attribute
// SLATE-FILECHECK-END DEFAULT
