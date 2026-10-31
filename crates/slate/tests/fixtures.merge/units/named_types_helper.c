#include <stddef.h>

struct tally {
    long count;
    size_t bytes;
    char tag;
};

long tally_sum(const struct tally *tally) {
    return tally->count + (long)tally->bytes + tally->tag;
}
