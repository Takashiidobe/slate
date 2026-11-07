/* { dg-do compile } */
/* { dg-options "-Wno-old-style-definition -O2 -Wall" } */
/* On Alpha EV4, dead code elimination and cfg simplification conspired
   to leave the register containing 'C' marked live, though all references
   to the variable had been removed.  */

struct operand_data
{
  struct operand_data *next;
  int index;
  const char *predicate;
  const char *constraint;
  int mode;
  unsigned char n_alternatives;
  char address_p;
  char strict_low;
  char eliminable;
  char seen;
};

struct data
{
  struct data *next;
  const char *name;
  const char *template;
  int code_number;
  int index_number;
  int lineno;
  int n_operands;
  int n_dups;
  int n_alternatives;
  int operand_number;
  int output_format;
  struct operand_data operand[40];
};

extern void message_with_line (int, const char *, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));
extern int have_error;

extern char *strchr (__const char *__s, int __c) __attribute__ ((__pure__));

void
validate_insn_alternatives (d)
     struct data *d;
{
  int n = 0, start;

  for (start = 0; start < d->n_operands; start++)
    if (d->operand[start].n_alternatives > 0)
      {
        int len, i;
        const char *p;
        char c;	 /* { dg-bogus "used uninitialized" "uninitialized variable warning" } */
        int which_alternative = 0;
        int alternative_count_unsure = 0;

        for (p = d->operand[start].constraint; (c = *p); p += len)
          {
            len = 1;

            if (len < 1 || (len > 1 && strchr (",#*+=&%!0123456789", c)))
              {
                message_with_line (d->lineno,
                                   "invalid length %d for char '%c' in alternative %d of operand %d",
                                    len, c, which_alternative, start);
                len = 1;
                have_error = 1;
              }

            if (c == ',')
              {
                which_alternative++;
                continue;
              }

            for (i = 1; i < len; i++)
              if (p[i] == '\0')
                {
                  message_with_line (d->lineno,
                                     "NUL in alternative %d of operand %d",
                                     which_alternative, start);
                  alternative_count_unsure = 1;
                  break;
                }
              else if (strchr (",#*", p[i]))
                {
                  message_with_line (d->lineno,
                                     "'%c' in alternative %d of operand %d",
                                     p[i], which_alternative, start);
                  alternative_count_unsure = 1;
                }
          }
        if (alternative_count_unsure)
          have_error = 1;
        else if (n == 0)
          n = d->operand[start].n_alternatives;
        else if (n != d->operand[start].n_alternatives)
          {
            message_with_line (d->lineno,
                               "wrong number of alternatives in operand %d",
                               start);
            have_error = 1;
          }
      }


