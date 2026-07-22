int main(void) {
    unsigned char data[] = {
#embed "embed_data.bin" limit(2) prefix(9,) suffix(,8)
    };
    unsigned char empty[] = {
#embed "embed_empty.bin" if_empty(7)
    };
    return sizeof(data) == 4 && data[0] == 9 && data[1] == 65 && data[2] == 66 &&
                   data[3] == 8 && sizeof(empty) == 1 && empty[0] == 7
               ? 0
               : 1;
}
