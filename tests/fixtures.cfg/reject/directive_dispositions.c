#define VALUE 1
#if defined(DISABLED_ERROR)
#error disabled
#endif
#warning visible later
#
#pragma pack(push, 1)
#slate_unknown payload

int main(void) { return VALUE; }
