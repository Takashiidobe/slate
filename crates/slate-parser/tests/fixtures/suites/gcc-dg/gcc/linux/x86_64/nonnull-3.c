/* Test for the "nonnull" function attribute on builtins.  Use the
   "__builtin_" style below so we don't need prototypes.  */
/* Origin: Kaveh R. Ghazi <ghazi@caip.rutgers.edu> */
/* { dg-do compile } */
/* { dg-options "-Wnonnull" } */

#include <stddef.h>

void
foo (void *p, char *s)
{
  __builtin_bzero (NULL, 16);  /* { dg-warning "null" "pr80936" } */
  __builtin_bcopy (NULL, p, 16);  /* { dg-warning "null" "pr80936" } */
  __builtin_bcopy (p, NULL, 16);  /* { dg-warning "null" "pr80936" } */
  __builtin_bcmp (NULL, p, 16);  /* { dg-warning "null" "pr80936" } */
  __builtin_bcmp (p, NULL, 16);  /* { dg-warning "null" "pr80936" } */
  __builtin_index (NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_rindex (NULL, 16);  /* { dg-warning "null" "null pointer check" } */

  __builtin_memcpy (p, NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_memcpy (NULL, p, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_memmove (p, NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_memmove (NULL, p, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_memcmp (p, NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_memcmp (NULL, p, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_memset (NULL, 0, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_mempcpy (p, NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_mempcpy (NULL, p, 16);  /* { dg-warning "null" "null pointer check" } */

  __builtin_strcat (NULL, s);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strcat (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strncat (NULL, s, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strncat (s, NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_stpcpy (NULL, s);  /* { dg-warning "null" "null pointer check" } */
  __builtin_stpcpy (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_stpncpy (NULL, s, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_stpncpy (s, NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strcpy (NULL, s);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strcpy (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strncpy (NULL, s, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strncpy (s, NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strcasecmp (NULL, s);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strcasecmp (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strncasecmp (NULL, s, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strncasecmp (s, NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strcmp (NULL, s);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strcmp (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strncmp (NULL, s, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strncmp (s, NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strlen (NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strstr (NULL, s);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strstr (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strpbrk (NULL, s);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strpbrk (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strspn (NULL, s);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strspn (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strchr (NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  __builtin_strrchr (NULL, 16);  /* { dg-warning "null" "null pointer check" } */
  void *p1 = __builtin_strdup (NULL);  /* { dg-warning "null" "null pointer check" } */
  void *p2 = __builtin_strndup (NULL, 16);  /* { dg-warning "null" "null pointer check" } */

  __builtin_nan (NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_nanf (NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_nanl (NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_nans (NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_nansf (NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_nansl (NULL);  /* { dg-warning "null" "null pointer check" } */

  __builtin_puts (NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fputc (*s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fputs (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fputs (NULL, p);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fwrite (s, 16, 16, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fwrite (NULL, 16, 16, p);  /* { dg-warning "null" "null pointer check" } */
  __builtin_puts_unlocked (NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fputc_unlocked (*s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fputs_unlocked (s, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fputs_unlocked (NULL, p);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fwrite_unlocked (s, 16, 16, NULL);  /* { dg-warning "null" "null pointer check" } */
  __builtin_fwrite_unlocked (NULL, 16, 16, p);  /* { dg-warning "null" "null pointer check" } */

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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bzero:[0-9]+]] @__builtin_bzero(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bcopy:[0-9]+]] @__builtin_bcopy(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bcmp:[0-9]+]] @__builtin_bcmp(%[[VALUE5:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE6:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE7:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_index:[0-9]+]] @__builtin_index(%[[VALUE8:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE9:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_rindex:[0-9]+]] @__builtin_rindex(%[[VALUE10:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE11:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE12:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE13:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE14:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memmove:[0-9]+]] @__builtin_memmove(%[[VALUE15:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE16:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE17:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE18:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE19:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE20:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE21:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE22:[0-9]+]] <unnamed>: i32, %[[VALUE23:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_mempcpy:[0-9]+]] @__builtin_mempcpy(%[[VALUE24:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE25:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE26:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcat:[0-9]+]] @__builtin_strcat(%[[VALUE27:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE28:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strncat:[0-9]+]] @__builtin_strncat(%[[VALUE29:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE30:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE31:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stpcpy:[0-9]+]] @__builtin_stpcpy(%[[VALUE32:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE33:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stpncpy:[0-9]+]] @__builtin_stpncpy(%[[VALUE34:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE35:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE36:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcpy:[0-9]+]] @__builtin_strcpy(%[[VALUE37:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE38:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strncpy:[0-9]+]] @__builtin_strncpy(%[[VALUE39:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE40:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE41:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcasecmp:[0-9]+]] @__builtin_strcasecmp(%[[VALUE42:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE43:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strncasecmp:[0-9]+]] @__builtin_strncasecmp(%[[VALUE44:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE45:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE46:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcmp:[0-9]+]] @__builtin_strcmp(%[[VALUE47:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE48:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strncmp:[0-9]+]] @__builtin_strncmp(%[[VALUE49:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE50:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE51:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strlen:[0-9]+]] @__builtin_strlen(%[[VALUE52:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strstr:[0-9]+]] @__builtin_strstr(%[[VALUE53:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE54:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strpbrk:[0-9]+]] @__builtin_strpbrk(%[[VALUE55:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE56:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strspn:[0-9]+]] @__builtin_strspn(%[[VALUE57:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE58:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strchr:[0-9]+]] @__builtin_strchr(%[[VALUE59:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE60:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strrchr:[0-9]+]] @__builtin_strrchr(%[[VALUE61:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE62:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strdup:[0-9]+]] @__builtin_strdup(%[[VALUE63:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strndup:[0-9]+]] @__builtin_strndup(%[[VALUE64:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE65:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan:[0-9]+]] @__builtin_nan(%[[VALUE66:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf:[0-9]+]] @__builtin_nanf(%[[VALUE67:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanl:[0-9]+]] @__builtin_nanl(%[[VALUE68:[0-9]+]] <unnamed>: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nans:[0-9]+]] @__builtin_nans(%[[VALUE69:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansf:[0-9]+]] @__builtin_nansf(%[[VALUE70:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansl:[0-9]+]] @__builtin_nansl(%[[VALUE71:[0-9]+]] <unnamed>: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_puts:[0-9]+]] @puts(%[[VALUE72:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fputc:[0-9]+]] @fputc(%[[VALUE73:[0-9]+]] <unnamed>: i32, %[[VALUE74:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fputs:[0-9]+]] @fputs(%[[VALUE75:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE76:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fwrite:[0-9]+]] @fwrite(%[[VALUE77:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE78:[0-9]+]] <unnamed>: u64, %[[VALUE79:[0-9]+]] <unnamed>: u64, %[[VALUE80:[0-9]+]] <unnamed>: ptr<void>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_puts_unlocked:[0-9]+]] @puts_unlocked(%[[VALUE81:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fputc_unlocked:[0-9]+]] @fputc_unlocked(%[[VALUE82:[0-9]+]] <unnamed>: i32, %[[VALUE83:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fputs_unlocked:[0-9]+]] @fputs_unlocked(%[[VALUE84:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE85:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fwrite_unlocked:[0-9]+]] @fwrite_unlocked(%[[VALUE86:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE87:[0-9]+]] <unnamed>: u64, %[[VALUE88:[0-9]+]] <unnamed>: u64, %[[VALUE89:[0-9]+]] <unnamed>: ptr<void>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<void>, %[[VALUE_s:[0-9]+]] s: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE___builtin_bzero]], null<ptr<void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ptr<void>, u64) -> void>(%[[VALUE___builtin_bcopy]], null<ptr<const void>>, read<ptr<void>>(%[[VALUE_p]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ptr<void>, u64) -> void>(%[[VALUE___builtin_bcopy]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), null<ptr<void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_bcmp]], null<ptr<const void>>, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_bcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), null<ptr<const void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE___builtin_index]], null<ptr<const i8>>, const<i32>(16));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE___builtin_rindex]], null<ptr<const i8>>, const<i32>(16));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_p]]), null<ptr<const void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], null<ptr<void>>, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memmove]], read<ptr<void>>(%[[VALUE_p]]), null<ptr<const void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memmove]], null<ptr<void>>, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), null<ptr<const void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], null<ptr<const void>>, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], null<ptr<void>>, const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_mempcpy]], read<ptr<void>>(%[[VALUE_p]]), null<ptr<const void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_mempcpy]], null<ptr<void>>, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strcat]], null<ptr<i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strcat]], read<ptr<i8>>(%[[VALUE_s]]), null<ptr<const i8>>);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strncat]], null<ptr<i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strncat]], read<ptr<i8>>(%[[VALUE_s]]), null<ptr<const i8>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_stpcpy]], null<ptr<i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_stpcpy]], read<ptr<i8>>(%[[VALUE_s]]), null<ptr<const i8>>);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], null<ptr<i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_s]]), null<ptr<const i8>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strcpy]], null<ptr<i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strcpy]], read<ptr<i8>>(%[[VALUE_s]]), null<ptr<const i8>>);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strncpy]], null<ptr<i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strncpy]], read<ptr<i8>>(%[[VALUE_s]]), null<ptr<const i8>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE___builtin_strcasecmp]], null<ptr<const i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE___builtin_strcasecmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), null<ptr<const i8>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE___builtin_strncasecmp]], null<ptr<const i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE___builtin_strncasecmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), null<ptr<const i8>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE___builtin_strcmp]], null<ptr<const i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE___builtin_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), null<ptr<const i8>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE___builtin_strncmp]], null<ptr<const i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE___builtin_strncmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), null<ptr<const i8>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE___builtin_strlen]], null<ptr<const i8>>);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strstr]], null<ptr<const i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strstr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), null<ptr<const i8>>);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strpbrk]], null<ptr<const i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strpbrk]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), null<ptr<const i8>>);
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE___builtin_strspn]], null<ptr<const i8>>, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE___builtin_strspn]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), null<ptr<const i8>>);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE___builtin_strchr]], null<ptr<const i8>>, const<i32>(16));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE___builtin_strrchr]], null<ptr<const i8>>, const<i32>(16));
// DEFAULT-NEXT:         let %[[VALUE_p1:[0-9]+]] p1: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strdup]], null<ptr<const i8>>));
// DEFAULT-NEXT:         let %[[VALUE_p2:[0-9]+]] p2: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strndup]], null<ptr<const i8>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], null<ptr<const i8>>);
// DEFAULT-NEXT:         call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], null<ptr<const i8>>);
// DEFAULT-NEXT:         call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], null<ptr<const i8>>);
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nans]], null<ptr<const i8>>);
// DEFAULT-NEXT:         call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nansf]], null<ptr<const i8>>);
// DEFAULT-NEXT:         call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nansl]], null<ptr<const i8>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_puts]], null<ptr<const i8>>);
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<void>) -> i32>(%[[VALUE_fputc]], widen<i32, reason=arg>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_s]])))), null<ptr<void>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<void>) -> i32>(%[[VALUE_fputs]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), null<ptr<void>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<void>) -> i32>(%[[VALUE_fputs]], null<ptr<const i8>>, read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const void>, u64, u64, ptr<void>) -> u64>(%[[VALUE_fwrite]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), null<ptr<void>>);
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const void>, u64, u64, ptr<void>) -> u64>(%[[VALUE_fwrite]], null<ptr<const void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_puts_unlocked]], null<ptr<const i8>>);
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<void>) -> i32>(%[[VALUE_fputc_unlocked]], widen<i32, reason=arg>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_s]])))), null<ptr<void>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<void>) -> i32>(%[[VALUE_fputs_unlocked]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), null<ptr<void>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<void>) -> i32>(%[[VALUE_fputs_unlocked]], null<ptr<const i8>>, read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const void>, u64, u64, ptr<void>) -> u64>(%[[VALUE_fwrite_unlocked]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_s]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), null<ptr<void>>);
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const void>, u64, u64, ptr<void>) -> u64>(%[[VALUE_fwrite_unlocked]], null<ptr<const void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
