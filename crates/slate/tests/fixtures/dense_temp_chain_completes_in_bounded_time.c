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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut results: __SlateAlign16<[i64; 400]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn compute(x: i64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[0] = (x + 1) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[1] = (x + 2) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[2] = (x + 3) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[3] = (x + 4) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[4] = (x + 5) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[5] = (x + 6) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[6] = (x + 7) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[7] = (x + 8) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[8] = (x + 9) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[9] = (x + 10) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[10] = (x + 11) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[11] = (x + 12) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[12] = (x + 13) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[13] = (x + 14) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[14] = (x + 15) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[15] = (x + 16) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[16] = (x + 17) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[17] = (x + 18) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[18] = (x + 19) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[19] = (x + 20) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[20] = (x + 21) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[21] = (x + 22) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[22] = (x + 23) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[23] = (x + 24) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[24] = (x + 25) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[25] = (x + 26) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[26] = (x + 27) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[27] = (x + 28) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[28] = (x + 29) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[29] = (x + 30) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[30] = (x + 31) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[31] = (x + 32) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[32] = (x + 33) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[33] = (x + 34) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[34] = (x + 35) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[35] = (x + 36) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[36] = (x + 37) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[37] = (x + 38) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[38] = (x + 39) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[39] = (x + 40) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[40] = (x + 41) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[41] = (x + 42) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[42] = (x + 43) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[43] = (x + 44) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[44] = (x + 45) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[45] = (x + 46) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[46] = (x + 47) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[47] = (x + 48) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[48] = (x + 49) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[49] = (x + 50) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[50] = (x + 51) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[51] = (x + 52) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[52] = (x + 53) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[53] = (x + 54) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[54] = (x + 55) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[55] = (x + 56) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[56] = (x + 57) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[57] = (x + 58) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[58] = (x + 59) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[59] = (x + 60) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[60] = (x + 61) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[61] = (x + 62) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[62] = (x + 63) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[63] = (x + 64) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[64] = (x + 65) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[65] = (x + 66) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[66] = (x + 67) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[67] = (x + 68) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[68] = (x + 69) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[69] = (x + 70) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[70] = (x + 71) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[71] = (x + 72) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[72] = (x + 73) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[73] = (x + 74) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[74] = (x + 75) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[75] = (x + 76) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[76] = (x + 77) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[77] = (x + 78) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[78] = (x + 79) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[79] = (x + 80) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[80] = (x + 81) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[81] = (x + 82) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[82] = (x + 83) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[83] = (x + 84) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[84] = (x + 85) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[85] = (x + 86) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[86] = (x + 87) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[87] = (x + 88) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[88] = (x + 89) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[89] = (x + 90) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[90] = (x + 91) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[91] = (x + 92) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[92] = (x + 93) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[93] = (x + 94) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[94] = (x + 95) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[95] = (x + 96) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[96] = (x + 97) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[97] = (x + 98) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[98] = (x + 99) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[99] = (x + 100) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[100] = (x + 101) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[101] = (x + 102) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[102] = (x + 103) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[103] = (x + 104) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[104] = (x + 105) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[105] = (x + 106) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[106] = (x + 107) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[107] = (x + 108) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[108] = (x + 109) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[109] = (x + 110) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[110] = (x + 111) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[111] = (x + 112) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[112] = (x + 113) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[113] = (x + 114) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[114] = (x + 115) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[115] = (x + 116) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[116] = (x + 117) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[117] = (x + 118) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[118] = (x + 119) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[119] = (x + 120) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[120] = (x + 121) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[121] = (x + 122) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[122] = (x + 123) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[123] = (x + 124) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[124] = (x + 125) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[125] = (x + 126) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[126] = (x + 127) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[127] = (x + 128) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[128] = (x + 129) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[129] = (x + 130) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[130] = (x + 131) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[131] = (x + 132) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[132] = (x + 133) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[133] = (x + 134) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[134] = (x + 135) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[135] = (x + 136) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[136] = (x + 137) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[137] = (x + 138) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[138] = (x + 139) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[139] = (x + 140) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[140] = (x + 141) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[141] = (x + 142) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[142] = (x + 143) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[143] = (x + 144) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[144] = (x + 145) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[145] = (x + 146) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[146] = (x + 147) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[147] = (x + 148) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[148] = (x + 149) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[149] = (x + 150) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[150] = (x + 151) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[151] = (x + 152) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[152] = (x + 153) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[153] = (x + 154) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[154] = (x + 155) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[155] = (x + 156) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[156] = (x + 157) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[157] = (x + 158) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[158] = (x + 159) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[159] = (x + 160) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[160] = (x + 161) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[161] = (x + 162) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[162] = (x + 163) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[163] = (x + 164) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[164] = (x + 165) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[165] = (x + 166) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[166] = (x + 167) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[167] = (x + 168) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[168] = (x + 169) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[169] = (x + 170) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[170] = (x + 171) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[171] = (x + 172) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[172] = (x + 173) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[173] = (x + 174) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[174] = (x + 175) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[175] = (x + 176) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[176] = (x + 177) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[177] = (x + 178) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[178] = (x + 179) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[179] = (x + 180) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[180] = (x + 181) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[181] = (x + 182) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[182] = (x + 183) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[183] = (x + 184) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[184] = (x + 185) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[185] = (x + 186) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[186] = (x + 187) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[187] = (x + 188) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[188] = (x + 189) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[189] = (x + 190) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[190] = (x + 191) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[191] = (x + 192) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[192] = (x + 193) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[193] = (x + 194) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[194] = (x + 195) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[195] = (x + 196) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[196] = (x + 197) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[197] = (x + 198) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[198] = (x + 199) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[199] = (x + 200) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[200] = (x + 201) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[201] = (x + 202) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[202] = (x + 203) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[203] = (x + 204) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[204] = (x + 205) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[205] = (x + 206) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[206] = (x + 207) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[207] = (x + 208) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[208] = (x + 209) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[209] = (x + 210) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[210] = (x + 211) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[211] = (x + 212) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[212] = (x + 213) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[213] = (x + 214) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[214] = (x + 215) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[215] = (x + 216) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[216] = (x + 217) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[217] = (x + 218) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[218] = (x + 219) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[219] = (x + 220) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[220] = (x + 221) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[221] = (x + 222) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[222] = (x + 223) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[223] = (x + 224) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[224] = (x + 225) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[225] = (x + 226) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[226] = (x + 227) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[227] = (x + 228) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[228] = (x + 229) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[229] = (x + 230) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[230] = (x + 231) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[231] = (x + 232) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[232] = (x + 233) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[233] = (x + 234) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[234] = (x + 235) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[235] = (x + 236) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[236] = (x + 237) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[237] = (x + 238) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[238] = (x + 239) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[239] = (x + 240) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[240] = (x + 241) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[241] = (x + 242) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[242] = (x + 243) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[243] = (x + 244) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[244] = (x + 245) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[245] = (x + 246) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[246] = (x + 247) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[247] = (x + 248) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[248] = (x + 249) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[249] = (x + 250) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[250] = (x + 251) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[251] = (x + 252) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[252] = (x + 253) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[253] = (x + 254) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[254] = (x + 255) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[255] = (x + 256) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[256] = (x + 257) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[257] = (x + 258) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[258] = (x + 259) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[259] = (x + 260) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[260] = (x + 261) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[261] = (x + 262) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[262] = (x + 263) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[263] = (x + 264) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[264] = (x + 265) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[265] = (x + 266) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[266] = (x + 267) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[267] = (x + 268) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[268] = (x + 269) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[269] = (x + 270) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[270] = (x + 271) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[271] = (x + 272) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[272] = (x + 273) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[273] = (x + 274) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[274] = (x + 275) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[275] = (x + 276) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[276] = (x + 277) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[277] = (x + 278) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[278] = (x + 279) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[279] = (x + 280) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[280] = (x + 281) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[281] = (x + 282) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[282] = (x + 283) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[283] = (x + 284) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[284] = (x + 285) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[285] = (x + 286) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[286] = (x + 287) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[287] = (x + 288) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[288] = (x + 289) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[289] = (x + 290) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[290] = (x + 291) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[291] = (x + 292) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[292] = (x + 293) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[293] = (x + 294) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[294] = (x + 295) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[295] = (x + 296) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[296] = (x + 297) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[297] = (x + 298) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[298] = (x + 299) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[299] = (x + 300) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[300] = (x + 301) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[301] = (x + 302) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[302] = (x + 303) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[303] = (x + 304) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[304] = (x + 305) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[305] = (x + 306) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[306] = (x + 307) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[307] = (x + 308) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[308] = (x + 309) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[309] = (x + 310) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[310] = (x + 311) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[311] = (x + 312) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[312] = (x + 313) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[313] = (x + 314) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[314] = (x + 315) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[315] = (x + 316) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[316] = (x + 317) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[317] = (x + 318) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[318] = (x + 319) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[319] = (x + 320) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[320] = (x + 321) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[321] = (x + 322) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[322] = (x + 323) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[323] = (x + 324) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[324] = (x + 325) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[325] = (x + 326) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[326] = (x + 327) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[327] = (x + 328) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[328] = (x + 329) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[329] = (x + 330) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[330] = (x + 331) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[331] = (x + 332) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[332] = (x + 333) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[333] = (x + 334) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[334] = (x + 335) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[335] = (x + 336) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[336] = (x + 337) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[337] = (x + 338) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[338] = (x + 339) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[339] = (x + 340) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[340] = (x + 341) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[341] = (x + 342) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[342] = (x + 343) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[343] = (x + 344) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[344] = (x + 345) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[345] = (x + 346) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[346] = (x + 347) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[347] = (x + 348) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[348] = (x + 349) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[349] = (x + 350) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[350] = (x + 351) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[351] = (x + 352) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[352] = (x + 353) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[353] = (x + 354) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[354] = (x + 355) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[355] = (x + 356) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[356] = (x + 357) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[357] = (x + 358) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[358] = (x + 359) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[359] = (x + 360) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[360] = (x + 361) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[361] = (x + 362) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[362] = (x + 363) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[363] = (x + 364) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[364] = (x + 365) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[365] = (x + 366) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[366] = (x + 367) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[367] = (x + 368) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[368] = (x + 369) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[369] = (x + 370) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[370] = (x + 371) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[371] = (x + 372) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[372] = (x + 373) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[373] = (x + 374) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[374] = (x + 375) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[375] = (x + 376) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[376] = (x + 377) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[377] = (x + 378) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[378] = (x + 379) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[379] = (x + 380) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[380] = (x + 381) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[381] = (x + 382) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[382] = (x + 383) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[383] = (x + 384) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[384] = (x + 385) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[385] = (x + 386) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[386] = (x + 387) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[387] = (x + 388) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[388] = (x + 389) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[389] = (x + 390) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[390] = (x + 391) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[391] = (x + 392) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[392] = (x + 393) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[393] = (x + 394) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[394] = (x + 395) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[395] = (x + 396) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[396] = (x + 397) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[397] = (x + 398) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[398] = (x + 399) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         results.0[399] = (x + 400) * 3 - 2;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     compute(1i64);
// CHECK-NEXT:     let mut sum: i64 = 0;
// CHECK-NEXT:     for i in 0i32..400i32 {
// CHECK-NEXT:         sum += unsafe { results.0[i as usize] };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), sum) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