  d->n_alternatives = n;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     type @type[[TYPE_operand_data:[0-9]+]] operand_data = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type[[TYPE_operand_data]]>;
// DEFAULT-NEXT:         field1 index: i32;
// DEFAULT-NEXT:         field2 predicate: ptr<const i8>;
// DEFAULT-NEXT:         field3 constraint: ptr<const i8>;
// DEFAULT-NEXT:         field4 mode: i32;
// DEFAULT-NEXT:         field5 n_alternatives: u8;
// DEFAULT-NEXT:         field6 address_p: i8;
// DEFAULT-NEXT:         field7 strict_low: i8;
// DEFAULT-NEXT:         field8 eliminable: i8;
// DEFAULT-NEXT:         field9 seen: i8;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 8, 16, 24, 32, 36, 37, 38, 39, 40]];
// DEFAULT-NEXT:     type @type[[TYPE_data:[0-9]+]] data = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type[[TYPE_data]]>;
// DEFAULT-NEXT:         field1 name: ptr<const i8>;
// DEFAULT-NEXT:         field2 template: ptr<const i8>;
// DEFAULT-NEXT:         field3 code_number: i32;
// DEFAULT-NEXT:         field4 index_number: i32;
// DEFAULT-NEXT:         field5 lineno: i32;
// DEFAULT-NEXT:         field6 n_operands: i32;
// DEFAULT-NEXT:         field7 n_dups: i32;
// DEFAULT-NEXT:         field8 n_alternatives: i32;
// DEFAULT-NEXT:         field9 operand_number: i32;
// DEFAULT-NEXT:         field10 output_format: i32;
// DEFAULT-NEXT:         field11 operand: array<@type[[TYPE_operand_data]], 40>;
// DEFAULT-NEXT:     } [size=1976, align=8, offsets=[0, 8, 16, 24, 28, 32, 36, 40, 44, 48, 52, 56]];
// DEFAULT-NEXT:     extern %[[VALUE_have_error:[0-9]+]] have_error: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([44, 35, 42, 43, 61, 38, 37, 33, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([105, 110, 118, 97, 108, 105, 100, 32, 108, 101, 110, 103, 116, 104, 32, 37, 100, 32, 102, 111, 114, 32, 99, 104, 97, 114, 32, 39, 37, 99, 39, 32, 105, 110, 32, 97, 108, 116, 101, 114, 110, 97, 116, 105, 118, 101, 32, 37, 100, 32, 111, 102, 32, 111, 112, 101, 114, 97, 110, 100, 32, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 36> [storage=static] = code_units<array<i8, 36>>([78, 85, 76, 32, 105, 110, 32, 97, 108, 116, 101, 114, 110, 97, 116, 105, 118, 101, 32, 37, 100, 32, 111, 102, 32, 111, 112, 101, 114, 97, 110, 100, 32, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([44, 35, 42, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([39, 37, 99, 39, 32, 105, 110, 32, 97, 108, 116, 101, 114, 110, 97, 116, 105, 118, 101, 32, 37, 100, 32, 111, 102, 32, 111, 112, 101, 114, 97, 110, 100, 32, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 43> [storage=static] = code_units<array<i8, 43>>([119, 114, 111, 110, 103, 32, 110, 117, 109, 98, 101, 114, 32, 111, 102, 32, 97, 108, 116, 101, 114, 110, 97, 116, 105, 118, 101, 115, 32, 105, 110, 32, 111, 112, 101, 114, 97, 110, 100, 32, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_message_with_line:[0-9]+]] @message_with_line(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strchr:[0-9]+]] @strchr(%[[VALUE___s:[0-9]+]] __s: ptr<const i8>, %[[VALUE___c:[0-9]+]] __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_validate_insn_alternatives:[0-9]+]] @validate_insn_alternatives(%[[VALUE_d:[0-9]+]] d: ptr<@type[[TYPE_data]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_start:[0-9]+]] start: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_start]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_start]]), read<i32>(field6(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]])))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_start]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_start]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if gt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field5(deref(ptr_offset<ptr<@type[[TYPE_operand_data]]>, subtract=false, element=@type[[TYPE_operand_data]], overflow=ub>(array_decay<ptr<@type[[TYPE_operand_data]]>, length=Some(40)>(field11(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]])))), read<i32>(%[[VALUE_start]]))))))), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_len:[0-9]+]] len: i32 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_p:[0-9]+]] p: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_which_alternative:[0-9]+]] which_alternative: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         let %[[VALUE_alternative_count_unsure:[0-9]+]] alternative_count_unsure: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<ptr<const i8>>(%[[VALUE_p]], read<ptr<const i8>>(field3(deref(ptr_offset<ptr<@type[[TYPE_operand_data]]>, subtract=false, element=@type[[TYPE_operand_data]], overflow=ub>(array_decay<ptr<@type[[TYPE_operand_data]]>, length=Some(40)>(field11(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]])))), read<i32>(%[[VALUE_start]]))))));
// DEFAULT-NEXT:                             condition: {
// DEFAULT-NEXT:                                 let %[[VALUE6:[0-9]+]]: i8 [synthetic] = read<i8>(deref(read<ptr<const i8>>(%[[VALUE_p]])));
// DEFAULT-NEXT:                                 write<i8>(%[[VALUE_c]], read<i8>(%[[VALUE6]]));
// DEFAULT-NEXT:                                 yield ne<i8>(read<i8>(%[[VALUE6]]), const<i8>(0));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE7:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE7]]), read<i32>(%[[VALUE_len]]));
// DEFAULT-NEXT:                                 write<ptr<const i8>>(%[[VALUE_p]], read<ptr<const i8>>(%[[VALUE8]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_len]], const<i32>(1));
// DEFAULT-NEXT:                                     if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE_len]]), const<i32>(1)), logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_len]]), const<i32>(1)), ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str]])), widen<i32, reason=arg>(read<i8>(%[[VALUE_c]]))), null<ptr<i8>>)))
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             call<void, signature=fn(i32, ptr<const i8>, ...) -> void>(%[[VALUE_message_with_line]], read<i32>(field5(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_len]]), widen<i32, reason=vararg>(read<i8>(%[[VALUE_c]])), read<i32>(%[[VALUE_which_alternative]]), read<i32>(%[[VALUE_start]]));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_len]], const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_have_error]], const<i32>(1));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                     if eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(44))
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_which_alternative]]);
// DEFAULT-NEXT:                                             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_which_alternative]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                                             continue %[[VALUE5]];
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                     for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_p]]), read<i32>(%[[VALUE_i]]))))), const<i32>(0))
// DEFAULT-NEXT:                                                 {
// DEFAULT-NEXT:                                                     call<void, signature=fn(i32, ptr<const i8>, ...) -> void>(%[[VALUE_message_with_line]], read<i32>(field5(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(36)>(%[[VALUE_str_3]])), read<i32>(%[[VALUE_which_alternative]]), read<i32>(%[[VALUE_start]]));
// DEFAULT-NEXT:                                                     write<i32>(%[[VALUE_alternative_count_unsure]], const<i32>(1));
// DEFAULT-NEXT:                                                     break %[[VALUE11]];
// DEFAULT-NEXT:                                                 }
// DEFAULT-NEXT:                                             else
// DEFAULT-NEXT:                                                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_p]]), read<i32>(%[[VALUE_i]])))))), null<ptr<i8>>)
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         call<void, signature=fn(i32, ptr<const i8>, ...) -> void>(%[[VALUE_message_with_line]], read<i32>(field5(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(37)>(%[[VALUE_str_5]])), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_p]]), read<i32>(%[[VALUE_i]]))))), read<i32>(%[[VALUE_which_alternative]]), read<i32>(%[[VALUE_start]]));
// DEFAULT-NEXT:                                                         write<i32>(%[[VALUE_alternative_count_unsure]], const<i32>(1));
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%[[VALUE_alternative_count_unsure]]), const<i32>(0))
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_have_error]], const<i32>(1));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_n]], reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(field5(deref(ptr_offset<ptr<@type[[TYPE_operand_data]]>, subtract=false, element=@type[[TYPE_operand_data]], overflow=ub>(array_decay<ptr<@type[[TYPE_operand_data]]>, length=Some(40)>(field11(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]])))), read<i32>(%[[VALUE_start]]))))))));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_n]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field5(deref(ptr_offset<ptr<@type[[TYPE_operand_data]]>, subtract=false, element=@type[[TYPE_operand_data]], overflow=ub>(array_decay<ptr<@type[[TYPE_operand_data]]>, length=Some(40)>(field11(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]])))), read<i32>(%[[VALUE_start]]))))))))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         call<void, signature=fn(i32, ptr<const i8>, ...) -> void>(%[[VALUE_message_with_line]], read<i32>(field5(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(43)>(%[[VALUE_str_6]])), read<i32>(%[[VALUE_start]]));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_have_error]], const<i32>(1));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         write<i32>(field8(deref(read<ptr<@type[[TYPE_data]]>>(%[[VALUE_d]]))), read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
