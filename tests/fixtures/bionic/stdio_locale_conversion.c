#include <langinfo.h>
#include <locale.h>
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

int bionic_stream_position(FILE *stream, fpos_t *pos) {
  if (fgetpos(stream, pos) != 0)
    return -1;
  return fsetpos(stream, pos);
}

int bionic_multibyte_roundtrip(const char *input, char *output) {
  mbstate_t decode_state;
  mbstate_t encode_state;
  wchar_t   wide;
  __builtin_memset(&decode_state, 0, sizeof(decode_state));
  __builtin_memset(&encode_state, 0, sizeof(encode_state));
  if (mbrtowc(&wide, input, 1, &decode_state) == (size_t)-1)
    return -1;
  return (int)wcrtomb(output, wide, &encode_state);
}

int bionic_locale_scope(void) {
  locale_t updated = newlocale(LC_ALL_MASK, "C", (locale_t)0);
  locale_t previous = uselocale(updated);
  int      is_alpha = iswctype(L'a', wctype("alpha"));
  freelocale(previous);
  return is_alpha;
}

const char *bionic_codeset(void) { return nl_langinfo(CODESET); }

int main(void) { return 0; }
