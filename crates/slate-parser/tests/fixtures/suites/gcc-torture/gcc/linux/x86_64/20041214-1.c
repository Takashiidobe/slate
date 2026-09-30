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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     global %[[VALUE_null:[0-9]+]] null: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([40, 110, 117, 108, 108, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_step0_jumps:[0-9]+]] step0_jumps: array<ptr<const void>, 3> [storage=static] [align=16] = aggregate<array<ptr<const void>, 3>, zero_fill=false>(index0 = pointer_cast<ptr<const void>, reason=assign>(label_addr<ptr<void>>(%[[VALUE_do_precision:[0-9]+]])), index1 = pointer_cast<ptr<const void>, reason=assign>(label_addr<ptr<void>>(%[[VALUE_do_form_integer:[0-9]+]])), index2 = pointer_cast<ptr<const void>, reason=assign>(label_addr<ptr<void>>(%[[VALUE_do_form_string:[0-9]+]]))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 115, 100, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 115, 100, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_s:[0-9]+]] s: ptr<i8>, %[[VALUE_format:[0-9]+]] format: ptr<const i8>, %[[VALUE_ap:[0-9]+]] ap: va_list) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_string:[0-9]+]] string: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_spec:[0-9]+]] spec: i8 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<const i8>>(%[[VALUE_f]], read<ptr<const i8>>(%[[VALUE_format]]));
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_f]])))), const<i32>(0))
// DEFAULT-NEXT:             goto %[[VALUE_all_done:[0-9]+]];
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_f]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%[[VALUE_f]], read<ptr<const i8>>(%[[VALUE6]]));
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_spec]], read<i8>(deref(read<ptr<const i8>>(%[[VALUE6]]))));
// DEFAULT-NEXT:                 goto *read<ptr<const void>>(deref(ptr_offset<ptr<ptr<const void>>, subtract=false, element=ptr<const void>, overflow=ub>(array_decay<ptr<ptr<const void>>, length=Some(3)>(%[[VALUE_step0_jumps]]), const<i32>(2))));
// DEFAULT-NEXT:                 label %[[VALUE_do_precision]] do_precision:
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_f]]);
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_f]], read<ptr<const i8>>(%[[VALUE8]]));
// DEFAULT-NEXT:                 va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_spec]], read<i8>(deref(read<ptr<const i8>>(%[[VALUE_f]]))));
// DEFAULT-NEXT:                 goto *read<ptr<const void>>(deref(ptr_offset<ptr<ptr<const void>>, subtract=false, element=ptr<const void>, overflow=ub>(array_decay<ptr<ptr<const void>>, length=Some(3)>(%[[VALUE_step0_jumps]]), const<i32>(2))));
// DEFAULT-NEXT:                 label %[[VALUE_do_form_integer]] do_form_integer:
// DEFAULT-NEXT:                     va_arg<u64>(%[[VALUE_ap]]);
// DEFAULT-NEXT:                 goto %[[VALUE_end:[0-9]+]];
// DEFAULT-NEXT:                 label %[[VALUE_do_form_string]] do_form_string:
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_string]], va_arg<ptr<const i8>>(%[[VALUE_ap]]));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], read<ptr<i8>>(%[[VALUE_s]]), read<ptr<const i8>>(%[[VALUE_string]]));
// DEFAULT-NEXT:                 label %[[VALUE_end]] end:
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_f]]);
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_f]], read<ptr<const i8>>(%[[VALUE10]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_f]])))), const<i32>(0));
// DEFAULT-NEXT:         label %[[VALUE_all_done]] all_done:
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f_2:[0-9]+]] @f(%[[VALUE_s_2:[0-9]+]] s: ptr<i8>, %[[VALUE_f_3:[0-9]+]] f: ptr<const i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, va_list) -> i32>(%[[VALUE_g]], read<ptr<i8>>(%[[VALUE_s_2]]), read<ptr<const i8>>(%[[VALUE_f_3]]), read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<i8, 10> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<const i8>, ...) -> void>(%[[VALUE_f_2]], array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_buf]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]]), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_buf]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_3]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
