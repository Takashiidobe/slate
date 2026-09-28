// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES MIXED MIXED
// SLATE-FILECHECK-DEFINES SELF SELF
// SLATE-FILECHECK-DEFINES NO_INIT NO_INIT
// SLATE-FILECHECK-DEFINES LIST LIST
// SLATE-FILECHECK-DEFINES PARAMS PARAMS
// SLATE-FILECHECK-DEFINES EXTENT EXTENT
// SLATE-FILECHECK-DEFINES TOP_ARRAY TOP_ARRAY
// SLATE-FILECHECK-DEFINES BIT_FIELD BIT_FIELD
// SLATE-FILECHECK-IR-ERROR MIXED
// SLATE-FILECHECK-IR-ERROR SELF
// SLATE-FILECHECK-IR-ERROR NO_INIT
// SLATE-FILECHECK-IR-ERROR LIST
// SLATE-FILECHECK-IR-ERROR PARAMS
// SLATE-FILECHECK-IR-ERROR EXTENT
// SLATE-FILECHECK-IR-ERROR TOP_ARRAY
// SLATE-FILECHECK-IR-ERROR BIT_FIELD
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

struct S { unsigned b : 3; };
_Atomic int ai;
const int ci = 1;
int arr[3];
const int carr[2];
int g(int);
long h(long);

auto file_scope = 1.5;
static auto internal = &file_scope;
_Static_assert(_Generic(&internal, double **: 1, default: 0));

