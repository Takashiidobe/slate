/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target tls } */
/* Sched1 moved {load_tp} pattern between strlen call and the copy
   of the hard return value to its pseudo.  This resulted in a
   reload abort, since the hard register was not spillable.  */

extern __thread int __libc_errno __attribute__ ((tls_model ("initial-exec")));

struct stat64
  {
    long dummy[4];
  };
typedef __SIZE_TYPE__ size_t;
typedef unsigned long long uint64_t;
typedef int __mode_t;

extern size_t strlen (__const char *__s) __attribute__ ((__pure__));
extern int strcmp (__const char *__s1, __const char *__s2)
     __attribute__ ((__pure__));

extern int __open64 (__const char *__file, int __oflag, ...);
extern int __open (__const char *__file, int __oflag, ...);
extern int __mkdir (__const char *__path, __mode_t __mode);
extern int __lxstat64 (int __ver, __const char *__filename,
                       struct stat64 *__stat_buf) ;

static const char letters[] =
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

int
__gen_tempname (char *tmpl, int kind)
{
  int len;
  char *XXXXXX;
  static uint64_t value;
  uint64_t random_time_bits;
  unsigned long count;
  int fd = -1;
  int save_errno = __libc_errno;
  struct stat64 st;
  unsigned long attempts_min = 62L * 62L * 62L;
  unsigned long attempts = attempts_min < 238328 ? 238328 : attempts_min;

  len = strlen (tmpl);
  if (len < 6 || strcmp(&tmpl[len - 6], "XXXXXX"))
    {
      (__libc_errno = (22));
      return -1;
    }

  XXXXXX = &tmpl[len - 6];

  for (count = 0; count < attempts; value += 7777, ++count)
    {
      uint64_t v = value;

      XXXXXX[0] = letters[v % 62];
      v /= 62;
      XXXXXX[1] = letters[v % 62];
      v /= 62;
      XXXXXX[2] = letters[v % 62];
      v /= 62;
      XXXXXX[3] = letters[v % 62];
      v /= 62;
      XXXXXX[4] = letters[v % 62];
      v /= 62;
      XXXXXX[5] = letters[v % 62];

      switch (kind)
        {
        case 0:
          fd = __open (tmpl, 02 | 01000 | 04000, 0400 | 0200);
          break;

        case 1:
          fd = __open64 (tmpl, 02 | 01000 | 04000, 0400 | 0200);
          break;

        case 2:
          fd = __mkdir (tmpl, 0400 | 0200 | 0100);
          break;

        case 3:
          if (__lxstat64 (2, tmpl, &st) < 0)
            {
              if (__libc_errno == 2)
                {
                  (__libc_errno = (save_errno));
                  return 0;
                }
              else

                return -1;
            }
          continue;
        }

      if (fd >= 0)
        {
          (__libc_errno = (save_errno));
          return fd;
        }
      else if (__libc_errno != 17)
        return -1;
    }

  (__libc_errno = (17));
  return -1;
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
// DEFAULT-NEXT:     type @type0 stat64 = struct {
// DEFAULT-NEXT:         field0 dummy: array<i64, 4>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 size_t = u64;
// DEFAULT-NEXT:     type @type2 uint64_t = u64;
// DEFAULT-NEXT:     type @type3 __mode_t = i32;
// DEFAULT-NEXT:     extern %0 __libc_errno: i32 [storage=thread] [linkage=external] [tls_model=initial-exec];
// DEFAULT-NEXT:     global %11 letters: array<i8, 63> [storage=static] [const] [align=16] = code_units<array<i8, 63>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 value: u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([88, 88, 88, 88, 88, 88, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %5 @strlen(%26 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %6 @strcmp(%27 __s1: ptr<const i8>, %28 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %7 @__open64(%29 __file: ptr<const i8>, %30 __oflag: i32, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @__open(%31 __file: ptr<const i8>, %32 __oflag: i32, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @__mkdir(%33 __path: ptr<const i8>, %34 __mode: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @__lxstat64(%35 __ver: i32, %36 __filename: ptr<const i8>, %37 __stat_buf: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @__gen_tempname(%13 tmpl: ptr<i8>, %14 kind: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %16 XXXXXX: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %18 random_time_bits: u64 [storage=automatic];
// DEFAULT-NEXT:         let %19 count: u64 [storage=automatic];
// DEFAULT-NEXT:         let %20 fd: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %21 save_errno: i32 [storage=automatic] = read<i32>(%0);
// DEFAULT-NEXT:         let %22 st: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %23 attempts_min: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(mul<i64, overflow=ub>(mul<i64, overflow=ub>(const<i64>(62), const<i64>(62)), const<i64>(62)));
// DEFAULT-NEXT:         let %24 attempts: u64 [storage=automatic] = conditional<u64>(lt<u64>(read<u64>(%23), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(238328)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(238328))), read<u64>(%23));
// DEFAULT-NEXT:         write<i32>(%15, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%5, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%13))))));
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%15), const<i32>(6)), ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%6, pointer_cast<ptr<const i8>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%13), sub<i32, overflow=ub>(read<i32>(%15), const<i32>(6)))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%38))), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%0, const<i32>(22));
// DEFAULT-NEXT:                 return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<i8>>(%16, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%13), sub<i32, overflow=ub>(read<i32>(%15), const<i32>(6))))));
// DEFAULT-NEXT:         for %39
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%19, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%19), read<u64>(%24))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %41: u64 [synthetic] = read<u64>(%17);
// DEFAULT-NEXT:                 let %42: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%41), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7777))));
// DEFAULT-NEXT:                 write<u64>(%17, read<u64>(%42));
// DEFAULT-NEXT:                 let %43: u64 [synthetic] = read<u64>(%19);
// DEFAULT-NEXT:                 let %44: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%43), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%19, read<u64>(%44));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %25 v: u64 [storage=automatic] = read<u64>(%17);
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(0))), read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(63)>(%11), rem<u64, by_zero=ub>(read<u64>(%25), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))))))));
// DEFAULT-NEXT:                     let %45: u64 [synthetic] = read<u64>(%25);
// DEFAULT-NEXT:                     let %46: u64 [synthetic] = div<u64, by_zero=ub>(read<u64>(%45), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))));
// DEFAULT-NEXT:                     write<u64>(%25, read<u64>(%46));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(1))), read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(63)>(%11), rem<u64, by_zero=ub>(read<u64>(%25), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))))))));
// DEFAULT-NEXT:                     let %47: u64 [synthetic] = read<u64>(%25);
// DEFAULT-NEXT:                     let %48: u64 [synthetic] = div<u64, by_zero=ub>(read<u64>(%47), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))));
// DEFAULT-NEXT:                     write<u64>(%25, read<u64>(%48));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(2))), read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(63)>(%11), rem<u64, by_zero=ub>(read<u64>(%25), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))))))));
// DEFAULT-NEXT:                     let %49: u64 [synthetic] = read<u64>(%25);
// DEFAULT-NEXT:                     let %50: u64 [synthetic] = div<u64, by_zero=ub>(read<u64>(%49), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))));
// DEFAULT-NEXT:                     write<u64>(%25, read<u64>(%50));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(3))), read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(63)>(%11), rem<u64, by_zero=ub>(read<u64>(%25), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))))))));
// DEFAULT-NEXT:                     let %51: u64 [synthetic] = read<u64>(%25);
// DEFAULT-NEXT:                     let %52: u64 [synthetic] = div<u64, by_zero=ub>(read<u64>(%51), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))));
// DEFAULT-NEXT:                     write<u64>(%25, read<u64>(%52));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(4))), read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(63)>(%11), rem<u64, by_zero=ub>(read<u64>(%25), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))))))));
// DEFAULT-NEXT:                     let %53: u64 [synthetic] = read<u64>(%25);
// DEFAULT-NEXT:                     let %54: u64 [synthetic] = div<u64, by_zero=ub>(read<u64>(%53), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))));
// DEFAULT-NEXT:                     write<u64>(%25, read<u64>(%54));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(5))), read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(63)>(%11), rem<u64, by_zero=ub>(read<u64>(%25), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(62))))))));
// DEFAULT-NEXT:                     switch %40 read<i32>(%14)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %40 const<i32>(0):
// DEFAULT-NEXT:                                 write<i32>(%20, call<i32, signature=fn(ptr<const i8>, i32, ...) -> i32>(%8, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%13)), or<i32>(or<i32>(const<i32>(2), const<i32>(512)), const<i32>(2048)), or<i32>(const<i32>(256), const<i32>(128))));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, i32, ...) -> i32>(%8, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%13)), or<i32>(or<i32>(const<i32>(2), const<i32>(512)), const<i32>(2048)), or<i32>(const<i32>(256), const<i32>(128)));
// DEFAULT-NEXT:                             break %40;
// DEFAULT-NEXT:                             case %40 const<i32>(1):
// DEFAULT-NEXT:                                 write<i32>(%20, call<i32, signature=fn(ptr<const i8>, i32, ...) -> i32>(%7, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%13)), or<i32>(or<i32>(const<i32>(2), const<i32>(512)), const<i32>(2048)), or<i32>(const<i32>(256), const<i32>(128))));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, i32, ...) -> i32>(%7, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%13)), or<i32>(or<i32>(const<i32>(2), const<i32>(512)), const<i32>(2048)), or<i32>(const<i32>(256), const<i32>(128)));
// DEFAULT-NEXT:                             break %40;
// DEFAULT-NEXT:                             case %40 const<i32>(2):
// DEFAULT-NEXT:                                 write<i32>(%20, call<i32, signature=fn(ptr<const i8>, i32) -> i32>(%9, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%13)), or<i32>(or<i32>(const<i32>(256), const<i32>(128)), const<i32>(64))));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, i32) -> i32>(%9, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%13)), or<i32>(or<i32>(const<i32>(256), const<i32>(128)), const<i32>(64)));
// DEFAULT-NEXT:                             break %40;
// DEFAULT-NEXT:                             case %40 const<i32>(3):
// DEFAULT-NEXT:                                 if lt<i32>(call<i32, signature=fn(i32, ptr<const i8>, ptr<@type0>) -> i32>(%10, const<i32>(2), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%13)), addr_of<ptr<@type0>>(%22)), const<i32>(0))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         if eq<i32>(read<i32>(%0), const<i32>(2))
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 write<i32>(%0, read<i32>(%21));
// DEFAULT-NEXT:                                                 return const<i32>(0);
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                         else
// DEFAULT-NEXT:                                             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             continue %39;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%20), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%21));
// DEFAULT-NEXT:                             return read<i32>(%20);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%0), const<i32>(17))
// DEFAULT-NEXT:                             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(%0, const<i32>(17));
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
