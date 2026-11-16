#include <stdio.h>
#include "entry.h"

static int value_u6(void) { return 60; }

__attribute__((used, section("slate_static_set"))) static const struct slate_entry entry_u6 = {
    6, &entry_u6, value_u6};

__attribute__((constructor)) static void announce_u6(void) { printf("ctor 6\n"); }

unsigned long step_u6_0(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 3u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u6_1(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 5u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u6_2(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 7u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u6_3(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 9u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u6_4(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 11u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u6_5(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 13u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u6_6(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 9)) * 15u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u6_7(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 10)) * 17u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u6_8(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 11)) * 19u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u6_9(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 12)) * 21u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u6_10(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 13)) * 23u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u6_11(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 14)) * 25u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u6_12(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 15)) * 27u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u6_13(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 16)) * 29u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u6_14(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 17)) * 31u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u6_15(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 18)) * 33u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u6_16(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 19)) * 35u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u6_17(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 20)) * 37u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u6_18(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 21)) * 39u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u6_19(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 22)) * 41u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u6_20(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 23)) * 43u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u6_21(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 24)) * 45u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u6_22(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 25)) * 47u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u6_23(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 26)) * 49u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u6_24(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 27)) * 51u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u6_25(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 28)) * 53u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u6_26(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 29)) * 55u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u6_27(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 30)) * 57u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u6_28(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 31)) * 59u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u6_29(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 61u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u6_30(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 63u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u6_31(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 65u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u6_32(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 67u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u6_33(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 69u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u6_34(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 71u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u6_35(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 9)) * 73u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u6_36(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 10)) * 75u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u6_37(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 11)) * 77u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u6_38(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 12)) * 79u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u6_39(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 13)) * 81u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u6_40(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 14)) * 83u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u6_41(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 15)) * 85u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u6_42(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 16)) * 87u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u6_43(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 17)) * 89u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u6_44(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 18)) * 91u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u6_45(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 19)) * 93u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u6_46(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 20)) * 95u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u6_47(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 21)) * 97u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u6_48(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 22)) * 99u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u6_49(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 23)) * 101u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u6_50(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 24)) * 103u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u6_51(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 25)) * 105u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u6_52(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 26)) * 107u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u6_53(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 27)) * 109u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u6_54(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 28)) * 111u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u6_55(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 29)) * 113u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u6_56(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 30)) * 115u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u6_57(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 31)) * 117u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u6_58(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 119u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u6_59(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 121u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u6_60(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 123u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u6_61(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 125u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u6_62(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 127u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u6_63(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 129u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long work_u6(unsigned long seed) {
  unsigned long acc = seed ^ 6u;
  for (unsigned long i = 0; i < 64; i++) {
    acc = step_u6_0(acc, i);
    acc = step_u6_1(acc, i);
    acc = step_u6_2(acc, i);
    acc = step_u6_3(acc, i);
    acc = step_u6_4(acc, i);
    acc = step_u6_5(acc, i);
    acc = step_u6_6(acc, i);
    acc = step_u6_7(acc, i);
    acc = step_u6_8(acc, i);
    acc = step_u6_9(acc, i);
    acc = step_u6_10(acc, i);
    acc = step_u6_11(acc, i);
    acc = step_u6_12(acc, i);
    acc = step_u6_13(acc, i);
    acc = step_u6_14(acc, i);
    acc = step_u6_15(acc, i);
    acc = step_u6_16(acc, i);
    acc = step_u6_17(acc, i);
    acc = step_u6_18(acc, i);
    acc = step_u6_19(acc, i);
    acc = step_u6_20(acc, i);
    acc = step_u6_21(acc, i);
    acc = step_u6_22(acc, i);
    acc = step_u6_23(acc, i);
    acc = step_u6_24(acc, i);
    acc = step_u6_25(acc, i);
    acc = step_u6_26(acc, i);
    acc = step_u6_27(acc, i);
    acc = step_u6_28(acc, i);
    acc = step_u6_29(acc, i);
    acc = step_u6_30(acc, i);
    acc = step_u6_31(acc, i);
    acc = step_u6_32(acc, i);
    acc = step_u6_33(acc, i);
    acc = step_u6_34(acc, i);
    acc = step_u6_35(acc, i);
    acc = step_u6_36(acc, i);
    acc = step_u6_37(acc, i);
    acc = step_u6_38(acc, i);
    acc = step_u6_39(acc, i);
    acc = step_u6_40(acc, i);
    acc = step_u6_41(acc, i);
    acc = step_u6_42(acc, i);
    acc = step_u6_43(acc, i);
    acc = step_u6_44(acc, i);
    acc = step_u6_45(acc, i);
    acc = step_u6_46(acc, i);
    acc = step_u6_47(acc, i);
    acc = step_u6_48(acc, i);
    acc = step_u6_49(acc, i);
    acc = step_u6_50(acc, i);
    acc = step_u6_51(acc, i);
    acc = step_u6_52(acc, i);
    acc = step_u6_53(acc, i);
    acc = step_u6_54(acc, i);
    acc = step_u6_55(acc, i);
    acc = step_u6_56(acc, i);
    acc = step_u6_57(acc, i);
    acc = step_u6_58(acc, i);
    acc = step_u6_59(acc, i);
    acc = step_u6_60(acc, i);
    acc = step_u6_61(acc, i);
    acc = step_u6_62(acc, i);
    acc = step_u6_63(acc, i);
  }
  return acc;
}
