#ifndef __STDARG_H
#define __STDARG_H

typedef struct {
  unsigned int gp_offset;
  unsigned int fp_offset;
  void *overflow_arg_area;
  void *reg_save_area;
} __va_elem;

typedef __va_elem va_list[1];

#define va_start(ap, ...)                                                      \
  do {                                                                         \
    *(ap) = *(__va_elem *)__va_area__;                                         \
  } while (0)

#define va_end(ap)

typedef va_list __builtin_va_list;
#define __builtin_va_start(ap, last) va_start(ap, last)
#define __builtin_va_end(ap) va_end(ap)
#define __builtin_va_copy(dest, src) va_copy(dest, src)

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST 1
typedef __builtin_va_list __gnuc_va_list;
#endif

static void *__va_arg_mem(__va_elem *ap, int sz, int align) {
  unsigned long p = (unsigned long)ap->overflow_arg_area;
  if (align > 8)
    p = (p + 15) / 16 * 16;
  ap->overflow_arg_area = (void *)(((p + sz + 7) / 8) * 8);
  return (void *)p;
}

static void *__va_arg_gp(__va_elem *ap, int sz, int align) {
  if (ap->gp_offset >= 48)
    return __va_arg_mem(ap, sz, align);

  void *r = (char *)ap->reg_save_area + ap->gp_offset;
  ap->gp_offset += 8;
  return r;
}

static void *__va_arg_fp(__va_elem *ap, int sz, int align) {
  if (ap->fp_offset >= 112)
    return __va_arg_mem(ap, sz, align);

  void *r = (char *)ap->reg_save_area + ap->fp_offset;
  ap->fp_offset += 8;
  return r;
}

#define va_arg(ap, ty)                                                         \
  ({                                                                           \
    int klass = __builtin_reg_class(ty);                                       \
    *(ty *)(klass == 0   ? __va_arg_gp(ap, sizeof(ty), _Alignof(ty))           \
            : klass == 1 ? __va_arg_fp(ap, sizeof(ty), _Alignof(ty))           \
                         : __va_arg_mem(ap, sizeof(ty), _Alignof(ty)));        \
  })

#define va_copy(dest, src) ((dest)[0] = (src)[0])

#endif
