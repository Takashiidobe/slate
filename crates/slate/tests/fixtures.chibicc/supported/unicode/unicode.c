#include "test.h"
#include <uchar.h>

#define STR(x) #x

// char16_t and char32_t are now built-in typedefs (C11)
// wchar_t still needs manual typedef (provided by <stddef.h> in standard C)
typedef int wchar_t;

int π = 3;

int main() {
  ASSERT(4, sizeof(L'\0'));
  ASSERT(97, L'a');

  ASSERT(0, strcmp("αβγ", "\u03B1\u03B2\u03B3"));
  ASSERT(0, strcmp("日本語", "\u65E5\u672C\u8A9E"));
  ASSERT(0, strcmp("日本語", "\U000065E5\U0000672C\U00008A9E"));
  ASSERT(0, strcmp("🌮", "\U0001F32E"));

  ASSERT(-1, L'\xffffffff' >> 31);
  ASSERT(946, L'β');
  ASSERT(12354, L'あ');
  ASSERT(127843, L'🍣');

  ASSERT(2, sizeof(u'\0'));
  ASSERT(1, u'\xffff' >> 15);
  ASSERT(97, u'a');
  ASSERT(946, u'β');
  ASSERT(12354, u'あ');
  ASSERT(35486, u'語');

  ASSERT(0, strcmp(STR(u'a'), "u'a'"));

  ASSERT(4, sizeof(U'\0'));
  ASSERT(1, U'\xffffffff' >> 31);
  ASSERT(97, U'a');
  ASSERT(946, U'β');
  ASSERT(12354, U'あ');
  ASSERT(127843, U'🍣');

  ASSERT(0, strcmp(STR(U'a'), "U'a'"));

  ASSERT(4, sizeof(u8"abc"));
  ASSERT(0, strcmp(u8"abc", "abc"));

  ASSERT(0, strcmp(STR(u8"a"), "u8\"a\""));

  ASSERT(2, sizeof(u""));
  ASSERT(10, sizeof(u"\xffzzz"));
  ASSERT(0, memcmp((char *)u"", "\0\0", 2));
  ASSERT(0, memcmp((char *)u"abc", "a\0b\0c\0\0\0", 8));
  ASSERT(0, memcmp((char *)u"日本語", "\345e,g\236\212\0\0", 8));
  ASSERT(0, memcmp((char *)u"🍣", "<\330c\337\0\0", 6));
  ASSERT(u'β', u"βb"[0]);
  ASSERT(u'b', u"βb"[1]);
  ASSERT(0, u"βb"[2]);

  ASSERT(0, strcmp(STR(u"a"), "u\"a\""));

  ASSERT(4, sizeof(U""));
  ASSERT(20, sizeof(U"\xffzzz"));
  ASSERT(0, memcmp((char *)U"", "\0\0\0\0", 4));
  ASSERT(0, memcmp((char *)U"abc", "a\0\0\0b\0\0\0c\0\0\0\0\0\0\0", 16));
  ASSERT(0, memcmp((char *)U"日本語", "\345e\0\0,g\0\0\236\212\0\0\0\0\0\0", 16));
  ASSERT(0, memcmp((char *)U"🍣", "c\363\001\0\0\0\0\0", 8));
  ASSERT(u'β', U"βb"[0]);
  ASSERT(u'b', U"βb"[1]);
  ASSERT(0, U"βb"[2]);
  ASSERT(1, U"\xffffffff"[0] >> 31);

  ASSERT(0, strcmp(STR(U"a"), "U\"a\""));

  ASSERT(4, sizeof(L""));
  ASSERT(20, sizeof(L"\xffzzz"));
  ASSERT(0, memcmp((char *)L"", "\0\0\0\0", 4));
  ASSERT(0, memcmp((char *)L"abc", "a\0\0\0b\0\0\0c\0\0\0\0\0\0\0", 16));
  ASSERT(0, memcmp((char *)L"日本語", "\345e\0\0,g\0\0\236\212\0\0\0\0\0\0", 16));
  ASSERT(0, memcmp((char *)L"🍣", "c\363\001\0\0\0\0\0", 8));
  ASSERT(u'β', L"βb"[0]);
  ASSERT(u'b', L"βb"[1]);
  ASSERT(0, L"βb"[2]);
  ASSERT(-1, L"\xffffffff"[0] >> 31);

  ASSERT(0, strcmp(STR(L"a"), "L\"a\""));

  ASSERT(u'α', ({
           char16_t x[] = u"αβ";
           x[0];
         }));
  ASSERT(u'β', ({
           char16_t x[] = u"αβ";
           x[1];
         }));
  ASSERT(6, ({
           char16_t x[] = u"αβ";
           sizeof(x);
         }));

  ASSERT(U'🤔', ({
           char32_t x[] = U"🤔x";
           x[0];
         }));
  ASSERT(U'x', ({
           char32_t x[] = U"🤔x";
           x[1];
         }));
  ASSERT(12, ({
           char32_t x[] = U"🤔x";
           sizeof(x);
         }));

  ASSERT(L'🤔', ({
           wchar_t x[] = L"🤔x";
           x[0];
         }));
  ASSERT(L'x', ({
           wchar_t x[] = L"🤔x";
           x[1];
         }));
  ASSERT(12, ({
           wchar_t x[] = L"🤔x";
           sizeof(x);
         }));

  ASSERT(3, π);
  ASSERT(3, ({
           int あβ0 = 3;
           あβ0;
         }));
  ASSERT(5, ({
           int $$$ = 5;
           $$$;
         }));

  printf("OK\n");
  return 0;
}
