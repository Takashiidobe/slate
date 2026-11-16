__attribute__((used)) static int registered_u4(void) { return 4; }

__asm__(".pushsection slate_order_set,\"aR\"\n"
        ".balign 4\n"
        ".long registered_u4 - .\n"
        ".popsection");

int registered_extern(void);

__asm__(".pushsection slate_order_set,\"aR\"\n"
        ".balign 4\n"
        ".long registered_extern - .\n"
        ".popsection");

unsigned long step_u4_0(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 3u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u4_1(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 5u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u4_2(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 7u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u4_3(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 9u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u4_4(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 11u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u4_5(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 13u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u4_6(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 9)) * 15u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u4_7(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 10)) * 17u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u4_8(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 11)) * 19u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u4_9(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 12)) * 21u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u4_10(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 13)) * 23u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u4_11(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 14)) * 25u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u4_12(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 15)) * 27u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u4_13(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 16)) * 29u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u4_14(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 17)) * 31u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u4_15(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 18)) * 33u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u4_16(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 19)) * 35u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u4_17(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 20)) * 37u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u4_18(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 21)) * 39u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u4_19(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 22)) * 41u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u4_20(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 23)) * 43u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u4_21(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 24)) * 45u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u4_22(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 25)) * 47u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u4_23(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 26)) * 49u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u4_24(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 27)) * 51u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u4_25(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 28)) * 53u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u4_26(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 29)) * 55u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u4_27(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 30)) * 57u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u4_28(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 31)) * 59u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u4_29(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 61u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u4_30(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 63u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u4_31(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 65u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u4_32(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 67u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u4_33(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 69u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u4_34(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 71u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u4_35(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 9)) * 73u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u4_36(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 10)) * 75u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u4_37(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 11)) * 77u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u4_38(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 12)) * 79u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u4_39(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 13)) * 81u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u4_40(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 14)) * 83u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u4_41(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 15)) * 85u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u4_42(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 16)) * 87u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u4_43(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 17)) * 89u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u4_44(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 18)) * 91u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u4_45(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 19)) * 93u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u4_46(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 20)) * 95u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u4_47(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 21)) * 97u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u4_48(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 22)) * 99u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u4_49(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 23)) * 101u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u4_50(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 24)) * 103u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u4_51(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 25)) * 105u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u4_52(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 26)) * 107u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u4_53(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 27)) * 109u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u4_54(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 28)) * 111u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u4_55(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 29)) * 113u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u4_56(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 30)) * 115u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long step_u4_57(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 31)) * 117u + j;
    else acc = acc + (acc << 2) - j;
  }
  return acc;
}

unsigned long step_u4_58(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 3)) * 119u + j;
    else acc = acc + (acc << 3) - j;
  }
  return acc;
}

unsigned long step_u4_59(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 7; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 4)) * 121u + j;
    else acc = acc + (acc << 4) - j;
  }
  return acc;
}

unsigned long step_u4_60(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 3; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 5)) * 123u + j;
    else acc = acc + (acc << 5) - j;
  }
  return acc;
}

unsigned long step_u4_61(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 4; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 6)) * 125u + j;
    else acc = acc + (acc << 6) - j;
  }
  return acc;
}

unsigned long step_u4_62(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 5; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 7)) * 127u + j;
    else acc = acc + (acc << 7) - j;
  }
  return acc;
}

unsigned long step_u4_63(unsigned long acc, unsigned long i) {
  for (unsigned long j = 0; j < i % 6; j++) {
    if (acc & 1) acc = (acc ^ (acc >> 8)) * 129u + j;
    else acc = acc + (acc << 1) - j;
  }
  return acc;
}

unsigned long work_u4(unsigned long seed) {
  unsigned long acc = seed ^ 4u;
  for (unsigned long i = 0; i < 64; i++) {
    acc = step_u4_0(acc, i);
    acc = step_u4_1(acc, i);
    acc = step_u4_2(acc, i);
    acc = step_u4_3(acc, i);
    acc = step_u4_4(acc, i);
    acc = step_u4_5(acc, i);
    acc = step_u4_6(acc, i);
    acc = step_u4_7(acc, i);
    acc = step_u4_8(acc, i);
    acc = step_u4_9(acc, i);
    acc = step_u4_10(acc, i);
    acc = step_u4_11(acc, i);
    acc = step_u4_12(acc, i);
    acc = step_u4_13(acc, i);
    acc = step_u4_14(acc, i);
    acc = step_u4_15(acc, i);
    acc = step_u4_16(acc, i);
    acc = step_u4_17(acc, i);
    acc = step_u4_18(acc, i);
    acc = step_u4_19(acc, i);
    acc = step_u4_20(acc, i);
    acc = step_u4_21(acc, i);
    acc = step_u4_22(acc, i);
    acc = step_u4_23(acc, i);
    acc = step_u4_24(acc, i);
    acc = step_u4_25(acc, i);
    acc = step_u4_26(acc, i);
    acc = step_u4_27(acc, i);
    acc = step_u4_28(acc, i);
    acc = step_u4_29(acc, i);
    acc = step_u4_30(acc, i);
    acc = step_u4_31(acc, i);
    acc = step_u4_32(acc, i);
    acc = step_u4_33(acc, i);
    acc = step_u4_34(acc, i);
    acc = step_u4_35(acc, i);
    acc = step_u4_36(acc, i);
    acc = step_u4_37(acc, i);
    acc = step_u4_38(acc, i);
    acc = step_u4_39(acc, i);
    acc = step_u4_40(acc, i);
    acc = step_u4_41(acc, i);
    acc = step_u4_42(acc, i);
    acc = step_u4_43(acc, i);
    acc = step_u4_44(acc, i);
    acc = step_u4_45(acc, i);
    acc = step_u4_46(acc, i);
    acc = step_u4_47(acc, i);
    acc = step_u4_48(acc, i);
    acc = step_u4_49(acc, i);
    acc = step_u4_50(acc, i);
    acc = step_u4_51(acc, i);
    acc = step_u4_52(acc, i);
    acc = step_u4_53(acc, i);
    acc = step_u4_54(acc, i);
    acc = step_u4_55(acc, i);
    acc = step_u4_56(acc, i);
    acc = step_u4_57(acc, i);
    acc = step_u4_58(acc, i);
    acc = step_u4_59(acc, i);
    acc = step_u4_60(acc, i);
    acc = step_u4_61(acc, i);
    acc = step_u4_62(acc, i);
    acc = step_u4_63(acc, i);
  }
  return acc;
}
