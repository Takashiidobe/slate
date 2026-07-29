#include <stddef.h>
#include <stdio.h>

struct NaturalBefore {
  unsigned char tag;
  unsigned int value;
};

#pragma pack(push, 2)
struct PackedTwo {
  unsigned char tag;
  unsigned int value;
};

#pragma pack(push, 1)
struct PackedOne {
  unsigned char tag;
  unsigned int value;
};
#pragma pack(pop)

struct PackedTwoAgain {
  unsigned char tag;
  unsigned int value;
};
#pragma pack(pop)

struct NaturalAfter {
  unsigned char tag;
  unsigned int value;
};

int main(void) {
  struct PackedOne packed = {29, 31};
  printf("%d %d %d %d %d %d %d %d %d %d %d\n",
         (int)sizeof(struct NaturalBefore),
         (int)offsetof(struct NaturalBefore, value),
         (int)sizeof(struct PackedTwo),
         (int)_Alignof(struct PackedTwo),
         (int)offsetof(struct PackedTwo, value), (int)sizeof(packed),
         (int)_Alignof(struct PackedOne),
         (int)offsetof(struct PackedOne, value),
         (int)sizeof(struct PackedTwoAgain),
         (int)offsetof(struct PackedTwoAgain, value),
         (int)offsetof(struct NaturalAfter, value));
  return packed.tag == 29 && packed.value == 31 ? 0 : 1;
}
