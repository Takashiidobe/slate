/* { dg-skip-if "assumes absence of larger-than-word padding" { epiphany-*-* } }
 */
extern void abort(void);

typedef int word __attribute__((mode(word)));

struct foo {
  word x;
  word y[0];
};

int main() {
  if (sizeof(word) != sizeof(struct foo))
    abort();
  if (__alignof__(word) != __alignof__(struct foo))
    abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: machine mode attribute
// SLATE-FILECHECK-END DEFAULT