void deduce(int n, int *ip, const int *cip, struct S s) {
  int vla[n];
  auto a1 = ci;
  _Static_assert(_Generic(&a1, int *: 1, default: 0));
  auto a2 = ai;
  _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
  auto a3 = carr;
  _Static_assert(_Generic(&a3, const int **: 1, default: 0));
  auto a4 = g;
  _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
  auto a5 = +s.b;
  _Static_assert(_Generic(&a5, int *: 1, default: 0));
  const auto a6 = 1;
  _Static_assert(_Generic(&a6, const int *: 1, default: 0));
  auto a7 = vla;
  auto a8 = &vla;
  auto a9 = "text";
  _Static_assert(_Generic(&a9, char **: 1, default: 0));
  static auto a10 = 2;

  const auto *p1 = ip;
  _Static_assert(_Generic(&p1, const int **: 1, default: 0));
  auto *p2 = cip;
  _Static_assert(_Generic(&p2, const int **: 1, default: 0));
  auto *const p3 = ip;
  auto **p4 = &ip;
  _Static_assert(_Generic(&p4, int ***: 1, default: 0));
  auto (*p5)(int) = g;
  auto (*p6)[3] = &arr;
  auto m1 = 1, *m2 = &m1;
  _Static_assert(_Generic(&m2, int **: 1, default: 0));
  const auto *q1 = ip, *q2 = cip;
  (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
  (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
  (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;

#ifdef MIXED
  auto x = 1, y = 2.0;
#endif
#ifdef SELF
  auto z = 1 + sizeof(z);
#endif
#ifdef NO_INIT
  auto w = 1, v;
#endif
#ifdef LIST
  auto l = {1};
#endif
#ifdef PARAMS
  auto (*f)(int) = h;
#endif
#ifdef EXTENT
  auto (*e)[4] = &arr;
#endif
#ifdef TOP_ARRAY
  auto t[3] = arr;
#endif
#ifdef BIT_FIELD
  auto b = s.b;
#endif
}

// SLATE-FILECHECK-BEGIN MIXED
// MIXED: Error:   × semantic analysis failed
// MIXED: Error:
// MIXED: × invalid in this context: 'auto' deduced as different types in one
// MIXED: ╭─[tests/fixtures/sema/c23_auto_inference.c:14:1]
// MIXED: 13 │
// MIXED: 14 │ ╭─▶ void deduce(int n, int *ip, const int *cip, struct S s) {
// MIXED: 15 │ │     int vla[n];
// MIXED: 16 │ │     auto a1 = ci;
// MIXED: 17 │ │     _Static_assert(_Generic(&a1, int *: 1, default: 0));
// MIXED: 18 │ │     auto a2 = ai;
// MIXED: 19 │ │     _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
// MIXED: 20 │ │     auto a3 = carr;
// MIXED: 21 │ │     _Static_assert(_Generic(&a3, const int **: 1, default: 0));
// MIXED: 22 │ │     auto a4 = g;
// MIXED: 23 │ │     _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
// MIXED: 24 │ │     auto a5 = +s.b;
// MIXED: 25 │ │     _Static_assert(_Generic(&a5, int *: 1, default: 0));
// MIXED: 26 │ │     const auto a6 = 1;
// MIXED: 27 │ │     _Static_assert(_Generic(&a6, const int *: 1, default: 0));
// MIXED: 28 │ │     auto a7 = vla;
// MIXED: 29 │ │     auto a8 = &vla;
// MIXED: 30 │ │     auto a9 = "text";
// MIXED: 31 │ │     _Static_assert(_Generic(&a9, char **: 1, default: 0));
// MIXED: 32 │ │     static auto a10 = 2;
// MIXED: 33 │ │
// MIXED: 34 │ │     const auto *p1 = ip;
// MIXED: 35 │ │     _Static_assert(_Generic(&p1, const int **: 1, default: 0));
// MIXED: 36 │ │     auto *p2 = cip;
// MIXED: 37 │ │     _Static_assert(_Generic(&p2, const int **: 1, default: 0));
// MIXED: 38 │ │     auto *const p3 = ip;
// MIXED: 39 │ │     auto **p4 = &ip;
// MIXED: 40 │ │     _Static_assert(_Generic(&p4, int ***: 1, default: 0));
// MIXED: 41 │ │     auto (*p5)(int) = g;
// MIXED: 42 │ │     auto (*p6)[3] = &arr;
// MIXED: 43 │ │     auto m1 = 1, *m2 = &m1;
// MIXED: 44 │ │     _Static_assert(_Generic(&m2, int **: 1, default: 0));
// MIXED: 45 │ │     const auto *q1 = ip, *q2 = cip;
// MIXED: 46 │ │     (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
// MIXED: 47 │ │     (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
// MIXED: 48 │ │     (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;
// MIXED: 49 │ │
// MIXED: 50 │ │   #ifdef MIXED
// MIXED: 51 │ │     auto x = 1, y = 2.0;
// MIXED: 52 │ │   #endif
// MIXED: 53 │ │   #ifdef SELF
// MIXED: 54 │ │     auto z = 1 + sizeof(z);
// MIXED: 55 │ │   #endif
// MIXED: 56 │ │   #ifdef NO_INIT
// MIXED: 57 │ │     auto w = 1, v;
// MIXED: 58 │ │   #endif
// MIXED: 59 │ │   #ifdef LIST
// MIXED: 60 │ │     auto l = {1};
// MIXED: 61 │ │   #endif
// MIXED: 62 │ │   #ifdef PARAMS
// MIXED: 63 │ │     auto (*f)(int) = h;
// MIXED: 64 │ │   #endif
// MIXED: 65 │ │   #ifdef EXTENT
// MIXED: 66 │ │     auto (*e)[4] = &arr;
// MIXED: 67 │ │   #endif
// MIXED: 68 │ │   #ifdef TOP_ARRAY
// MIXED: 69 │ │     auto t[3] = arr;
// MIXED: 70 │ │   #endif
// MIXED: 71 │ │   #ifdef BIT_FIELD
// MIXED: 72 │ │     auto b = s.b;
// MIXED: 73 │ │   #endif
// MIXED: 74 │ ╰─▶ }
// MIXED: 75 │
// MIXED: ╰────
// SLATE-FILECHECK-END MIXED
// SLATE-FILECHECK-BEGIN SELF
// SELF: Error:   × semantic analysis failed
// SELF: Error:
// SELF: × invalid in this context: variable declared with deduced type cannot appear
// SELF: ╭─[tests/fixtures/sema/c23_auto_inference.c:14:1]
// SELF: 13 │
// SELF: 14 │ ╭─▶ void deduce(int n, int *ip, const int *cip, struct S s) {
// SELF: 15 │ │     int vla[n];
// SELF: 16 │ │     auto a1 = ci;
// SELF: 17 │ │     _Static_assert(_Generic(&a1, int *: 1, default: 0));
// SELF: 18 │ │     auto a2 = ai;
// SELF: 19 │ │     _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
// SELF: 20 │ │     auto a3 = carr;
// SELF: 21 │ │     _Static_assert(_Generic(&a3, const int **: 1, default: 0));
// SELF: 22 │ │     auto a4 = g;
// SELF: 23 │ │     _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
// SELF: 24 │ │     auto a5 = +s.b;
// SELF: 25 │ │     _Static_assert(_Generic(&a5, int *: 1, default: 0));
// SELF: 26 │ │     const auto a6 = 1;
// SELF: 27 │ │     _Static_assert(_Generic(&a6, const int *: 1, default: 0));
// SELF: 28 │ │     auto a7 = vla;
// SELF: 29 │ │     auto a8 = &vla;
// SELF: 30 │ │     auto a9 = "text";
// SELF: 31 │ │     _Static_assert(_Generic(&a9, char **: 1, default: 0));
// SELF: 32 │ │     static auto a10 = 2;
// SELF: 33 │ │
// SELF: 34 │ │     const auto *p1 = ip;
// SELF: 35 │ │     _Static_assert(_Generic(&p1, const int **: 1, default: 0));
// SELF: 36 │ │     auto *p2 = cip;
// SELF: 37 │ │     _Static_assert(_Generic(&p2, const int **: 1, default: 0));
// SELF: 38 │ │     auto *const p3 = ip;
// SELF: 39 │ │     auto **p4 = &ip;
// SELF: 40 │ │     _Static_assert(_Generic(&p4, int ***: 1, default: 0));
// SELF: 41 │ │     auto (*p5)(int) = g;
// SELF: 42 │ │     auto (*p6)[3] = &arr;
// SELF: 43 │ │     auto m1 = 1, *m2 = &m1;
// SELF: 44 │ │     _Static_assert(_Generic(&m2, int **: 1, default: 0));
// SELF: 45 │ │     const auto *q1 = ip, *q2 = cip;
// SELF: 46 │ │     (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
// SELF: 47 │ │     (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
// SELF: 48 │ │     (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;
// SELF: 49 │ │
// SELF: 50 │ │   #ifdef MIXED
// SELF: 51 │ │     auto x = 1, y = 2.0;
// SELF: 52 │ │   #endif
// SELF: 53 │ │   #ifdef SELF
// SELF: 54 │ │     auto z = 1 + sizeof(z);
// SELF: 55 │ │   #endif
// SELF: 56 │ │   #ifdef NO_INIT
// SELF: 57 │ │     auto w = 1, v;
// SELF: 58 │ │   #endif
// SELF: 59 │ │   #ifdef LIST
// SELF: 60 │ │     auto l = {1};
// SELF: 61 │ │   #endif
// SELF: 62 │ │   #ifdef PARAMS
// SELF: 63 │ │     auto (*f)(int) = h;
// SELF: 64 │ │   #endif
// SELF: 65 │ │   #ifdef EXTENT
// SELF: 66 │ │     auto (*e)[4] = &arr;
// SELF: 67 │ │   #endif
// SELF: 68 │ │   #ifdef TOP_ARRAY
// SELF: 69 │ │     auto t[3] = arr;
// SELF: 70 │ │   #endif
// SELF: 71 │ │   #ifdef BIT_FIELD
// SELF: 72 │ │     auto b = s.b;
// SELF: 73 │ │   #endif
// SELF: 74 │ ╰─▶ }
// SELF: 75 │
// SELF: ╰────
// SLATE-FILECHECK-END SELF
// SLATE-FILECHECK-BEGIN NO_INIT
// NO_INIT: Error:   × semantic analysis failed
// NO_INIT: Error:
// NO_INIT: × invalid in this context: declaration with deduced type requires an
// NO_INIT: ╭─[tests/fixtures/sema/c23_auto_inference.c:14:1]
// NO_INIT: 13 │
// NO_INIT: 14 │ ╭─▶ void deduce(int n, int *ip, const int *cip, struct S s) {
// NO_INIT: 15 │ │     int vla[n];
// NO_INIT: 16 │ │     auto a1 = ci;
// NO_INIT: 17 │ │     _Static_assert(_Generic(&a1, int *: 1, default: 0));
// NO_INIT: 18 │ │     auto a2 = ai;
// NO_INIT: 19 │ │     _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
// NO_INIT: 20 │ │     auto a3 = carr;
// NO_INIT: 21 │ │     _Static_assert(_Generic(&a3, const int **: 1, default: 0));
// NO_INIT: 22 │ │     auto a4 = g;
// NO_INIT: 23 │ │     _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
// NO_INIT: 24 │ │     auto a5 = +s.b;
// NO_INIT: 25 │ │     _Static_assert(_Generic(&a5, int *: 1, default: 0));
// NO_INIT: 26 │ │     const auto a6 = 1;
// NO_INIT: 27 │ │     _Static_assert(_Generic(&a6, const int *: 1, default: 0));
// NO_INIT: 28 │ │     auto a7 = vla;
// NO_INIT: 29 │ │     auto a8 = &vla;
// NO_INIT: 30 │ │     auto a9 = "text";
// NO_INIT: 31 │ │     _Static_assert(_Generic(&a9, char **: 1, default: 0));
// NO_INIT: 32 │ │     static auto a10 = 2;
// NO_INIT: 33 │ │
// NO_INIT: 34 │ │     const auto *p1 = ip;
// NO_INIT: 35 │ │     _Static_assert(_Generic(&p1, const int **: 1, default: 0));
// NO_INIT: 36 │ │     auto *p2 = cip;
// NO_INIT: 37 │ │     _Static_assert(_Generic(&p2, const int **: 1, default: 0));
// NO_INIT: 38 │ │     auto *const p3 = ip;
// NO_INIT: 39 │ │     auto **p4 = &ip;
// NO_INIT: 40 │ │     _Static_assert(_Generic(&p4, int ***: 1, default: 0));
// NO_INIT: 41 │ │     auto (*p5)(int) = g;
// NO_INIT: 42 │ │     auto (*p6)[3] = &arr;
// NO_INIT: 43 │ │     auto m1 = 1, *m2 = &m1;
// NO_INIT: 44 │ │     _Static_assert(_Generic(&m2, int **: 1, default: 0));
// NO_INIT: 45 │ │     const auto *q1 = ip, *q2 = cip;
// NO_INIT: 46 │ │     (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
// NO_INIT: 47 │ │     (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
// NO_INIT: 48 │ │     (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;
// NO_INIT: 49 │ │
// NO_INIT: 50 │ │   #ifdef MIXED
// NO_INIT: 51 │ │     auto x = 1, y = 2.0;
// NO_INIT: 52 │ │   #endif
// NO_INIT: 53 │ │   #ifdef SELF
// NO_INIT: 54 │ │     auto z = 1 + sizeof(z);
// NO_INIT: 55 │ │   #endif
// NO_INIT: 56 │ │   #ifdef NO_INIT
// NO_INIT: 57 │ │     auto w = 1, v;
// NO_INIT: 58 │ │   #endif
// NO_INIT: 59 │ │   #ifdef LIST
// NO_INIT: 60 │ │     auto l = {1};
// NO_INIT: 61 │ │   #endif
// NO_INIT: 62 │ │   #ifdef PARAMS
// NO_INIT: 63 │ │     auto (*f)(int) = h;
// NO_INIT: 64 │ │   #endif
// NO_INIT: 65 │ │   #ifdef EXTENT
// NO_INIT: 66 │ │     auto (*e)[4] = &arr;
// NO_INIT: 67 │ │   #endif
// NO_INIT: 68 │ │   #ifdef TOP_ARRAY
// NO_INIT: 69 │ │     auto t[3] = arr;
// NO_INIT: 70 │ │   #endif
// NO_INIT: 71 │ │   #ifdef BIT_FIELD
// NO_INIT: 72 │ │     auto b = s.b;
// NO_INIT: 73 │ │   #endif
// NO_INIT: 74 │ ╰─▶ }
// NO_INIT: 75 │
// NO_INIT: ╰────
// SLATE-FILECHECK-END NO_INIT
// SLATE-FILECHECK-BEGIN LIST
// LIST: Error:   × semantic analysis failed
// LIST: Error:
// LIST: × invalid in this context: cannot use 'auto' with an initializer list
// LIST: ╭─[tests/fixtures/sema/c23_auto_inference.c:14:1]
// LIST: 13 │
// LIST: 14 │ ╭─▶ void deduce(int n, int *ip, const int *cip, struct S s) {
// LIST: 15 │ │     int vla[n];
// LIST: 16 │ │     auto a1 = ci;
// LIST: 17 │ │     _Static_assert(_Generic(&a1, int *: 1, default: 0));
// LIST: 18 │ │     auto a2 = ai;
// LIST: 19 │ │     _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
// LIST: 20 │ │     auto a3 = carr;
// LIST: 21 │ │     _Static_assert(_Generic(&a3, const int **: 1, default: 0));
// LIST: 22 │ │     auto a4 = g;
// LIST: 23 │ │     _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
// LIST: 24 │ │     auto a5 = +s.b;
// LIST: 25 │ │     _Static_assert(_Generic(&a5, int *: 1, default: 0));
// LIST: 26 │ │     const auto a6 = 1;
// LIST: 27 │ │     _Static_assert(_Generic(&a6, const int *: 1, default: 0));
// LIST: 28 │ │     auto a7 = vla;
// LIST: 29 │ │     auto a8 = &vla;
// LIST: 30 │ │     auto a9 = "text";
// LIST: 31 │ │     _Static_assert(_Generic(&a9, char **: 1, default: 0));
// LIST: 32 │ │     static auto a10 = 2;
// LIST: 33 │ │
// LIST: 34 │ │     const auto *p1 = ip;
// LIST: 35 │ │     _Static_assert(_Generic(&p1, const int **: 1, default: 0));
// LIST: 36 │ │     auto *p2 = cip;
// LIST: 37 │ │     _Static_assert(_Generic(&p2, const int **: 1, default: 0));
// LIST: 38 │ │     auto *const p3 = ip;
// LIST: 39 │ │     auto **p4 = &ip;
// LIST: 40 │ │     _Static_assert(_Generic(&p4, int ***: 1, default: 0));
// LIST: 41 │ │     auto (*p5)(int) = g;
// LIST: 42 │ │     auto (*p6)[3] = &arr;
// LIST: 43 │ │     auto m1 = 1, *m2 = &m1;
// LIST: 44 │ │     _Static_assert(_Generic(&m2, int **: 1, default: 0));
// LIST: 45 │ │     const auto *q1 = ip, *q2 = cip;
// LIST: 46 │ │     (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
// LIST: 47 │ │     (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
// LIST: 48 │ │     (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;
// LIST: 49 │ │
// LIST: 50 │ │   #ifdef MIXED
// LIST: 51 │ │     auto x = 1, y = 2.0;
// LIST: 52 │ │   #endif
// LIST: 53 │ │   #ifdef SELF
// LIST: 54 │ │     auto z = 1 + sizeof(z);
// LIST: 55 │ │   #endif
// LIST: 56 │ │   #ifdef NO_INIT
// LIST: 57 │ │     auto w = 1, v;
// LIST: 58 │ │   #endif
// LIST: 59 │ │   #ifdef LIST
// LIST: 60 │ │     auto l = {1};
// LIST: 61 │ │   #endif
// LIST: 62 │ │   #ifdef PARAMS
// LIST: 63 │ │     auto (*f)(int) = h;
// LIST: 64 │ │   #endif
// LIST: 65 │ │   #ifdef EXTENT
// LIST: 66 │ │     auto (*e)[4] = &arr;
// LIST: 67 │ │   #endif
// LIST: 68 │ │   #ifdef TOP_ARRAY
// LIST: 69 │ │     auto t[3] = arr;
// LIST: 70 │ │   #endif
// LIST: 71 │ │   #ifdef BIT_FIELD
// LIST: 72 │ │     auto b = s.b;
// LIST: 73 │ │   #endif
// LIST: 74 │ ╰─▶ }
// LIST: 75 │
// LIST: ╰────
// SLATE-FILECHECK-END LIST
// SLATE-FILECHECK-BEGIN PARAMS
// PARAMS: Error:   × semantic analysis failed
// PARAMS: Error:
// PARAMS: × invalid in this context: initializer does not match the deduced declarator
// PARAMS: ╭─[tests/fixtures/sema/c23_auto_inference.c:14:1]
// PARAMS: 13 │
// PARAMS: 14 │ ╭─▶ void deduce(int n, int *ip, const int *cip, struct S s) {
// PARAMS: 15 │ │     int vla[n];
// PARAMS: 16 │ │     auto a1 = ci;
// PARAMS: 17 │ │     _Static_assert(_Generic(&a1, int *: 1, default: 0));
// PARAMS: 18 │ │     auto a2 = ai;
// PARAMS: 19 │ │     _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
// PARAMS: 20 │ │     auto a3 = carr;
// PARAMS: 21 │ │     _Static_assert(_Generic(&a3, const int **: 1, default: 0));
// PARAMS: 22 │ │     auto a4 = g;
// PARAMS: 23 │ │     _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
// PARAMS: 24 │ │     auto a5 = +s.b;
// PARAMS: 25 │ │     _Static_assert(_Generic(&a5, int *: 1, default: 0));
// PARAMS: 26 │ │     const auto a6 = 1;
// PARAMS: 27 │ │     _Static_assert(_Generic(&a6, const int *: 1, default: 0));
// PARAMS: 28 │ │     auto a7 = vla;
// PARAMS: 29 │ │     auto a8 = &vla;
// PARAMS: 30 │ │     auto a9 = "text";
// PARAMS: 31 │ │     _Static_assert(_Generic(&a9, char **: 1, default: 0));
// PARAMS: 32 │ │     static auto a10 = 2;
// PARAMS: 33 │ │
// PARAMS: 34 │ │     const auto *p1 = ip;
// PARAMS: 35 │ │     _Static_assert(_Generic(&p1, const int **: 1, default: 0));
// PARAMS: 36 │ │     auto *p2 = cip;
// PARAMS: 37 │ │     _Static_assert(_Generic(&p2, const int **: 1, default: 0));
// PARAMS: 38 │ │     auto *const p3 = ip;
// PARAMS: 39 │ │     auto **p4 = &ip;
// PARAMS: 40 │ │     _Static_assert(_Generic(&p4, int ***: 1, default: 0));
// PARAMS: 41 │ │     auto (*p5)(int) = g;
// PARAMS: 42 │ │     auto (*p6)[3] = &arr;
// PARAMS: 43 │ │     auto m1 = 1, *m2 = &m1;
// PARAMS: 44 │ │     _Static_assert(_Generic(&m2, int **: 1, default: 0));
// PARAMS: 45 │ │     const auto *q1 = ip, *q2 = cip;
// PARAMS: 46 │ │     (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
// PARAMS: 47 │ │     (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
// PARAMS: 48 │ │     (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;
// PARAMS: 49 │ │
// PARAMS: 50 │ │   #ifdef MIXED
// PARAMS: 51 │ │     auto x = 1, y = 2.0;
// PARAMS: 52 │ │   #endif
// PARAMS: 53 │ │   #ifdef SELF
// PARAMS: 54 │ │     auto z = 1 + sizeof(z);
// PARAMS: 55 │ │   #endif
// PARAMS: 56 │ │   #ifdef NO_INIT
// PARAMS: 57 │ │     auto w = 1, v;
// PARAMS: 58 │ │   #endif
// PARAMS: 59 │ │   #ifdef LIST
// PARAMS: 60 │ │     auto l = {1};
// PARAMS: 61 │ │   #endif
// PARAMS: 62 │ │   #ifdef PARAMS
// PARAMS: 63 │ │     auto (*f)(int) = h;
// PARAMS: 64 │ │   #endif
// PARAMS: 65 │ │   #ifdef EXTENT
// PARAMS: 66 │ │     auto (*e)[4] = &arr;
// PARAMS: 67 │ │   #endif
// PARAMS: 68 │ │   #ifdef TOP_ARRAY
// PARAMS: 69 │ │     auto t[3] = arr;
// PARAMS: 70 │ │   #endif
// PARAMS: 71 │ │   #ifdef BIT_FIELD
// PARAMS: 72 │ │     auto b = s.b;
// PARAMS: 73 │ │   #endif
// PARAMS: 74 │ ╰─▶ }
// PARAMS: 75 │
// PARAMS: ╰────
// SLATE-FILECHECK-END PARAMS
// SLATE-FILECHECK-BEGIN EXTENT
// EXTENT: Error:   × semantic analysis failed
// EXTENT: Error:
// EXTENT: × invalid in this context: initializer does not match the deduced declarator
// EXTENT: ╭─[tests/fixtures/sema/c23_auto_inference.c:14:1]
// EXTENT: 13 │
// EXTENT: 14 │ ╭─▶ void deduce(int n, int *ip, const int *cip, struct S s) {
// EXTENT: 15 │ │     int vla[n];
// EXTENT: 16 │ │     auto a1 = ci;
// EXTENT: 17 │ │     _Static_assert(_Generic(&a1, int *: 1, default: 0));
// EXTENT: 18 │ │     auto a2 = ai;
// EXTENT: 19 │ │     _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
// EXTENT: 20 │ │     auto a3 = carr;
// EXTENT: 21 │ │     _Static_assert(_Generic(&a3, const int **: 1, default: 0));
// EXTENT: 22 │ │     auto a4 = g;
// EXTENT: 23 │ │     _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
// EXTENT: 24 │ │     auto a5 = +s.b;
// EXTENT: 25 │ │     _Static_assert(_Generic(&a5, int *: 1, default: 0));
// EXTENT: 26 │ │     const auto a6 = 1;
// EXTENT: 27 │ │     _Static_assert(_Generic(&a6, const int *: 1, default: 0));
// EXTENT: 28 │ │     auto a7 = vla;
// EXTENT: 29 │ │     auto a8 = &vla;
// EXTENT: 30 │ │     auto a9 = "text";
// EXTENT: 31 │ │     _Static_assert(_Generic(&a9, char **: 1, default: 0));
// EXTENT: 32 │ │     static auto a10 = 2;
// EXTENT: 33 │ │
// EXTENT: 34 │ │     const auto *p1 = ip;
// EXTENT: 35 │ │     _Static_assert(_Generic(&p1, const int **: 1, default: 0));
// EXTENT: 36 │ │     auto *p2 = cip;
// EXTENT: 37 │ │     _Static_assert(_Generic(&p2, const int **: 1, default: 0));
// EXTENT: 38 │ │     auto *const p3 = ip;
// EXTENT: 39 │ │     auto **p4 = &ip;
// EXTENT: 40 │ │     _Static_assert(_Generic(&p4, int ***: 1, default: 0));
// EXTENT: 41 │ │     auto (*p5)(int) = g;
// EXTENT: 42 │ │     auto (*p6)[3] = &arr;
// EXTENT: 43 │ │     auto m1 = 1, *m2 = &m1;
// EXTENT: 44 │ │     _Static_assert(_Generic(&m2, int **: 1, default: 0));
// EXTENT: 45 │ │     const auto *q1 = ip, *q2 = cip;
// EXTENT: 46 │ │     (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
// EXTENT: 47 │ │     (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
// EXTENT: 48 │ │     (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;
// EXTENT: 49 │ │
// EXTENT: 50 │ │   #ifdef MIXED
// EXTENT: 51 │ │     auto x = 1, y = 2.0;
// EXTENT: 52 │ │   #endif
// EXTENT: 53 │ │   #ifdef SELF
// EXTENT: 54 │ │     auto z = 1 + sizeof(z);
// EXTENT: 55 │ │   #endif
// EXTENT: 56 │ │   #ifdef NO_INIT
// EXTENT: 57 │ │     auto w = 1, v;
// EXTENT: 58 │ │   #endif
// EXTENT: 59 │ │   #ifdef LIST
// EXTENT: 60 │ │     auto l = {1};
// EXTENT: 61 │ │   #endif
// EXTENT: 62 │ │   #ifdef PARAMS
// EXTENT: 63 │ │     auto (*f)(int) = h;
// EXTENT: 64 │ │   #endif
// EXTENT: 65 │ │   #ifdef EXTENT
// EXTENT: 66 │ │     auto (*e)[4] = &arr;
// EXTENT: 67 │ │   #endif
// EXTENT: 68 │ │   #ifdef TOP_ARRAY
// EXTENT: 69 │ │     auto t[3] = arr;
// EXTENT: 70 │ │   #endif
// EXTENT: 71 │ │   #ifdef BIT_FIELD
// EXTENT: 72 │ │     auto b = s.b;
// EXTENT: 73 │ │   #endif
// EXTENT: 74 │ ╰─▶ }
// EXTENT: 75 │
// EXTENT: ╰────
// SLATE-FILECHECK-END EXTENT
// SLATE-FILECHECK-BEGIN TOP_ARRAY
// TOP_ARRAY: Error:   × semantic analysis failed
// TOP_ARRAY: Error:
// TOP_ARRAY: × invalid in this context: initializer does not match the deduced declarator
// TOP_ARRAY: ╭─[tests/fixtures/sema/c23_auto_inference.c:14:1]
// TOP_ARRAY: 13 │
// TOP_ARRAY: 14 │ ╭─▶ void deduce(int n, int *ip, const int *cip, struct S s) {
// TOP_ARRAY: 15 │ │     int vla[n];
// TOP_ARRAY: 16 │ │     auto a1 = ci;
// TOP_ARRAY: 17 │ │     _Static_assert(_Generic(&a1, int *: 1, default: 0));
// TOP_ARRAY: 18 │ │     auto a2 = ai;
// TOP_ARRAY: 19 │ │     _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
// TOP_ARRAY: 20 │ │     auto a3 = carr;
// TOP_ARRAY: 21 │ │     _Static_assert(_Generic(&a3, const int **: 1, default: 0));
// TOP_ARRAY: 22 │ │     auto a4 = g;
// TOP_ARRAY: 23 │ │     _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
// TOP_ARRAY: 24 │ │     auto a5 = +s.b;
// TOP_ARRAY: 25 │ │     _Static_assert(_Generic(&a5, int *: 1, default: 0));
// TOP_ARRAY: 26 │ │     const auto a6 = 1;
// TOP_ARRAY: 27 │ │     _Static_assert(_Generic(&a6, const int *: 1, default: 0));
// TOP_ARRAY: 28 │ │     auto a7 = vla;
// TOP_ARRAY: 29 │ │     auto a8 = &vla;
// TOP_ARRAY: 30 │ │     auto a9 = "text";
// TOP_ARRAY: 31 │ │     _Static_assert(_Generic(&a9, char **: 1, default: 0));
// TOP_ARRAY: 32 │ │     static auto a10 = 2;
// TOP_ARRAY: 33 │ │
// TOP_ARRAY: 34 │ │     const auto *p1 = ip;
// TOP_ARRAY: 35 │ │     _Static_assert(_Generic(&p1, const int **: 1, default: 0));
// TOP_ARRAY: 36 │ │     auto *p2 = cip;
// TOP_ARRAY: 37 │ │     _Static_assert(_Generic(&p2, const int **: 1, default: 0));
// TOP_ARRAY: 38 │ │     auto *const p3 = ip;
// TOP_ARRAY: 39 │ │     auto **p4 = &ip;
// TOP_ARRAY: 40 │ │     _Static_assert(_Generic(&p4, int ***: 1, default: 0));
// TOP_ARRAY: 41 │ │     auto (*p5)(int) = g;
// TOP_ARRAY: 42 │ │     auto (*p6)[3] = &arr;
// TOP_ARRAY: 43 │ │     auto m1 = 1, *m2 = &m1;
// TOP_ARRAY: 44 │ │     _Static_assert(_Generic(&m2, int **: 1, default: 0));
// TOP_ARRAY: 45 │ │     const auto *q1 = ip, *q2 = cip;
// TOP_ARRAY: 46 │ │     (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
// TOP_ARRAY: 47 │ │     (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
// TOP_ARRAY: 48 │ │     (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;
// TOP_ARRAY: 49 │ │
// TOP_ARRAY: 50 │ │   #ifdef MIXED
// TOP_ARRAY: 51 │ │     auto x = 1, y = 2.0;
// TOP_ARRAY: 52 │ │   #endif
// TOP_ARRAY: 53 │ │   #ifdef SELF
// TOP_ARRAY: 54 │ │     auto z = 1 + sizeof(z);
// TOP_ARRAY: 55 │ │   #endif
// TOP_ARRAY: 56 │ │   #ifdef NO_INIT
// TOP_ARRAY: 57 │ │     auto w = 1, v;
// TOP_ARRAY: 58 │ │   #endif
// TOP_ARRAY: 59 │ │   #ifdef LIST
// TOP_ARRAY: 60 │ │     auto l = {1};
// TOP_ARRAY: 61 │ │   #endif
// TOP_ARRAY: 62 │ │   #ifdef PARAMS
// TOP_ARRAY: 63 │ │     auto (*f)(int) = h;
// TOP_ARRAY: 64 │ │   #endif
// TOP_ARRAY: 65 │ │   #ifdef EXTENT
// TOP_ARRAY: 66 │ │     auto (*e)[4] = &arr;
// TOP_ARRAY: 67 │ │   #endif
// TOP_ARRAY: 68 │ │   #ifdef TOP_ARRAY
// TOP_ARRAY: 69 │ │     auto t[3] = arr;
// TOP_ARRAY: 70 │ │   #endif
// TOP_ARRAY: 71 │ │   #ifdef BIT_FIELD
// TOP_ARRAY: 72 │ │     auto b = s.b;
// TOP_ARRAY: 73 │ │   #endif
// TOP_ARRAY: 74 │ ╰─▶ }
// TOP_ARRAY: 75 │
// TOP_ARRAY: ╰────
// SLATE-FILECHECK-END TOP_ARRAY
// SLATE-FILECHECK-BEGIN BIT_FIELD
// BIT_FIELD: Error:   × semantic analysis failed
// BIT_FIELD: Error:
// BIT_FIELD: × invalid in this context: cannot use a bit-field as a deduced-type
// BIT_FIELD: ╭─[tests/fixtures/sema/c23_auto_inference.c:14:1]
// BIT_FIELD: 13 │
// BIT_FIELD: 14 │ ╭─▶ void deduce(int n, int *ip, const int *cip, struct S s) {
// BIT_FIELD: 15 │ │     int vla[n];
// BIT_FIELD: 16 │ │     auto a1 = ci;
// BIT_FIELD: 17 │ │     _Static_assert(_Generic(&a1, int *: 1, default: 0));
// BIT_FIELD: 18 │ │     auto a2 = ai;
// BIT_FIELD: 19 │ │     _Static_assert(_Generic(&a2, _Atomic int *: 1, default: 0));
// BIT_FIELD: 20 │ │     auto a3 = carr;
// BIT_FIELD: 21 │ │     _Static_assert(_Generic(&a3, const int **: 1, default: 0));
// BIT_FIELD: 22 │ │     auto a4 = g;
// BIT_FIELD: 23 │ │     _Static_assert(_Generic(&a4, int (**)(int): 1, default: 0));
// BIT_FIELD: 24 │ │     auto a5 = +s.b;
// BIT_FIELD: 25 │ │     _Static_assert(_Generic(&a5, int *: 1, default: 0));
// BIT_FIELD: 26 │ │     const auto a6 = 1;
// BIT_FIELD: 27 │ │     _Static_assert(_Generic(&a6, const int *: 1, default: 0));
// BIT_FIELD: 28 │ │     auto a7 = vla;
// BIT_FIELD: 29 │ │     auto a8 = &vla;
// BIT_FIELD: 30 │ │     auto a9 = "text";
// BIT_FIELD: 31 │ │     _Static_assert(_Generic(&a9, char **: 1, default: 0));
// BIT_FIELD: 32 │ │     static auto a10 = 2;
// BIT_FIELD: 33 │ │
// BIT_FIELD: 34 │ │     const auto *p1 = ip;
// BIT_FIELD: 35 │ │     _Static_assert(_Generic(&p1, const int **: 1, default: 0));
// BIT_FIELD: 36 │ │     auto *p2 = cip;
// BIT_FIELD: 37 │ │     _Static_assert(_Generic(&p2, const int **: 1, default: 0));
// BIT_FIELD: 38 │ │     auto *const p3 = ip;
// BIT_FIELD: 39 │ │     auto **p4 = &ip;
// BIT_FIELD: 40 │ │     _Static_assert(_Generic(&p4, int ***: 1, default: 0));
// BIT_FIELD: 41 │ │     auto (*p5)(int) = g;
// BIT_FIELD: 42 │ │     auto (*p6)[3] = &arr;
// BIT_FIELD: 43 │ │     auto m1 = 1, *m2 = &m1;
// BIT_FIELD: 44 │ │     _Static_assert(_Generic(&m2, int **: 1, default: 0));
// BIT_FIELD: 45 │ │     const auto *q1 = ip, *q2 = cip;
// BIT_FIELD: 46 │ │     (void)a1, (void)a2, (void)a3, (void)a4, (void)a5, (void)a6, (void)a7;
// BIT_FIELD: 47 │ │     (void)a8, (void)a9, (void)a10, (void)p1, (void)p2, (void)p3, (void)p4;
// BIT_FIELD: 48 │ │     (void)p5, (void)p6, (void)m2, (void)q1, (void)q2;
// BIT_FIELD: 49 │ │
// BIT_FIELD: 50 │ │   #ifdef MIXED
// BIT_FIELD: 51 │ │     auto x = 1, y = 2.0;
// BIT_FIELD: 52 │ │   #endif
// BIT_FIELD: 53 │ │   #ifdef SELF
// BIT_FIELD: 54 │ │     auto z = 1 + sizeof(z);
// BIT_FIELD: 55 │ │   #endif
// BIT_FIELD: 56 │ │   #ifdef NO_INIT
// BIT_FIELD: 57 │ │     auto w = 1, v;
// BIT_FIELD: 58 │ │   #endif
// BIT_FIELD: 59 │ │   #ifdef LIST
// BIT_FIELD: 60 │ │     auto l = {1};
// BIT_FIELD: 61 │ │   #endif
// BIT_FIELD: 62 │ │   #ifdef PARAMS
// BIT_FIELD: 63 │ │     auto (*f)(int) = h;
// BIT_FIELD: 64 │ │   #endif
// BIT_FIELD: 65 │ │   #ifdef EXTENT
// BIT_FIELD: 66 │ │     auto (*e)[4] = &arr;
// BIT_FIELD: 67 │ │   #endif
// BIT_FIELD: 68 │ │   #ifdef TOP_ARRAY
// BIT_FIELD: 69 │ │     auto t[3] = arr;
// BIT_FIELD: 70 │ │   #endif
// BIT_FIELD: 71 │ │   #ifdef BIT_FIELD
// BIT_FIELD: 72 │ │     auto b = s.b;
// BIT_FIELD: 73 │ │   #endif
// BIT_FIELD: 74 │ ╰─▶ }
// BIT_FIELD: 75 │
// BIT_FIELD: ╰────
// SLATE-FILECHECK-END BIT_FIELD
// SLATE-FILECHECK-BEGIN VALID
// VALID: module {
// VALID-NEXT:     target "x86_64-unknown-linux-gnu" {
// VALID-NEXT:         endian = little;
// VALID-NEXT:         pointer [size=8, align=8];
// VALID-NEXT:         stack_alignment = 16;
// VALID-NEXT:         long_double = f80;
// VALID-NEXT:         storage bool [size=1, align=1];
// VALID-NEXT:         storage i8, u8 [size=1, align=1];
// VALID-NEXT:         storage i16, u16 [size=2, align=2];
// VALID-NEXT:         storage i32, u32 [size=4, align=4];
// VALID-NEXT:         storage i64, u64 [size=8, align=8];
// VALID-NEXT:         storage i128, u128 [size=16, align=16];
// VALID-NEXT:         storage bf16 [size=2, align=2];
// VALID-NEXT:         storage f16 [size=2, align=2];
// VALID-NEXT:         storage f32 [size=4, align=4];
// VALID-NEXT:         storage f64 [size=8, align=8];
// VALID-NEXT:         storage f80 [size=16, align=16];
// VALID-NEXT:         storage f128 [size=16, align=16];
// VALID-NEXT:         storage d32 [size=4, align=4];
// VALID-NEXT:         storage d64 [size=8, align=8];
// VALID-NEXT:         storage d128 [size=16, align=16];
// VALID-NEXT:     }
// VALID-NEXT:     type @type0 S = struct {
// VALID-NEXT:         field0 b: u32 : 3;
// VALID-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// VALID-NEXT:     global %1 ai: atomic i32 [storage=static] [linkage=external];
// VALID-NEXT:     global %2 ci: i32 [storage=static] [const] = const<i32>(1) [linkage=external];
// VALID-NEXT:     global %3 arr: array<i32, 3> [storage=static] [linkage=external];
// VALID-NEXT:     global %4 carr: array<i32, 2> [storage=static] [const] [linkage=external];
// VALID-NEXT:     global %7 file_scope: f64 [storage=static] = const<f64>(1.5) [linkage=external];
// VALID-NEXT:     global %8 internal: ptr<f64> [storage=static] = addr_of<ptr<f64>>(%7) [linkage=internal];
// VALID-NEXT:     global %38 .str38: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 120, 116, 0]) [linkage=internal];
// VALID-NEXT:     global %24 a10: i32 [storage=static] = const<i32>(2) [linkage=internal];
// VALID-NEXT:     fn %5 @g(%35 <unnamed>: i32) -> i32 [linkage=external];
// VALID-NEXT:     fn %6 @h(%36 <unnamed>: i64) -> i64 [linkage=external];
// VALID-NEXT:     fn %9 @deduce(%10 n: i32, %11 ip: ptr<i32>, %12 cip: ptr<const i32>, %13 s: @type0) -> void [linkage=external] [abi=sysv64(scalar, scalar, scalar, coerce<i32>) -> void] [fallthrough=ret_void] {
// VALID-NEXT:         let %37: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%10)));
// VALID-NEXT:         let %14 vla: vla<i32, %37> [storage=automatic];
// VALID-NEXT:         let %15 a1: i32 [storage=automatic] = read<i32>(%2);
// VALID-NEXT:         let %16 a2: atomic i32 [storage=automatic] = read<i32, atomic=seq_cst>(%1);
// VALID-NEXT:         let %17 a3: ptr<const i32> [storage=automatic] = array_decay<ptr<const i32>, length=Some(2)>(%4);
// VALID-NEXT:         let %18 a4: ptr<fn(i32) -> i32> [storage=automatic] = function_decay<ptr<fn(i32) -> i32>>(%5);
// VALID-NEXT:         let %19 a5: i32 [storage=automatic] = reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%13)));
// VALID-NEXT:         let %20 a6: i32 [storage=automatic] [const] = const<i32>(1);
// VALID-NEXT:         let %21 a7: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=None>(%14);
// VALID-NEXT:         let %22 a8: ptr<vla<i32, %37>> [storage=automatic] = addr_of<ptr<vla<i32, %37>>>(%14);
// VALID-NEXT:         let %23 a9: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(5)>(%38);
// VALID-NEXT:         let %25 p1: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(%11));
// VALID-NEXT:         let %26 p2: ptr<const i32> [storage=automatic] = read<ptr<const i32>>(%12);
// VALID-NEXT:         let %27 p3: ptr<i32> [storage=automatic] [const] = read<ptr<i32>>(%11);
// VALID-NEXT:         let %28 p4: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%11);
// VALID-NEXT:         let %29 p5: ptr<fn(i32) -> i32> [storage=automatic] = function_decay<ptr<fn(i32) -> i32>>(%5);
// VALID-NEXT:         let %30 p6: ptr<array<i32, 3>> [storage=automatic] = addr_of<ptr<array<i32, 3>>>(%3);
// VALID-NEXT:         let %31 m1: i32 [storage=automatic] = const<i32>(1);
// VALID-NEXT:         let %32 m2: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%31);
// VALID-NEXT:         let %33 q1: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(%11));
// VALID-NEXT:         let %34 q2: ptr<const i32> [storage=automatic] = read<ptr<const i32>>(%12);
// VALID-NEXT:         read<i32>(%15);
// VALID-NEXT:         read<i32, atomic=seq_cst>(%16);
// VALID-NEXT:         read<ptr<const i32>>(%17);
// VALID-NEXT:         read<ptr<fn(i32) -> i32>>(%18);
// VALID-NEXT:         read<i32>(%19);
// VALID-NEXT:         read<i32>(%20);
// VALID-NEXT:         read<ptr<i32>>(%21);
// VALID-NEXT:         read<ptr<vla<i32, %37>>>(%22);
// VALID-NEXT:         read<ptr<i8>>(%23);
// VALID-NEXT:         read<i32>(%24);
// VALID-NEXT:         read<ptr<const i32>>(%25);
// VALID-NEXT:         read<ptr<const i32>>(%26);
// VALID-NEXT:         read<ptr<i32>>(%27);
// VALID-NEXT:         read<ptr<ptr<i32>>>(%28);
// VALID-NEXT:         read<ptr<fn(i32) -> i32>>(%29);
// VALID-NEXT:         read<ptr<array<i32, 3>>>(%30);
// VALID-NEXT:         read<ptr<i32>>(%32);
// VALID-NEXT:         read<ptr<const i32>>(%33);
// VALID-NEXT:         read<ptr<const i32>>(%34);
// VALID-NEXT:     }
// VALID-NEXT: }
// SLATE-FILECHECK-END VALID
