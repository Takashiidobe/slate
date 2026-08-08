#if defined(__SLATE_ARCH_X86_64) && defined(__SLATE_VENDOR_PC) &&              \
    defined(__SLATE_KERNEL_WINDOWS) && defined(__SLATE_LIBC_MSVC) &&           \
    defined(__SLATE_OBJ_COFF) && defined(__SLATE_WORDSIZE_64) &&               \
    defined(__SLATE_ENDIAN_LITTLE)
static int target_code(void) { return 1; }
#else
static int target_code(void) { return 0; }
#endif

int main(void) { return target_code(); }
