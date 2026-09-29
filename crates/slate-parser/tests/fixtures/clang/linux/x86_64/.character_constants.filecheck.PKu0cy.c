
static_assert(sizeof('a') == 4);
static_assert(sizeof(u8'a') == 1);
static_assert(sizeof(u'a') == 2);
static_assert(sizeof(U'a') == 4);
static_assert(sizeof(L'a') == 4);

int plain = 'a';
int signed_byte = '\xff';
int multicharacter = 'ab';
int multicharacter_truncated = 'abcde';
int wide = L'Ω';
int wide_escape = L'\xff';
unsigned char utf8 = u8'a';
unsigned short utf16 = u'Ω';
unsigned int utf32 = U'\U0001F600';

