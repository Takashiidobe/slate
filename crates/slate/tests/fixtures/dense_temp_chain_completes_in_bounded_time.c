#include <stdio.h>

static long results[400];

static void compute(long x) {
  results[0]   = (x + 1) * 3 - 2;
  results[1]   = (x + 2) * 3 - 2;
  results[2]   = (x + 3) * 3 - 2;
  results[3]   = (x + 4) * 3 - 2;
  results[4]   = (x + 5) * 3 - 2;
  results[5]   = (x + 6) * 3 - 2;
  results[6]   = (x + 7) * 3 - 2;
  results[7]   = (x + 8) * 3 - 2;
  results[8]   = (x + 9) * 3 - 2;
  results[9]   = (x + 10) * 3 - 2;
  results[10]  = (x + 11) * 3 - 2;
  results[11]  = (x + 12) * 3 - 2;
  results[12]  = (x + 13) * 3 - 2;
  results[13]  = (x + 14) * 3 - 2;
  results[14]  = (x + 15) * 3 - 2;
  results[15]  = (x + 16) * 3 - 2;
  results[16]  = (x + 17) * 3 - 2;
  results[17]  = (x + 18) * 3 - 2;
  results[18]  = (x + 19) * 3 - 2;
  results[19]  = (x + 20) * 3 - 2;
  results[20]  = (x + 21) * 3 - 2;
  results[21]  = (x + 22) * 3 - 2;
  results[22]  = (x + 23) * 3 - 2;
  results[23]  = (x + 24) * 3 - 2;
  results[24]  = (x + 25) * 3 - 2;
  results[25]  = (x + 26) * 3 - 2;
  results[26]  = (x + 27) * 3 - 2;
  results[27]  = (x + 28) * 3 - 2;
  results[28]  = (x + 29) * 3 - 2;
  results[29]  = (x + 30) * 3 - 2;
  results[30]  = (x + 31) * 3 - 2;
  results[31]  = (x + 32) * 3 - 2;
  results[32]  = (x + 33) * 3 - 2;
  results[33]  = (x + 34) * 3 - 2;
  results[34]  = (x + 35) * 3 - 2;
  results[35]  = (x + 36) * 3 - 2;
  results[36]  = (x + 37) * 3 - 2;
  results[37]  = (x + 38) * 3 - 2;
  results[38]  = (x + 39) * 3 - 2;
  results[39]  = (x + 40) * 3 - 2;
  results[40]  = (x + 41) * 3 - 2;
  results[41]  = (x + 42) * 3 - 2;
  results[42]  = (x + 43) * 3 - 2;
  results[43]  = (x + 44) * 3 - 2;
  results[44]  = (x + 45) * 3 - 2;
  results[45]  = (x + 46) * 3 - 2;
  results[46]  = (x + 47) * 3 - 2;
  results[47]  = (x + 48) * 3 - 2;
  results[48]  = (x + 49) * 3 - 2;
  results[49]  = (x + 50) * 3 - 2;
  results[50]  = (x + 51) * 3 - 2;
  results[51]  = (x + 52) * 3 - 2;
  results[52]  = (x + 53) * 3 - 2;
  results[53]  = (x + 54) * 3 - 2;
  results[54]  = (x + 55) * 3 - 2;
  results[55]  = (x + 56) * 3 - 2;
  results[56]  = (x + 57) * 3 - 2;
  results[57]  = (x + 58) * 3 - 2;
  results[58]  = (x + 59) * 3 - 2;
  results[59]  = (x + 60) * 3 - 2;
  results[60]  = (x + 61) * 3 - 2;
  results[61]  = (x + 62) * 3 - 2;
  results[62]  = (x + 63) * 3 - 2;
  results[63]  = (x + 64) * 3 - 2;
  results[64]  = (x + 65) * 3 - 2;
  results[65]  = (x + 66) * 3 - 2;
  results[66]  = (x + 67) * 3 - 2;
  results[67]  = (x + 68) * 3 - 2;
  results[68]  = (x + 69) * 3 - 2;
  results[69]  = (x + 70) * 3 - 2;
  results[70]  = (x + 71) * 3 - 2;
  results[71]  = (x + 72) * 3 - 2;
  results[72]  = (x + 73) * 3 - 2;
  results[73]  = (x + 74) * 3 - 2;
  results[74]  = (x + 75) * 3 - 2;
  results[75]  = (x + 76) * 3 - 2;
  results[76]  = (x + 77) * 3 - 2;
  results[77]  = (x + 78) * 3 - 2;
  results[78]  = (x + 79) * 3 - 2;
  results[79]  = (x + 80) * 3 - 2;
  results[80]  = (x + 81) * 3 - 2;
  results[81]  = (x + 82) * 3 - 2;
  results[82]  = (x + 83) * 3 - 2;
  results[83]  = (x + 84) * 3 - 2;
  results[84]  = (x + 85) * 3 - 2;
  results[85]  = (x + 86) * 3 - 2;
  results[86]  = (x + 87) * 3 - 2;
  results[87]  = (x + 88) * 3 - 2;
  results[88]  = (x + 89) * 3 - 2;
  results[89]  = (x + 90) * 3 - 2;
  results[90]  = (x + 91) * 3 - 2;
  results[91]  = (x + 92) * 3 - 2;
  results[92]  = (x + 93) * 3 - 2;
  results[93]  = (x + 94) * 3 - 2;
  results[94]  = (x + 95) * 3 - 2;
  results[95]  = (x + 96) * 3 - 2;
  results[96]  = (x + 97) * 3 - 2;
  results[97]  = (x + 98) * 3 - 2;
  results[98]  = (x + 99) * 3 - 2;
  results[99]  = (x + 100) * 3 - 2;
  results[100] = (x + 101) * 3 - 2;
  results[101] = (x + 102) * 3 - 2;
  results[102] = (x + 103) * 3 - 2;
  results[103] = (x + 104) * 3 - 2;
  results[104] = (x + 105) * 3 - 2;
  results[105] = (x + 106) * 3 - 2;
  results[106] = (x + 107) * 3 - 2;
  results[107] = (x + 108) * 3 - 2;
  results[108] = (x + 109) * 3 - 2;
  results[109] = (x + 110) * 3 - 2;
  results[110] = (x + 111) * 3 - 2;
  results[111] = (x + 112) * 3 - 2;
  results[112] = (x + 113) * 3 - 2;
  results[113] = (x + 114) * 3 - 2;
  results[114] = (x + 115) * 3 - 2;
  results[115] = (x + 116) * 3 - 2;
  results[116] = (x + 117) * 3 - 2;
  results[117] = (x + 118) * 3 - 2;
  results[118] = (x + 119) * 3 - 2;
  results[119] = (x + 120) * 3 - 2;
  results[120] = (x + 121) * 3 - 2;
  results[121] = (x + 122) * 3 - 2;
  results[122] = (x + 123) * 3 - 2;
  results[123] = (x + 124) * 3 - 2;
  results[124] = (x + 125) * 3 - 2;
  results[125] = (x + 126) * 3 - 2;
  results[126] = (x + 127) * 3 - 2;
  results[127] = (x + 128) * 3 - 2;
  results[128] = (x + 129) * 3 - 2;
  results[129] = (x + 130) * 3 - 2;
  results[130] = (x + 131) * 3 - 2;
  results[131] = (x + 132) * 3 - 2;
  results[132] = (x + 133) * 3 - 2;
  results[133] = (x + 134) * 3 - 2;
  results[134] = (x + 135) * 3 - 2;
  results[135] = (x + 136) * 3 - 2;
  results[136] = (x + 137) * 3 - 2;
  results[137] = (x + 138) * 3 - 2;
  results[138] = (x + 139) * 3 - 2;
  results[139] = (x + 140) * 3 - 2;
  results[140] = (x + 141) * 3 - 2;
  results[141] = (x + 142) * 3 - 2;
  results[142] = (x + 143) * 3 - 2;
  results[143] = (x + 144) * 3 - 2;
  results[144] = (x + 145) * 3 - 2;
  results[145] = (x + 146) * 3 - 2;
  results[146] = (x + 147) * 3 - 2;
  results[147] = (x + 148) * 3 - 2;
  results[148] = (x + 149) * 3 - 2;
  results[149] = (x + 150) * 3 - 2;
  results[150] = (x + 151) * 3 - 2;
  results[151] = (x + 152) * 3 - 2;
  results[152] = (x + 153) * 3 - 2;
  results[153] = (x + 154) * 3 - 2;
  results[154] = (x + 155) * 3 - 2;
  results[155] = (x + 156) * 3 - 2;
  results[156] = (x + 157) * 3 - 2;
  results[157] = (x + 158) * 3 - 2;
  results[158] = (x + 159) * 3 - 2;
  results[159] = (x + 160) * 3 - 2;
  results[160] = (x + 161) * 3 - 2;
  results[161] = (x + 162) * 3 - 2;
  results[162] = (x + 163) * 3 - 2;
  results[163] = (x + 164) * 3 - 2;
  results[164] = (x + 165) * 3 - 2;
  results[165] = (x + 166) * 3 - 2;
  results[166] = (x + 167) * 3 - 2;
  results[167] = (x + 168) * 3 - 2;
  results[168] = (x + 169) * 3 - 2;
  results[169] = (x + 170) * 3 - 2;
  results[170] = (x + 171) * 3 - 2;
  results[171] = (x + 172) * 3 - 2;
  results[172] = (x + 173) * 3 - 2;
  results[173] = (x + 174) * 3 - 2;
  results[174] = (x + 175) * 3 - 2;
  results[175] = (x + 176) * 3 - 2;
  results[176] = (x + 177) * 3 - 2;
  results[177] = (x + 178) * 3 - 2;
  results[178] = (x + 179) * 3 - 2;
  results[179] = (x + 180) * 3 - 2;
  results[180] = (x + 181) * 3 - 2;
  results[181] = (x + 182) * 3 - 2;
  results[182] = (x + 183) * 3 - 2;
  results[183] = (x + 184) * 3 - 2;
  results[184] = (x + 185) * 3 - 2;
  results[185] = (x + 186) * 3 - 2;
  results[186] = (x + 187) * 3 - 2;
  results[187] = (x + 188) * 3 - 2;
  results[188] = (x + 189) * 3 - 2;
  results[189] = (x + 190) * 3 - 2;
  results[190] = (x + 191) * 3 - 2;
  results[191] = (x + 192) * 3 - 2;
  results[192] = (x + 193) * 3 - 2;
  results[193] = (x + 194) * 3 - 2;
  results[194] = (x + 195) * 3 - 2;
  results[195] = (x + 196) * 3 - 2;
  results[196] = (x + 197) * 3 - 2;
  results[197] = (x + 198) * 3 - 2;
  results[198] = (x + 199) * 3 - 2;
  results[199] = (x + 200) * 3 - 2;
  results[200] = (x + 201) * 3 - 2;
  results[201] = (x + 202) * 3 - 2;
  results[202] = (x + 203) * 3 - 2;
  results[203] = (x + 204) * 3 - 2;
  results[204] = (x + 205) * 3 - 2;
  results[205] = (x + 206) * 3 - 2;
  results[206] = (x + 207) * 3 - 2;
  results[207] = (x + 208) * 3 - 2;
  results[208] = (x + 209) * 3 - 2;
  results[209] = (x + 210) * 3 - 2;
  results[210] = (x + 211) * 3 - 2;
  results[211] = (x + 212) * 3 - 2;
  results[212] = (x + 213) * 3 - 2;
  results[213] = (x + 214) * 3 - 2;
  results[214] = (x + 215) * 3 - 2;
  results[215] = (x + 216) * 3 - 2;
  results[216] = (x + 217) * 3 - 2;
  results[217] = (x + 218) * 3 - 2;
  results[218] = (x + 219) * 3 - 2;
  results[219] = (x + 220) * 3 - 2;
  results[220] = (x + 221) * 3 - 2;
  results[221] = (x + 222) * 3 - 2;
  results[222] = (x + 223) * 3 - 2;
  results[223] = (x + 224) * 3 - 2;
  results[224] = (x + 225) * 3 - 2;
  results[225] = (x + 226) * 3 - 2;
  results[226] = (x + 227) * 3 - 2;
  results[227] = (x + 228) * 3 - 2;
  results[228] = (x + 229) * 3 - 2;
  results[229] = (x + 230) * 3 - 2;
  results[230] = (x + 231) * 3 - 2;
  results[231] = (x + 232) * 3 - 2;
  results[232] = (x + 233) * 3 - 2;
  results[233] = (x + 234) * 3 - 2;
  results[234] = (x + 235) * 3 - 2;
  results[235] = (x + 236) * 3 - 2;
  results[236] = (x + 237) * 3 - 2;
  results[237] = (x + 238) * 3 - 2;
  results[238] = (x + 239) * 3 - 2;
  results[239] = (x + 240) * 3 - 2;
  results[240] = (x + 241) * 3 - 2;
  results[241] = (x + 242) * 3 - 2;
  results[242] = (x + 243) * 3 - 2;
  results[243] = (x + 244) * 3 - 2;
  results[244] = (x + 245) * 3 - 2;
  results[245] = (x + 246) * 3 - 2;
  results[246] = (x + 247) * 3 - 2;
  results[247] = (x + 248) * 3 - 2;
  results[248] = (x + 249) * 3 - 2;
  results[249] = (x + 250) * 3 - 2;
  results[250] = (x + 251) * 3 - 2;
  results[251] = (x + 252) * 3 - 2;
  results[252] = (x + 253) * 3 - 2;
  results[253] = (x + 254) * 3 - 2;
  results[254] = (x + 255) * 3 - 2;
  results[255] = (x + 256) * 3 - 2;
  results[256] = (x + 257) * 3 - 2;
  results[257] = (x + 258) * 3 - 2;
  results[258] = (x + 259) * 3 - 2;
  results[259] = (x + 260) * 3 - 2;
  results[260] = (x + 261) * 3 - 2;
  results[261] = (x + 262) * 3 - 2;
  results[262] = (x + 263) * 3 - 2;
  results[263] = (x + 264) * 3 - 2;
  results[264] = (x + 265) * 3 - 2;
  results[265] = (x + 266) * 3 - 2;
  results[266] = (x + 267) * 3 - 2;
  results[267] = (x + 268) * 3 - 2;
  results[268] = (x + 269) * 3 - 2;
  results[269] = (x + 270) * 3 - 2;
  results[270] = (x + 271) * 3 - 2;
  results[271] = (x + 272) * 3 - 2;
  results[272] = (x + 273) * 3 - 2;
  results[273] = (x + 274) * 3 - 2;
  results[274] = (x + 275) * 3 - 2;
  results[275] = (x + 276) * 3 - 2;
  results[276] = (x + 277) * 3 - 2;
  results[277] = (x + 278) * 3 - 2;
  results[278] = (x + 279) * 3 - 2;
  results[279] = (x + 280) * 3 - 2;
  results[280] = (x + 281) * 3 - 2;
  results[281] = (x + 282) * 3 - 2;
  results[282] = (x + 283) * 3 - 2;
  results[283] = (x + 284) * 3 - 2;
  results[284] = (x + 285) * 3 - 2;
  results[285] = (x + 286) * 3 - 2;
  results[286] = (x + 287) * 3 - 2;
  results[287] = (x + 288) * 3 - 2;
  results[288] = (x + 289) * 3 - 2;
  results[289] = (x + 290) * 3 - 2;
  results[290] = (x + 291) * 3 - 2;
  results[291] = (x + 292) * 3 - 2;
  results[292] = (x + 293) * 3 - 2;
  results[293] = (x + 294) * 3 - 2;
  results[294] = (x + 295) * 3 - 2;
  results[295] = (x + 296) * 3 - 2;
  results[296] = (x + 297) * 3 - 2;
  results[297] = (x + 298) * 3 - 2;
  results[298] = (x + 299) * 3 - 2;
  results[299] = (x + 300) * 3 - 2;
  results[300] = (x + 301) * 3 - 2;
  results[301] = (x + 302) * 3 - 2;
  results[302] = (x + 303) * 3 - 2;
  results[303] = (x + 304) * 3 - 2;
  results[304] = (x + 305) * 3 - 2;
  results[305] = (x + 306) * 3 - 2;
  results[306] = (x + 307) * 3 - 2;
  results[307] = (x + 308) * 3 - 2;
  results[308] = (x + 309) * 3 - 2;
  results[309] = (x + 310) * 3 - 2;
  results[310] = (x + 311) * 3 - 2;
  results[311] = (x + 312) * 3 - 2;
  results[312] = (x + 313) * 3 - 2;
  results[313] = (x + 314) * 3 - 2;
  results[314] = (x + 315) * 3 - 2;
  results[315] = (x + 316) * 3 - 2;
  results[316] = (x + 317) * 3 - 2;
  results[317] = (x + 318) * 3 - 2;
  results[318] = (x + 319) * 3 - 2;
  results[319] = (x + 320) * 3 - 2;
  results[320] = (x + 321) * 3 - 2;
  results[321] = (x + 322) * 3 - 2;
  results[322] = (x + 323) * 3 - 2;
  results[323] = (x + 324) * 3 - 2;
  results[324] = (x + 325) * 3 - 2;
  results[325] = (x + 326) * 3 - 2;
  results[326] = (x + 327) * 3 - 2;
  results[327] = (x + 328) * 3 - 2;
  results[328] = (x + 329) * 3 - 2;
  results[329] = (x + 330) * 3 - 2;
  results[330] = (x + 331) * 3 - 2;
  results[331] = (x + 332) * 3 - 2;
  results[332] = (x + 333) * 3 - 2;
  results[333] = (x + 334) * 3 - 2;
  results[334] = (x + 335) * 3 - 2;
  results[335] = (x + 336) * 3 - 2;
  results[336] = (x + 337) * 3 - 2;
  results[337] = (x + 338) * 3 - 2;
  results[338] = (x + 339) * 3 - 2;
  results[339] = (x + 340) * 3 - 2;
  results[340] = (x + 341) * 3 - 2;
  results[341] = (x + 342) * 3 - 2;
  results[342] = (x + 343) * 3 - 2;
  results[343] = (x + 344) * 3 - 2;
  results[344] = (x + 345) * 3 - 2;
  results[345] = (x + 346) * 3 - 2;
  results[346] = (x + 347) * 3 - 2;
  results[347] = (x + 348) * 3 - 2;
  results[348] = (x + 349) * 3 - 2;
  results[349] = (x + 350) * 3 - 2;
  results[350] = (x + 351) * 3 - 2;
  results[351] = (x + 352) * 3 - 2;
  results[352] = (x + 353) * 3 - 2;
  results[353] = (x + 354) * 3 - 2;
  results[354] = (x + 355) * 3 - 2;
  results[355] = (x + 356) * 3 - 2;
  results[356] = (x + 357) * 3 - 2;
  results[357] = (x + 358) * 3 - 2;
  results[358] = (x + 359) * 3 - 2;
  results[359] = (x + 360) * 3 - 2;
  results[360] = (x + 361) * 3 - 2;
  results[361] = (x + 362) * 3 - 2;
  results[362] = (x + 363) * 3 - 2;
  results[363] = (x + 364) * 3 - 2;
  results[364] = (x + 365) * 3 - 2;
  results[365] = (x + 366) * 3 - 2;
  results[366] = (x + 367) * 3 - 2;
  results[367] = (x + 368) * 3 - 2;
  results[368] = (x + 369) * 3 - 2;
  results[369] = (x + 370) * 3 - 2;
  results[370] = (x + 371) * 3 - 2;
  results[371] = (x + 372) * 3 - 2;
  results[372] = (x + 373) * 3 - 2;
  results[373] = (x + 374) * 3 - 2;
  results[374] = (x + 375) * 3 - 2;
  results[375] = (x + 376) * 3 - 2;
  results[376] = (x + 377) * 3 - 2;
  results[377] = (x + 378) * 3 - 2;
  results[378] = (x + 379) * 3 - 2;
  results[379] = (x + 380) * 3 - 2;
  results[380] = (x + 381) * 3 - 2;
  results[381] = (x + 382) * 3 - 2;
  results[382] = (x + 383) * 3 - 2;
  results[383] = (x + 384) * 3 - 2;
  results[384] = (x + 385) * 3 - 2;
  results[385] = (x + 386) * 3 - 2;
  results[386] = (x + 387) * 3 - 2;
  results[387] = (x + 388) * 3 - 2;
  results[388] = (x + 389) * 3 - 2;
  results[389] = (x + 390) * 3 - 2;
  results[390] = (x + 391) * 3 - 2;
  results[391] = (x + 392) * 3 - 2;
  results[392] = (x + 393) * 3 - 2;
  results[393] = (x + 394) * 3 - 2;
  results[394] = (x + 395) * 3 - 2;
  results[395] = (x + 396) * 3 - 2;
  results[396] = (x + 397) * 3 - 2;
  results[397] = (x + 398) * 3 - 2;
  results[398] = (x + 399) * 3 - 2;
  results[399] = (x + 400) * 3 - 2;
}

