/* { dg-require-effective-target indirect_jumps } */

// SLATE-FILECHECK-DEFINES DEFAULT

typedef long unsigned int size_t;
extern void               abort(void);
extern char              *strcpy(char *, const char *);
extern int                strcmp(const char *, const char *);
typedef __builtin_va_list va_list;
static const char         null[] = "(null)";
int                       g(char *s, const char *format, va_list ap) {
  const char        *f;
  const char        *string;
  char               spec;
  static const void *step0_jumps[] = {
      &&do_precision,
      &&do_form_integer,
      &&do_form_string,
  };
  f = format;
  if (*f == '\0')
    goto all_done;
  do {
    spec = (*++f);
    goto *(step0_jumps[2]);

  /* begin switch table. */
  do_precision:
    ++f;
    __builtin_va_arg(ap, int);
    spec = *f;
    goto *(step0_jumps[2]);

  do_form_integer:
    __builtin_va_arg(ap, unsigned long int);
    goto end;

  do_form_string:
    string = __builtin_va_arg(ap, const char *);
    strcpy(s, string);

  /* End of switch table. */
  end:
    ++f;
  } while (*f != '\0');

all_done:
  return 0;
}

void f(char *s, const char *f, ...) {
  va_list ap;
  __builtin_va_start(ap, f);
  g(s, f, ap);
  __builtin_va_end(ap);
}

int main(void) {
  char buf[10];
  f(buf, "%s", "asdf", 0);
  if (strcmp(buf, "asdf"))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     global %5 null: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([40, 110, 117, 108, 108, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 step0_jumps: array<ptr<const void>, 3> [storage=static] [align=16] = aggregate<array<ptr<const void>, 3>, zero_fill=false>(index0 = pointer_cast<ptr<const void>, reason=assign>(label_addr<ptr<void>>(%7)), index1 = pointer_cast<ptr<const void>, reason=assign>(label_addr<ptr<void>>(%8)), index2 = pointer_cast<ptr<const void>, reason=assign>(label_addr<ptr<void>>(%9))) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 115, 100, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 115, 100, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @strcpy(%25 <unnamed>: ptr<i8>, %26 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %3 @strcmp(%27 <unnamed>: ptr<const i8>, %28 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @g(%12 s: ptr<i8>, %13 format: ptr<const i8>, %14 ap: va_list) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 f: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %16 string: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %17 spec: i8 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<const i8>>(%15, read<ptr<const i8>>(%13));
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%15)))), const<i32>(0))
// DEFAULT-NEXT:             goto %11;
// DEFAULT-NEXT:         do %29
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %33: ptr<const i8> [synthetic] = read<ptr<const i8>>(%15);
// DEFAULT-NEXT:                 let %34: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%15, read<ptr<const i8>>(%34));
// DEFAULT-NEXT:                 write<i8>(%17, read<i8>(deref(read<ptr<const i8>>(%34))));
// DEFAULT-NEXT:                 goto *read<ptr<const void>>(deref(ptr_offset<ptr<ptr<const void>>, subtract=false, element=ptr<const void>, overflow=ub>(array_decay<ptr<ptr<const void>>, length=Some(3)>(%18), const<i32>(2))));
// DEFAULT-NEXT:                 label %7 do_precision:
// DEFAULT-NEXT:                     let %35: ptr<const i8> [synthetic] = read<ptr<const i8>>(%15);
// DEFAULT-NEXT:                     let %36: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%35), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<const i8>>(%15, read<ptr<const i8>>(%36));
// DEFAULT-NEXT:                 va_arg<i32>(%14);
// DEFAULT-NEXT:                 write<i8>(%17, read<i8>(deref(read<ptr<const i8>>(%15))));
// DEFAULT-NEXT:                 goto *read<ptr<const void>>(deref(ptr_offset<ptr<ptr<const void>>, subtract=false, element=ptr<const void>, overflow=ub>(array_decay<ptr<ptr<const void>>, length=Some(3)>(%18), const<i32>(2))));
// DEFAULT-NEXT:                 label %8 do_form_integer:
// DEFAULT-NEXT:                     va_arg<u64>(%14);
// DEFAULT-NEXT:                 goto %10;
// DEFAULT-NEXT:                 label %9 do_form_string:
// DEFAULT-NEXT:                     write<ptr<const i8>>(%16, va_arg<ptr<const i8>>(%14));
// DEFAULT-NEXT:                     va_arg<ptr<const i8>>(%14);
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%2, read<ptr<i8>>(%12), read<ptr<const i8>>(%16));
// DEFAULT-NEXT:                 label %10 end:
// DEFAULT-NEXT:                     let %37: ptr<const i8> [synthetic] = read<ptr<const i8>>(%15);
// DEFAULT-NEXT:                     let %38: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%37), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<const i8>>(%15, read<ptr<const i8>>(%38));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%15)))), const<i32>(0));
// DEFAULT-NEXT:         label %11 all_done:
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @f(%20 s: ptr<i8>, %21 f: ptr<const i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %22 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%22);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, va_list) -> i32>(%6, read<ptr<i8>>(%20), read<ptr<const i8>>(%21), read<va_list>(%22));
// DEFAULT-NEXT:         va_end(%22);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %24 buf: array<i8, 10> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<const i8>, ...) -> void>(%19, array_decay<ptr<i8>, length=Some(10)>(%24), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%30)), array_decay<ptr<i8>, length=Some(5)>(%31), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%24)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%32))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
