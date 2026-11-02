/* SLATE: volatile access to a misaligned packed field needs one unaligned
 load or store. Rust's read_volatile/write_volatile require an aligned
 pointer, read_unaligned is not volatile, and byte-wise or asm fallbacks
 change the access width. Slate reports a barrier instead of emitting UB
 (slate-yar4.7). */
struct __attribute__((packed)) S { char pad; volatile int x; };
_Alignas(4) struct S s;
int main(void) { s.x = 7; return s.x != 7; }
