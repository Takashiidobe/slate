#include <stdio.h>
#include "entry.h"

static int value_u7(void) { return 70; }

__attribute__((used, section("slate_static_set"))) static const struct slate_entry entry_u7 = {
    7, &entry_u7, value_u7};

__attribute__((constructor)) static void announce_u7(void) { printf("ctor 7\n"); }

unsigned long step_u7_0(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 3u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u7_1(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 5u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u7_2(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 7u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u7_3(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 9u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u7_4(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 11u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u7_5(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 13u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u7_6(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 9)) * 15u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u7_7(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 10)) * 17u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u7_8(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 11)) * 19u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u7_9(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 12)) * 21u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u7_10(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 13)) * 23u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u7_11(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 14)) * 25u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u7_12(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 15)) * 27u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u7_13(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 16)) * 29u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u7_14(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 17)) * 31u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u7_15(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 18)) * 33u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u7_16(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 19)) * 35u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u7_17(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 20)) * 37u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u7_18(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 21)) * 39u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u7_19(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 22)) * 41u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u7_20(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 23)) * 43u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u7_21(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 24)) * 45u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u7_22(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 25)) * 47u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u7_23(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 26)) * 49u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u7_24(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 27)) * 51u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u7_25(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 28)) * 53u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u7_26(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 29)) * 55u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u7_27(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 30)) * 57u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u7_28(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 31)) * 59u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u7_29(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 61u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u7_30(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 63u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u7_31(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 65u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u7_32(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 67u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u7_33(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 69u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u7_34(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 71u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u7_35(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 9)) * 73u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u7_36(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 10)) * 75u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u7_37(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 11)) * 77u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u7_38(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 12)) * 79u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u7_39(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 13)) * 81u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u7_40(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 14)) * 83u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u7_41(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 15)) * 85u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u7_42(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 16)) * 87u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u7_43(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 17)) * 89u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u7_44(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 18)) * 91u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u7_45(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 19)) * 93u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u7_46(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 20)) * 95u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u7_47(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 21)) * 97u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u7_48(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 22)) * 99u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u7_49(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 23)) * 101u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u7_50(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 24)) * 103u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u7_51(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 25)) * 105u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u7_52(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 26)) * 107u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u7_53(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 27)) * 109u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u7_54(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 28)) * 111u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u7_55(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 29)) * 113u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u7_56(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 30)) * 115u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u7_57(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 31)) * 117u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u7_58(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 119u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u7_59(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 121u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u7_60(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 123u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u7_61(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 125u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u7_62(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 127u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u7_63(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 129u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long work_u7(unsigned long seed) {
  unsigned long acc = seed ^ 7u;
  for (unsigned long i = 0; i < 64; i++) {
    acc = step_u7_0(acc, i);
    acc = step_u7_1(acc, i);
    acc = step_u7_2(acc, i);
    acc = step_u7_3(acc, i);
    acc = step_u7_4(acc, i);
    acc = step_u7_5(acc, i);
    acc = step_u7_6(acc, i);
    acc = step_u7_7(acc, i);
    acc = step_u7_8(acc, i);
    acc = step_u7_9(acc, i);
    acc = step_u7_10(acc, i);
    acc = step_u7_11(acc, i);
    acc = step_u7_12(acc, i);
    acc = step_u7_13(acc, i);
    acc = step_u7_14(acc, i);
    acc = step_u7_15(acc, i);
    acc = step_u7_16(acc, i);
    acc = step_u7_17(acc, i);
    acc = step_u7_18(acc, i);
    acc = step_u7_19(acc, i);
    acc = step_u7_20(acc, i);
    acc = step_u7_21(acc, i);
    acc = step_u7_22(acc, i);
    acc = step_u7_23(acc, i);
    acc = step_u7_24(acc, i);
    acc = step_u7_25(acc, i);
    acc = step_u7_26(acc, i);
    acc = step_u7_27(acc, i);
    acc = step_u7_28(acc, i);
    acc = step_u7_29(acc, i);
    acc = step_u7_30(acc, i);
    acc = step_u7_31(acc, i);
    acc = step_u7_32(acc, i);
    acc = step_u7_33(acc, i);
    acc = step_u7_34(acc, i);
    acc = step_u7_35(acc, i);
    acc = step_u7_36(acc, i);
    acc = step_u7_37(acc, i);
    acc = step_u7_38(acc, i);
    acc = step_u7_39(acc, i);
    acc = step_u7_40(acc, i);
    acc = step_u7_41(acc, i);
    acc = step_u7_42(acc, i);
    acc = step_u7_43(acc, i);
    acc = step_u7_44(acc, i);
    acc = step_u7_45(acc, i);
    acc = step_u7_46(acc, i);
    acc = step_u7_47(acc, i);
    acc = step_u7_48(acc, i);
    acc = step_u7_49(acc, i);
    acc = step_u7_50(acc, i);
    acc = step_u7_51(acc, i);
    acc = step_u7_52(acc, i);
    acc = step_u7_53(acc, i);
    acc = step_u7_54(acc, i);
    acc = step_u7_55(acc, i);
    acc = step_u7_56(acc, i);
    acc = step_u7_57(acc, i);
    acc = step_u7_58(acc, i);
    acc = step_u7_59(acc, i);
    acc = step_u7_60(acc, i);
    acc = step_u7_61(acc, i);
    acc = step_u7_62(acc, i);
    acc = step_u7_63(acc, i);
  }
  return acc;
}
