#ifdef _WIN32
#define ODR_INLINE inline
#define ODR_EXTERN extern
#define ODR_EXPORT __declspec(dllexport)
#define ODR_DEFINE_HERE
#else
#define ODR_INLINE
#define ODR_EXTERN
#define ODR_EXPORT
#endif

#ifdef ODR_DEFINE_HERE
ODR_INLINE int next_ticket(void) {
  static int count;
  return ++count;
}

ODR_EXTERN ODR_INLINE int twice_counted(int x) {
  static int calls;
  return x * 2 + 100 * ++calls;
}

ODR_EXPORT ODR_INLINE int thrice_counted(int x) {
  static int calls;
  return x * 3 + 100 * ++calls;
}
#else
int next_ticket(void);
int twice_counted(int x);
int thrice_counted(int x);
#endif

int other_unit(void);