int main(void) {
  compute(1);
  long sum = 0;
  for (int i = 0; i < 400; i++) {
    sum += results[i];
  }
  printf("%ld\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut results: aligned::Aligned<aligned::A16, [i64; 400]> = aligned::Aligned([0; 400]);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut sum: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     compute({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     sum = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 400;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = unsafe { (*results)[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = sum;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 sum = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = sum;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn compute({{arg[0-9]+}}: i64) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 5;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 6;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 7;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 9;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 9;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 11;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 10;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 12;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 11;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 13;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 12;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 14;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 13;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 15;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 14;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 15;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 17;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 16;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 18;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 17;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 19;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 18;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 20;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 19;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 21;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 20;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 22;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 21;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 23;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 22;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 24;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 23;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 25;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 24;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 26;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 25;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 27;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 26;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 28;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 27;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 29;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 28;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 30;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 29;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 31;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 30;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 31;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 33;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 34;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 33;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 35;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 34;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 36;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 35;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 37;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 36;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 38;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 37;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 39;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 38;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 40;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 39;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 41;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 40;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 42;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 41;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 43;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 42;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 44;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 43;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 45;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 44;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 46;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 45;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 47;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 46;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 48;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 47;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 49;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 48;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 50;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 49;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 51;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 50;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 52;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 51;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 53;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 52;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 54;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 53;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 55;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 54;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 56;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 55;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 57;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 56;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 58;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 57;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 59;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 58;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 60;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 59;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 61;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 60;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 62;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 61;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 63;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 62;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 63;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 65;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 64;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 66;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 65;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 67;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 66;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 68;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 67;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 69;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 68;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 70;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 69;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 71;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 70;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 72;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 71;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 73;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 72;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 74;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 73;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 75;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 74;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 76;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 75;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 77;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 76;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 78;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 77;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 79;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 78;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 80;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 79;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 81;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 80;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 82;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 81;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 83;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 82;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 84;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 83;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 85;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 84;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 86;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 85;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 87;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 86;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 88;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 87;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 89;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 88;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 90;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 89;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 91;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 90;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 92;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 91;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 93;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 92;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 94;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 93;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 95;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 94;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 96;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 95;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 97;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 96;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 98;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 97;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 99;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 98;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 99;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 101;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 100;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 102;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 101;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 103;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 102;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 104;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 103;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 105;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 104;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 106;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 105;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 107;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 106;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 108;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 107;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 109;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 108;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 110;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 109;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 111;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 110;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 112;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 111;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 113;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 112;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 114;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 113;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 115;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 114;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 116;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 115;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 117;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 116;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 118;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 117;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 119;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 118;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 120;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 119;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 121;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 120;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 122;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 121;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 123;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 122;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 124;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 123;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 125;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 124;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 126;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 125;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 127;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 126;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 128;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 127;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 129;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 128;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 130;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 129;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 131;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 130;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 132;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 131;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 133;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 132;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 134;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 133;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 135;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 134;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 136;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 135;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 137;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 136;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 138;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 137;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 139;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 138;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 140;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 139;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 141;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 140;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 142;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 141;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 143;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 142;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 144;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 143;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 145;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 144;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 146;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 145;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 147;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 146;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 148;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 147;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 149;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 148;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 150;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 149;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 151;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 150;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 152;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 151;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 153;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 152;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 154;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 153;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 155;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 154;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 156;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 155;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 157;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 156;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 158;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 157;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 159;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 158;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 160;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 159;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 161;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 160;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 162;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 161;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 163;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 162;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 164;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 163;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 165;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 164;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 166;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 165;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 167;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 166;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 168;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 167;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 169;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 168;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 170;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 169;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 171;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 170;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 172;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 171;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 173;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 172;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 174;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 173;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 175;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 174;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 176;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 175;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 177;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 176;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 178;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 177;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 179;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 178;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 180;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 179;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 181;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 180;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 182;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 181;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 183;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 182;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 184;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 183;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 185;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 184;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 186;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 185;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 187;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 186;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 188;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 187;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 189;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 188;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 190;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 189;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 191;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 190;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 192;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 191;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 193;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 192;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 194;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 193;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 195;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 194;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 196;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 195;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 197;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 196;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 198;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 197;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 199;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 198;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 200;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 199;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 201;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 200;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 201;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 203;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 204;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 203;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 205;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 204;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 206;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 205;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 207;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 206;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 208;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 207;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 209;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 208;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 210;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 209;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 211;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 210;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 212;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 211;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 213;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 212;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 214;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 213;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 215;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 214;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 216;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 215;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 217;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 216;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 218;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 217;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 219;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 218;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 220;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 219;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 221;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 220;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 222;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 221;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 223;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 222;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 224;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 223;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 225;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 224;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 226;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 225;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 227;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 226;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 228;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 227;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 229;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 228;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 230;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 229;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 231;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 230;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 232;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 231;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 233;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 232;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 234;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 233;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 235;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 234;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 236;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 235;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 237;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 236;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 238;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 237;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 239;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 238;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 240;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 239;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 241;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 240;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 242;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 241;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 243;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 242;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 244;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 243;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 245;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 244;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 246;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 245;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 247;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 246;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 248;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 247;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 249;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 248;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 250;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 249;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 251;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 250;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 252;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 251;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 253;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 252;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 254;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 253;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 255;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 254;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 256;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 255;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 257;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 256;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 258;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 257;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 259;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 258;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 260;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 259;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 261;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 260;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 262;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 261;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 263;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 262;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 264;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 263;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 265;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 264;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 266;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 265;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 267;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 266;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 268;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 267;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 269;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 268;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 270;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 269;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 271;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 270;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 272;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 271;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 273;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 272;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 274;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 273;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 275;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 274;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 276;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 275;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 277;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 276;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 278;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 277;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 279;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 278;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 280;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 279;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 281;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 280;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 282;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 281;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 283;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 282;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 284;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 283;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 285;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 284;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 286;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 285;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 287;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 286;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 288;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 287;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 289;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 288;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 290;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 289;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 291;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 290;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 292;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 291;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 293;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 292;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 294;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 293;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 295;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 294;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 296;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 295;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 297;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 296;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 298;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 297;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 299;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 298;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 300;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 299;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 301;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 300;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 302;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 301;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 303;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 302;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 304;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 303;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 305;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 304;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 306;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 305;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 307;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 306;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 308;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 307;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 309;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 308;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 310;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 309;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 310;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 312;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 311;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 313;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 312;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 314;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 313;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 315;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 314;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 316;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 315;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 317;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 316;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 318;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 317;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 319;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 318;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 320;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 319;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 321;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 320;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 322;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 321;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 323;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 322;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 324;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 323;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 325;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 324;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 326;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 325;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 327;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 326;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 328;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 327;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 329;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 328;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 330;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 329;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 331;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 330;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 332;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 331;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 333;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 332;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 334;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 333;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 335;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 334;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 336;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 335;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 337;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 336;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 338;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 337;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 339;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 338;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 340;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 339;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 341;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 340;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 342;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 341;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 343;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 342;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 344;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 343;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 345;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 344;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 346;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 345;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 347;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 346;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 348;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 347;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 349;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 348;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 350;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 349;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 351;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 350;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 352;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 351;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 353;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 352;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 354;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 353;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 355;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 354;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 356;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 355;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 357;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 356;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 358;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 357;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 359;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 358;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 360;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 359;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 361;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 360;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 362;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 361;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 363;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 362;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 364;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 363;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 365;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 364;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 366;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 365;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 367;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 366;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 368;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 367;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 369;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 368;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 370;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 369;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 371;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 370;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 372;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 371;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 373;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 372;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 374;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 373;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 375;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 374;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 376;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 375;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 377;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 376;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 378;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 377;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 379;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 378;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 380;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 379;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 381;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 380;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 382;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 381;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 383;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 382;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 384;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 383;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 385;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 384;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 386;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 385;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 387;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 386;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 388;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 387;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 389;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 388;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 390;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 389;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 391;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 390;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 392;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 391;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 393;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 392;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 394;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 393;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 395;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 394;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 396;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 395;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 397;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 396;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 398;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 397;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 399;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 398;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 400;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 399;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut results: aligned::Aligned<aligned::A16, [i64; 400]> = aligned::Aligned([0; 400]);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut sum: i64 = 0;
// REWRITES-NEXT:     compute(1);
// REWRITES-NEXT:     for i in 0..400 {
// REWRITES-NEXT:         sum += unsafe { (*results)[((i as i64) as usize)] };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), sum) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn compute({{arg[0-9]+}}: i64) {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 1) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 2) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 3) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 4) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 5) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 5;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 6) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 6;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 7) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 7;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 8) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 9) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 9;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 10) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 10;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 11) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 11;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 12) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 12;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 13) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 13;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 14) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 14;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 15) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 15;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 16) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 16;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 17) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 17;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 18) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 18;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 19) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 19;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 20) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 20;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 21) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 21;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 22) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 22;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 23) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 23;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 24) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 24;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 25) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 25;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 26) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 26;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 27) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 27;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 28) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 28;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 29) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 29;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 30) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 30;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 31) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 31;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 32) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 33) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 33;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 34) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 34;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 35) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 35;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 36) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 36;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 37) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 37;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 38) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 38;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 39) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 39;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 40) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 40;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 41) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 41;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 42) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 42;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 43) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 43;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 44) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 44;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 45) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 45;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 46) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 46;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 47) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 47;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 48) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 48;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 49) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 49;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 50) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 50;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 51) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 51;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 52) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 52;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 53) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 53;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 54) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 54;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 55) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 55;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 56) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 56;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 57) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 57;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 58) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 58;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 59) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 59;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 60) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 60;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 61) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 61;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 62) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 62;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 63) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 63;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 64) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 64;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 65) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 65;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 66) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 66;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 67) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 67;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 68) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 68;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 69) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 69;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 70) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 70;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 71) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 71;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 72) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 72;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 73) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 73;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 74) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 74;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 75) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 75;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 76) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 76;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 77) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 77;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 78) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 78;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 79) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 79;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 80) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 80;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 81) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 81;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 82) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 82;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 83) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 83;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 84) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 84;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 85) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 85;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 86) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 86;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 87) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 87;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 88) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 88;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 89) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 89;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 90) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 90;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 91) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 91;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 92) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 92;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 93) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 93;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 94) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 94;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 95) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 95;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 96) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 96;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 97) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 97;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 98) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 98;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 99) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 99;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 100) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 100;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 101) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 101;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 102) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 102;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 103) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 103;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 104) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 104;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 105) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 105;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 106) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 106;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 107) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 107;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 108) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 108;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 109) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 109;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 110) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 110;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 111) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 111;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 112) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 112;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 113) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 113;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 114) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 114;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 115) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 115;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 116) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 116;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 117) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 117;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 118) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 118;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 119) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 119;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 120) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 120;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 121) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 121;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 122) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 122;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 123) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 123;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 124) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 124;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 125) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 125;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 126) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 126;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 127) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 127;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 128) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 128;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 129) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 129;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 130) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 130;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 131) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 131;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 132) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 132;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 133) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 133;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 134) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 134;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 135) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 135;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 136) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 136;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 137) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 137;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 138) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 138;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 139) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 139;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 140) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 140;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 141) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 141;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 142) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 142;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 143) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 143;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 144) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 144;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 145) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 145;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 146) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 146;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 147) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 147;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 148) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 148;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 149) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 149;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 150) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 150;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 151) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 151;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 152) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 152;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 153) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 153;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 154) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 154;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 155) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 155;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 156) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 156;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 157) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 157;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 158) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 158;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 159) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 159;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 160) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 160;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 161) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 161;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 162) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 162;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 163) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 163;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 164) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 164;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 165) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 165;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 166) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 166;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 167) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 167;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 168) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 168;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 169) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 169;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 170) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 170;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 171) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 171;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 172) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 172;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 173) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 173;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 174) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 174;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 175) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 175;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 176) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 176;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 177) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 177;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 178) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 178;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 179) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 179;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 180) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 180;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 181) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 181;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 182) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 182;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 183) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 183;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 184) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 184;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 185) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 185;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 186) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 186;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 187) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 187;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 188) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 188;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 189) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 189;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 190) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 190;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 191) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 191;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 192) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 192;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 193) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 193;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 194) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 194;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 195) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 195;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 196) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 196;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 197) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 197;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 198) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 198;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 199) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 199;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 200) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 200;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 201) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 201;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 202) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 202;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 203) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 203;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 204) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 204;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 205) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 205;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 206) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 206;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 207) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 207;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 208) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 208;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 209) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 209;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 210) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 210;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 211) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 211;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 212) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 212;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 213) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 213;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 214) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 214;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 215) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 215;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 216) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 216;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 217) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 217;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 218) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 218;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 219) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 219;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 220) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 220;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 221) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 221;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 222) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 222;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 223) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 223;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 224) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 224;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 225) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 225;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 226) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 226;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 227) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 227;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 228) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 228;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 229) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 229;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 230) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 230;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 231) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 231;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 232) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 232;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 233) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 233;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 234) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 234;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 235) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 235;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 236) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 236;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 237) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 237;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 238) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 238;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 239) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 239;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 240) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 240;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 241) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 241;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 242) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 242;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 243) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 243;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 244) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 244;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 245) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 245;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 246) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 246;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 247) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 247;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 248) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 248;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 249) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 249;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 250) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 250;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 251) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 251;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 252) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 252;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 253) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 253;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 254) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 254;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 255) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 255;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 256) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 256;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 257) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 257;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 258) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 258;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 259) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 259;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 260) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 260;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 261) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 261;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 262) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 262;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 263) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 263;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 264) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 264;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 265) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 265;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 266) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 266;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 267) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 267;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 268) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 268;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 269) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 269;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 270) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 270;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 271) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 271;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 272) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 272;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 273) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 273;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 274) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 274;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 275) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 275;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 276) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 276;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 277) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 277;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 278) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 278;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 279) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 279;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 280) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 280;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 281) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 281;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 282) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 282;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 283) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 283;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 284) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 284;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 285) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 285;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 286) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 286;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 287) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 287;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 288) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 288;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 289) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 289;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 290) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 290;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 291) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 291;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 292) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 292;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 293) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 293;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 294) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 294;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 295) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 295;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 296) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 296;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 297) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 297;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 298) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 298;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 299) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 299;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 300) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 300;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 301) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 301;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 302) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 302;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 303) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 303;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 304) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 304;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 305) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 305;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 306) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 306;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 307) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 307;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 308) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 308;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 309) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 309;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 310) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 310;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 311) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 311;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 312) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 312;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 313) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 313;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 314) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 314;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 315) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 315;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 316) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 316;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 317) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 317;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 318) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 318;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 319) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 319;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 320) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 320;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 321) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 321;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 322) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 322;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 323) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 323;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 324) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 324;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 325) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 325;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 326) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 326;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 327) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 327;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 328) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 328;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 329) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 329;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 330) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 330;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 331) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 331;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 332) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 332;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 333) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 333;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 334) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 334;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 335) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 335;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 336) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 336;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 337) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 337;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 338) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 338;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 339) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 339;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 340) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 340;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 341) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 341;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 342) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 342;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 343) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 343;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 344) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 344;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 345) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 345;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 346) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 346;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 347) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 347;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 348) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 348;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 349) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 349;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 350) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 350;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 351) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 351;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 352) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 352;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 353) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 353;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 354) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 354;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 355) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 355;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 356) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 356;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 357) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 357;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 358) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 358;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 359) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 359;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 360) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 360;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 361) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 361;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 362) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 362;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 363) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 363;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 364) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 364;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 365) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 365;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 366) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 366;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 367) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 367;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 368) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 368;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 369) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 369;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 370) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 370;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 371) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 371;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 372) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 372;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 373) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 373;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 374) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 374;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 375) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 375;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 376) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 376;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 377) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 377;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 378) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 378;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 379) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 379;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 380) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 380;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 381) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 381;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 382) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 382;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 383) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 383;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 384) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 384;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 385) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 385;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 386) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 386;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 387) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 387;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 388) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 388;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 389) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 389;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 390) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 390;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 391) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 391;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 392) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 392;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 393) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 393;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 394) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 394;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 395) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 395;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 396) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 396;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 397) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 397;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 398) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 398;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 399) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 399;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*results)[({{_v[0-9]+}} as usize)] = ({{arg[0-9]+}} + 400) * 3 - 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
