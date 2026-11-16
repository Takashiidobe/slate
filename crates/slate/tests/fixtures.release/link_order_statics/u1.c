#include <stdio.h>
#include "entry.h"

static int value_u1(void) { return 10; }

__attribute__((used, section("slate_static_set"))) static const struct slate_entry entry_u1 = {
    1, &entry_u1, value_u1};

__attribute__((constructor)) static void announce_u1(void) { printf("ctor 1\n"); }

unsigned long step_u1_0(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 3u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u1_1(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 5u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u1_2(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 7u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u1_3(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 9u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u1_4(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 11u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u1_5(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 13u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u1_6(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 9)) * 15u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u1_7(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 10)) * 17u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u1_8(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 11)) * 19u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u1_9(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 12)) * 21u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u1_10(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 13)) * 23u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u1_11(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 14)) * 25u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u1_12(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 15)) * 27u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u1_13(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 16)) * 29u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u1_14(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 17)) * 31u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u1_15(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 18)) * 33u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u1_16(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 19)) * 35u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u1_17(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 20)) * 37u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u1_18(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 21)) * 39u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u1_19(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 22)) * 41u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u1_20(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 23)) * 43u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u1_21(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 24)) * 45u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u1_22(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 25)) * 47u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u1_23(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 26)) * 49u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u1_24(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 27)) * 51u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u1_25(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 28)) * 53u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u1_26(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 29)) * 55u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u1_27(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 30)) * 57u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u1_28(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 31)) * 59u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u1_29(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 61u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u1_30(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 63u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u1_31(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 65u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u1_32(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 67u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u1_33(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 69u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u1_34(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 71u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u1_35(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 9)) * 73u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u1_36(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 10)) * 75u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u1_37(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 11)) * 77u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u1_38(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 12)) * 79u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u1_39(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 13)) * 81u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u1_40(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 14)) * 83u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u1_41(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 15)) * 85u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u1_42(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 16)) * 87u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u1_43(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 17)) * 89u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u1_44(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 18)) * 91u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u1_45(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 19)) * 93u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u1_46(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 20)) * 95u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u1_47(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 21)) * 97u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u1_48(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 22)) * 99u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u1_49(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 23)) * 101u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u1_50(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 24)) * 103u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u1_51(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 25)) * 105u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u1_52(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 26)) * 107u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u1_53(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 27)) * 109u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u1_54(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 28)) * 111u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u1_55(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 29)) * 113u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u1_56(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 30)) * 115u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u1_57(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 31)) * 117u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u1_58(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 119u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u1_59(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 121u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u1_60(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 123u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u1_61(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 125u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u1_62(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 127u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u1_63(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 129u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long work_u1(unsigned long seed) {
  unsigned long acc = seed ^ 1u;
  for (unsigned long i = 0; i < 64; i++) {
    acc = step_u1_0(acc, i);
    acc = step_u1_1(acc, i);
    acc = step_u1_2(acc, i);
    acc = step_u1_3(acc, i);
    acc = step_u1_4(acc, i);
    acc = step_u1_5(acc, i);
    acc = step_u1_6(acc, i);
    acc = step_u1_7(acc, i);
    acc = step_u1_8(acc, i);
    acc = step_u1_9(acc, i);
    acc = step_u1_10(acc, i);
    acc = step_u1_11(acc, i);
    acc = step_u1_12(acc, i);
    acc = step_u1_13(acc, i);
    acc = step_u1_14(acc, i);
    acc = step_u1_15(acc, i);
    acc = step_u1_16(acc, i);
    acc = step_u1_17(acc, i);
    acc = step_u1_18(acc, i);
    acc = step_u1_19(acc, i);
    acc = step_u1_20(acc, i);
    acc = step_u1_21(acc, i);
    acc = step_u1_22(acc, i);
    acc = step_u1_23(acc, i);
    acc = step_u1_24(acc, i);
    acc = step_u1_25(acc, i);
    acc = step_u1_26(acc, i);
    acc = step_u1_27(acc, i);
    acc = step_u1_28(acc, i);
    acc = step_u1_29(acc, i);
    acc = step_u1_30(acc, i);
    acc = step_u1_31(acc, i);
    acc = step_u1_32(acc, i);
    acc = step_u1_33(acc, i);
    acc = step_u1_34(acc, i);
    acc = step_u1_35(acc, i);
    acc = step_u1_36(acc, i);
    acc = step_u1_37(acc, i);
    acc = step_u1_38(acc, i);
    acc = step_u1_39(acc, i);
    acc = step_u1_40(acc, i);
    acc = step_u1_41(acc, i);
    acc = step_u1_42(acc, i);
    acc = step_u1_43(acc, i);
    acc = step_u1_44(acc, i);
    acc = step_u1_45(acc, i);
    acc = step_u1_46(acc, i);
    acc = step_u1_47(acc, i);
    acc = step_u1_48(acc, i);
    acc = step_u1_49(acc, i);
    acc = step_u1_50(acc, i);
    acc = step_u1_51(acc, i);
    acc = step_u1_52(acc, i);
    acc = step_u1_53(acc, i);
    acc = step_u1_54(acc, i);
    acc = step_u1_55(acc, i);
    acc = step_u1_56(acc, i);
    acc = step_u1_57(acc, i);
    acc = step_u1_58(acc, i);
    acc = step_u1_59(acc, i);
    acc = step_u1_60(acc, i);
    acc = step_u1_61(acc, i);
    acc = step_u1_62(acc, i);
    acc = step_u1_63(acc, i);
  }
  return acc;
}
