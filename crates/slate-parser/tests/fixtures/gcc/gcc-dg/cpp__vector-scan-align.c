/* Exercise the vectorised search_line_fast helpers in libcpp at every
   alignment they can be entered at: each declaration below puts a line
   splice, a stray '?' and a trigraph at a different column, sweeping
   past the widest vector any host implements.  */
/* { dg-do compile } */
/* { dg-options "-trigraphs -Wno-trigraphs" } */

int s0 = \
  0;
int s1 =  \
  1;
int s2 =   \
  2;
int s3 =    \
  3;
int s4 =     \
  4;
int s5 =      \
  5;
int s6 =       \
  6;
int s7 =        \
  7;
int s8 =         \
  8;
int s9 =          \
  9;
int s10 =           \
  10;
int s11 =            \
  11;
int s12 =             \
  12;
int s13 =              \
  13;
int s14 =               \
  14;
int s15 =                \
  15;
int s16 =                 \
  16;
int s17 =                  \
  17;
int s18 =                   \
  18;
int s19 =                    \
  19;
int s20 =                     \
  20;
int s21 =                      \
  21;
int s22 =                       \
  22;
int s23 =                        \
  23;
int s24 =                         \
  24;
int s25 =                          \
  25;
int s26 =                           \
  26;
int s27 =                            \
  27;
int s28 =                             \
  28;
int s29 =                              \
  29;
int s30 =                               \
  30;
int s31 =                                \
  31;
int s32 =                                 \
  32;
int s33 =                                  \
  33;
int s34 =                                   \
  34;
int s35 =                                    \
  35;
int s36 =                                     \
  36;
int s37 =                                      \
  37;
int s38 =                                       \
  38;
int s39 =                                        \
  39;
int s40 =                                         \
  40;
int s41 =                                          \
  41;
int s42 =                                           \
  42;
int s43 =                                            \
  43;
int s44 =                                             \
  44;
int s45 =                                              \
  45;
int s46 =                                               \
  46;
int s47 =                                                \
  47;
int s48 =                                                 \
  48;
int s49 =                                                  \
  49;
int s50 =                                                   \
  50;
int s51 =                                                    \
  51;
int s52 =                                                     \
  52;
int s53 =                                                      \
  53;
int s54 =                                                       \
  54;
int s55 =                                                        \
  55;
int s56 =                                                         \
  56;
int s57 =                                                          \
  57;
int s58 =                                                           \
  58;
int s59 =                                                            \
  59;
int s60 =                                                             \
  60;
int s61 =                                                              \
  61;
int s62 =                                                               \
  62;
int s63 =                                                                \
  63;
int s64 =                                                                 \
  64;
int s65 =                                                                  \
  65;
int s66 =                                                                   \
  66;
int s67 =                                                                    \
  67;
int s68 =                                                                     \
  68;
int s69 =                                                                      \
  69;
int s70 =                                                                       \
  70;
int s71 =                                                                        \
  71;
int s72 =                                                                         \
  72;
int s73 =                                                                          \
  73;
int s74 =                                                                           \
  74;
int s75 =                                                                            \
  75;
int s76 =                                                                             \
  76;
int s77 =                                                                              \
  77;
int s78 =                                                                               \
  78;
int s79 =                                                                                \
  79;
int s80 =                                                                                 \
  80;
int s81 =                                                                                  \
  81;
int s82 =                                                                                   \
  82;
int s83 =                                                                                    \
  83;
int s84 =                                                                                     \
  84;
int s85 =                                                                                      \
  85;
int s86 =                                                                                       \
  86;
int s87 =                                                                                        \
  87;
int s88 =                                                                                         \
  88;
int s89 =                                                                                          \
  89;
int s90 =                                                                                           \
  90;
int s91 =                                                                                            \
  91;
int s92 =                                                                                             \
  92;
int s93 =                                                                                              \
  93;
int s94 =                                                                                               \
  94;
int s95 =                                                                                                \
  95;
int s96 =                                                                                                 \
  96;
int s97 =                                                                                                  \
  97;
int s98 =                                                                                                   \
  98;
int s99 =                                                                                                    \
  99;
int s100 =                                                                                                     \
  100;
int s101 =                                                                                                      \
  101;
int s102 =                                                                                                       \
  102;
int s103 =                                                                                                        \
  103;
int s104 =                                                                                                         \
  104;
int s105 =                                                                                                          \
  105;
int s106 =                                                                                                           \
  106;
int s107 =                                                                                                            \
  107;
int s108 =                                                                                                             \
  108;
int s109 =                                                                                                              \
  109;
int s110 =                                                                                                               \
  110;
int s111 =                                                                                                                \
  111;
int s112 =                                                                                                                 \
  112;
int s113 =                                                                                                                  \
  113;
int s114 =                                                                                                                   \
  114;
int s115 =                                                                                                                    \
  115;
int s116 =                                                                                                                     \
  116;
int s117 =                                                                                                                      \
  117;
int s118 =                                                                                                                       \
  118;
int s119 =                                                                                                                        \
  119;
int s120 =                                                                                                                         \
  120;
int s121 =                                                                                                                          \
  121;
int s122 =                                                                                                                           \
  122;
int s123 =                                                                                                                            \
  123;
int s124 =                                                                                                                             \
  124;
int s125 =                                                                                                                              \
  125;
int s126 =                                                                                                                               \
  126;
int s127 =                                                                                                                                \
  127;
int s128 =                                                                                                                                 \
  128;
int s129 =                                                                                                                                  \
  129;
int s130 =                                                                                                                                   \
  130;
int s131 =                                                                                                                                    \
  131;
int s132 =                                                                                                                                     \
  132;
int s133 =                                                                                                                                      \
  133;
int s134 =                                                                                                                                       \
  134;
int s135 =                                                                                                                                        \
  135;
int s136 =                                                                                                                                         \
  136;
int s137 =                                                                                                                                          \
  137;
int s138 =                                                                                                                                           \
  138;
int s139 =                                                                                                                                            \
  139;
int s140 =                                                                                                                                             \
  140;
int s141 =                                                                                                                                              \
  141;
int s142 =                                                                                                                                               \
  142;
int s143 =                                                                                                                                                \
  143;
int s144 =                                                                                                                                                 \
  144;
int s145 =                                                                                                                                                  \
  145;
int s146 =                                                                                                                                                   \
  146;
int s147 =                                                                                                                                                    \
  147;
int s148 =                                                                                                                                                     \
  148;
int s149 =                                                                                                                                                      \
  149;
int s150 =                                                                                                                                                       \
  150;
int s151 =                                                                                                                                                        \
  151;
int s152 =                                                                                                                                                         \
  152;
int s153 =                                                                                                                                                          \
  153;
int s154 =                                                                                                                                                           \
  154;
int s155 =                                                                                                                                                            \
  155;
int s156 =                                                                                                                                                             \
  156;
int s157 =                                                                                                                                                              \
  157;
int s158 =                                                                                                                                                               \
  158;
int s159 =                                                                                                                                                                \
  159;
int s160 =                                                                                                                                                                 \
  160;
int s161 =                                                                                                                                                                  \
  161;
int s162 =                                                                                                                                                                   \
  162;
int s163 =                                                                                                                                                                    \
  163;
int s164 =                                                                                                                                                                     \
  164;
int s165 =                                                                                                                                                                      \
  165;
int s166 =                                                                                                                                                                       \
  166;
int s167 =                                                                                                                                                                        \
  167;
int s168 =                                                                                                                                                                         \
  168;
int s169 =                                                                                                                                                                          \
  169;
int s170 =                                                                                                                                                                           \
  170;
int s171 =                                                                                                                                                                            \
  171;
int s172 =                                                                                                                                                                             \
  172;
int s173 =                                                                                                                                                                              \
  173;
int s174 =                                                                                                                                                                               \
  174;
int s175 =                                                                                                                                                                                \
  175;
int s176 =                                                                                                                                                                                 \
  176;
int s177 =                                                                                                                                                                                  \
  177;
int s178 =                                                                                                                                                                                   \
  178;
int s179 =                                                                                                                                                                                    \
  179;
int s180 =                                                                                                                                                                                     \
  180;
int s181 =                                                                                                                                                                                      \
  181;
int s182 =                                                                                                                                                                                       \
  182;
int s183 =                                                                                                                                                                                        \
  183;
int s184 =                                                                                                                                                                                         \
  184;
int s185 =                                                                                                                                                                                          \
  185;
int s186 =                                                                                                                                                                                           \
  186;
int s187 =                                                                                                                                                                                            \
  187;
int s188 =                                                                                                                                                                                             \
  188;
int s189 =                                                                                                                                                                                              \
  189;
int s190 =                                                                                                                                                                                               \
  190;
int s191 =                                                                                                                                                                                                \
  191;
int s192 =                                                                                                                                                                                                 \
  192;
int s193 =                                                                                                                                                                                                  \
  193;
int s194 =                                                                                                                                                                                                   \
  194;
int s195 =                                                                                                                                                                                                    \
  195;
int s196 =                                                                                                                                                                                                     \
  196;
int s197 =                                                                                                                                                                                                      \
  197;
int s198 =                                                                                                                                                                                                       \
  198;
int s199 =                                                                                                                                                                                                        \
  199;
int s200 =                                                                                                                                                                                                         \
  200;
int s201 =                                                                                                                                                                                                          \
  201;
int s202 =                                                                                                                                                                                                           \
  202;
int s203 =                                                                                                                                                                                                            \
  203;
int s204 =                                                                                                                                                                                                             \
  204;
int s205 =                                                                                                                                                                                                              \
  205;
int s206 =                                                                                                                                                                                                               \
  206;
int s207 =                                                                                                                                                                                                                \
  207;
int s208 =                                                                                                                                                                                                                 \
  208;
int s209 =                                                                                                                                                                                                                  \
  209;
int s210 =                                                                                                                                                                                                                   \
  210;
int s211 =                                                                                                                                                                                                                    \
  211;
int s212 =                                                                                                                                                                                                                     \
  212;
int s213 =                                                                                                                                                                                                                      \
  213;
int s214 =                                                                                                                                                                                                                       \
  214;
int s215 =                                                                                                                                                                                                                        \
  215;
int s216 =                                                                                                                                                                                                                         \
  216;
int s217 =                                                                                                                                                                                                                          \
  217;
int s218 =                                                                                                                                                                                                                           \
  218;
int s219 =                                                                                                                                                                                                                            \
  219;
int s220 =                                                                                                                                                                                                                             \
  220;
int s221 =                                                                                                                                                                                                                              \
  221;
int s222 =                                                                                                                                                                                                                               \
  222;
int s223 =                                                                                                                                                                                                                                \
  223;
int s224 =                                                                                                                                                                                                                                 \
  224;
int s225 =                                                                                                                                                                                                                                  \
  225;
int s226 =                                                                                                                                                                                                                                   \
  226;
int s227 =                                                                                                                                                                                                                                    \
  227;
int s228 =                                                                                                                                                                                                                                     \
  228;
int s229 =                                                                                                                                                                                                                                      \
  229;
int s230 =                                                                                                                                                                                                                                       \
  230;
int s231 =                                                                                                                                                                                                                                        \
  231;
int s232 =                                                                                                                                                                                                                                         \
  232;
int s233 =                                                                                                                                                                                                                                          \
  233;
int s234 =                                                                                                                                                                                                                                           \
  234;
int s235 =                                                                                                                                                                                                                                            \
  235;
int s236 =                                                                                                                                                                                                                                             \
  236;
int s237 =                                                                                                                                                                                                                                              \
  237;
int s238 =                                                                                                                                                                                                                                               \
  238;
int s239 =                                                                                                                                                                                                                                                \
  239;
int s240 =                                                                                                                                                                                                                                                 \
  240;
int s241 =                                                                                                                                                                                                                                                  \
  241;
int s242 =                                                                                                                                                                                                                                                   \
  242;
int s243 =                                                                                                                                                                                                                                                    \
  243;
int s244 =                                                                                                                                                                                                                                                     \
  244;
int s245 =                                                                                                                                                                                                                                                      \
  245;
int s246 =                                                                                                                                                                                                                                                       \
  246;
int s247 =                                                                                                                                                                                                                                                        \
  247;
int s248 =                                                                                                                                                                                                                                                         \
  248;
int s249 =                                                                                                                                                                                                                                                          \
  249;
int s250 =                                                                                                                                                                                                                                                           \
  250;
int s251 =                                                                                                                                                                                                                                                            \
  251;
int s252 =                                                                                                                                                                                                                                                             \
  252;
int s253 =                                                                                                                                                                                                                                                              \
  253;
int s254 =                                                                                                                                                                                                                                                               \
  254;
int s255 =                                                                                                                                                                                                                                                                \
  255;
int s256 =                                                                                                                                                                                                                                                                 \
  256;
int s257 =                                                                                                                                                                                                                                                                  \
  257;
int s258 =                                                                                                                                                                                                                                                                   \
  258;
int s259 =                                                                                                                                                                                                                                                                    \
  259;
int s260 =                                                                                                                                                                                                                                                                     \
  260;
int s261 =                                                                                                                                                                                                                                                                      \
  261;
int s262 =                                                                                                                                                                                                                                                                       \
  262;
int s263 =                                                                                                                                                                                                                                                                        \
  263;
int s264 =                                                                                                                                                                                                                                                                         \
  264;
int s265 =                                                                                                                                                                                                                                                                          \
  265;
int s266 =                                                                                                                                                                                                                                                                           \
  266;
int s267 =                                                                                                                                                                                                                                                                            \
  267;
int s268 =                                                                                                                                                                                                                                                                             \
  268;
int s269 =                                                                                                                                                                                                                                                                              \
  269;
int s270 =                                                                                                                                                                                                                                                                               \
  270;
int s271 =                                                                                                                                                                                                                                                                                \
  271;
int s272 =                                                                                                                                                                                                                                                                                 \
  272;
int s273 =                                                                                                                                                                                                                                                                                  \
  273;
int s274 =                                                                                                                                                                                                                                                                                   \
  274;
int s275 =                                                                                                                                                                                                                                                                                    \
  275;
int s276 =                                                                                                                                                                                                                                                                                     \
  276;
int s277 =                                                                                                                                                                                                                                                                                      \
  277;
int s278 =                                                                                                                                                                                                                                                                                       \
  278;
int s279 =                                                                                                                                                                                                                                                                                        \
  279;
int s280 =                                                                                                                                                                                                                                                                                         \
  280;
int s281 =                                                                                                                                                                                                                                                                                          \
  281;
int s282 =                                                                                                                                                                                                                                                                                           \
  282;
int s283 =                                                                                                                                                                                                                                                                                            \
  283;
int s284 =                                                                                                                                                                                                                                                                                             \
  284;
int s285 =                                                                                                                                                                                                                                                                                              \
  285;
int s286 =                                                                                                                                                                                                                                                                                               \
  286;
int s287 =                                                                                                                                                                                                                                                                                                \
  287;
int s288 =                                                                                                                                                                                                                                                                                                 \
  288;
int s289 =                                                                                                                                                                                                                                                                                                  \
  289;
int s290 =                                                                                                                                                                                                                                                                                                   \
  290;
int s291 =                                                                                                                                                                                                                                                                                                    \
  291;
int s292 =                                                                                                                                                                                                                                                                                                     \
  292;
int s293 =                                                                                                                                                                                                                                                                                                      \
  293;
int s294 =                                                                                                                                                                                                                                                                                                       \
  294;
int s295 =                                                                                                                                                                                                                                                                                                        \
  295;
int s296 =                                                                                                                                                                                                                                                                                                         \
  296;
int s297 =                                                                                                                                                                                                                                                                                                          \
  297;
int s298 =                                                                                                                                                                                                                                                                                                           \
  298;
int s299 =                                                                                                                                                                                                                                                                                                            \
  299;
/*  is this a trigraph? no ??' */
/* x is this a trigraph? no ??' */
/* xx is this a trigraph? no ??' */
/* xxx is this a trigraph? no ??' */
/* xxxx is this a trigraph? no ??' */
/* xxxxx is this a trigraph? no ??' */
/* xxxxxx is this a trigraph? no ??' */
/* xxxxxxx is this a trigraph? no ??' */
/* xxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */
/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx is this a trigraph? no ??' */

int main (void) { return s0 + s299; }

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %0 s0: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %1 s1: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %2 s2: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %3 s3: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %4 s4: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %5 s5: i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %6 s6: i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %7 s7: i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %8 s8: i32 [storage=static] = const<i32>(8) [linkage=external];
// DEFAULT-NEXT:     global %9 s9: i32 [storage=static] = const<i32>(9) [linkage=external];
// DEFAULT-NEXT:     global %10 s10: i32 [storage=static] = const<i32>(10) [linkage=external];
// DEFAULT-NEXT:     global %11 s11: i32 [storage=static] = const<i32>(11) [linkage=external];
// DEFAULT-NEXT:     global %12 s12: i32 [storage=static] = const<i32>(12) [linkage=external];
// DEFAULT-NEXT:     global %13 s13: i32 [storage=static] = const<i32>(13) [linkage=external];
// DEFAULT-NEXT:     global %14 s14: i32 [storage=static] = const<i32>(14) [linkage=external];
// DEFAULT-NEXT:     global %15 s15: i32 [storage=static] = const<i32>(15) [linkage=external];
// DEFAULT-NEXT:     global %16 s16: i32 [storage=static] = const<i32>(16) [linkage=external];
// DEFAULT-NEXT:     global %17 s17: i32 [storage=static] = const<i32>(17) [linkage=external];
// DEFAULT-NEXT:     global %18 s18: i32 [storage=static] = const<i32>(18) [linkage=external];
// DEFAULT-NEXT:     global %19 s19: i32 [storage=static] = const<i32>(19) [linkage=external];
// DEFAULT-NEXT:     global %20 s20: i32 [storage=static] = const<i32>(20) [linkage=external];
// DEFAULT-NEXT:     global %21 s21: i32 [storage=static] = const<i32>(21) [linkage=external];
// DEFAULT-NEXT:     global %22 s22: i32 [storage=static] = const<i32>(22) [linkage=external];
// DEFAULT-NEXT:     global %23 s23: i32 [storage=static] = const<i32>(23) [linkage=external];
// DEFAULT-NEXT:     global %24 s24: i32 [storage=static] = const<i32>(24) [linkage=external];
// DEFAULT-NEXT:     global %25 s25: i32 [storage=static] = const<i32>(25) [linkage=external];
// DEFAULT-NEXT:     global %26 s26: i32 [storage=static] = const<i32>(26) [linkage=external];
// DEFAULT-NEXT:     global %27 s27: i32 [storage=static] = const<i32>(27) [linkage=external];
// DEFAULT-NEXT:     global %28 s28: i32 [storage=static] = const<i32>(28) [linkage=external];
// DEFAULT-NEXT:     global %29 s29: i32 [storage=static] = const<i32>(29) [linkage=external];
// DEFAULT-NEXT:     global %30 s30: i32 [storage=static] = const<i32>(30) [linkage=external];
// DEFAULT-NEXT:     global %31 s31: i32 [storage=static] = const<i32>(31) [linkage=external];
// DEFAULT-NEXT:     global %32 s32: i32 [storage=static] = const<i32>(32) [linkage=external];
// DEFAULT-NEXT:     global %33 s33: i32 [storage=static] = const<i32>(33) [linkage=external];
// DEFAULT-NEXT:     global %34 s34: i32 [storage=static] = const<i32>(34) [linkage=external];
// DEFAULT-NEXT:     global %35 s35: i32 [storage=static] = const<i32>(35) [linkage=external];
// DEFAULT-NEXT:     global %36 s36: i32 [storage=static] = const<i32>(36) [linkage=external];
// DEFAULT-NEXT:     global %37 s37: i32 [storage=static] = const<i32>(37) [linkage=external];
// DEFAULT-NEXT:     global %38 s38: i32 [storage=static] = const<i32>(38) [linkage=external];
// DEFAULT-NEXT:     global %39 s39: i32 [storage=static] = const<i32>(39) [linkage=external];
// DEFAULT-NEXT:     global %40 s40: i32 [storage=static] = const<i32>(40) [linkage=external];
// DEFAULT-NEXT:     global %41 s41: i32 [storage=static] = const<i32>(41) [linkage=external];
// DEFAULT-NEXT:     global %42 s42: i32 [storage=static] = const<i32>(42) [linkage=external];
// DEFAULT-NEXT:     global %43 s43: i32 [storage=static] = const<i32>(43) [linkage=external];
// DEFAULT-NEXT:     global %44 s44: i32 [storage=static] = const<i32>(44) [linkage=external];
// DEFAULT-NEXT:     global %45 s45: i32 [storage=static] = const<i32>(45) [linkage=external];
// DEFAULT-NEXT:     global %46 s46: i32 [storage=static] = const<i32>(46) [linkage=external];
// DEFAULT-NEXT:     global %47 s47: i32 [storage=static] = const<i32>(47) [linkage=external];
// DEFAULT-NEXT:     global %48 s48: i32 [storage=static] = const<i32>(48) [linkage=external];
// DEFAULT-NEXT:     global %49 s49: i32 [storage=static] = const<i32>(49) [linkage=external];
// DEFAULT-NEXT:     global %50 s50: i32 [storage=static] = const<i32>(50) [linkage=external];
// DEFAULT-NEXT:     global %51 s51: i32 [storage=static] = const<i32>(51) [linkage=external];
// DEFAULT-NEXT:     global %52 s52: i32 [storage=static] = const<i32>(52) [linkage=external];
// DEFAULT-NEXT:     global %53 s53: i32 [storage=static] = const<i32>(53) [linkage=external];
// DEFAULT-NEXT:     global %54 s54: i32 [storage=static] = const<i32>(54) [linkage=external];
// DEFAULT-NEXT:     global %55 s55: i32 [storage=static] = const<i32>(55) [linkage=external];
// DEFAULT-NEXT:     global %56 s56: i32 [storage=static] = const<i32>(56) [linkage=external];
// DEFAULT-NEXT:     global %57 s57: i32 [storage=static] = const<i32>(57) [linkage=external];
// DEFAULT-NEXT:     global %58 s58: i32 [storage=static] = const<i32>(58) [linkage=external];
// DEFAULT-NEXT:     global %59 s59: i32 [storage=static] = const<i32>(59) [linkage=external];
// DEFAULT-NEXT:     global %60 s60: i32 [storage=static] = const<i32>(60) [linkage=external];
// DEFAULT-NEXT:     global %61 s61: i32 [storage=static] = const<i32>(61) [linkage=external];
// DEFAULT-NEXT:     global %62 s62: i32 [storage=static] = const<i32>(62) [linkage=external];
// DEFAULT-NEXT:     global %63 s63: i32 [storage=static] = const<i32>(63) [linkage=external];
// DEFAULT-NEXT:     global %64 s64: i32 [storage=static] = const<i32>(64) [linkage=external];
// DEFAULT-NEXT:     global %65 s65: i32 [storage=static] = const<i32>(65) [linkage=external];
// DEFAULT-NEXT:     global %66 s66: i32 [storage=static] = const<i32>(66) [linkage=external];
// DEFAULT-NEXT:     global %67 s67: i32 [storage=static] = const<i32>(67) [linkage=external];
// DEFAULT-NEXT:     global %68 s68: i32 [storage=static] = const<i32>(68) [linkage=external];
// DEFAULT-NEXT:     global %69 s69: i32 [storage=static] = const<i32>(69) [linkage=external];
// DEFAULT-NEXT:     global %70 s70: i32 [storage=static] = const<i32>(70) [linkage=external];
// DEFAULT-NEXT:     global %71 s71: i32 [storage=static] = const<i32>(71) [linkage=external];
// DEFAULT-NEXT:     global %72 s72: i32 [storage=static] = const<i32>(72) [linkage=external];
// DEFAULT-NEXT:     global %73 s73: i32 [storage=static] = const<i32>(73) [linkage=external];
// DEFAULT-NEXT:     global %74 s74: i32 [storage=static] = const<i32>(74) [linkage=external];
// DEFAULT-NEXT:     global %75 s75: i32 [storage=static] = const<i32>(75) [linkage=external];
// DEFAULT-NEXT:     global %76 s76: i32 [storage=static] = const<i32>(76) [linkage=external];
// DEFAULT-NEXT:     global %77 s77: i32 [storage=static] = const<i32>(77) [linkage=external];
// DEFAULT-NEXT:     global %78 s78: i32 [storage=static] = const<i32>(78) [linkage=external];
// DEFAULT-NEXT:     global %79 s79: i32 [storage=static] = const<i32>(79) [linkage=external];
// DEFAULT-NEXT:     global %80 s80: i32 [storage=static] = const<i32>(80) [linkage=external];
// DEFAULT-NEXT:     global %81 s81: i32 [storage=static] = const<i32>(81) [linkage=external];
// DEFAULT-NEXT:     global %82 s82: i32 [storage=static] = const<i32>(82) [linkage=external];
// DEFAULT-NEXT:     global %83 s83: i32 [storage=static] = const<i32>(83) [linkage=external];
// DEFAULT-NEXT:     global %84 s84: i32 [storage=static] = const<i32>(84) [linkage=external];
// DEFAULT-NEXT:     global %85 s85: i32 [storage=static] = const<i32>(85) [linkage=external];
// DEFAULT-NEXT:     global %86 s86: i32 [storage=static] = const<i32>(86) [linkage=external];
// DEFAULT-NEXT:     global %87 s87: i32 [storage=static] = const<i32>(87) [linkage=external];
// DEFAULT-NEXT:     global %88 s88: i32 [storage=static] = const<i32>(88) [linkage=external];
// DEFAULT-NEXT:     global %89 s89: i32 [storage=static] = const<i32>(89) [linkage=external];
// DEFAULT-NEXT:     global %90 s90: i32 [storage=static] = const<i32>(90) [linkage=external];
// DEFAULT-NEXT:     global %91 s91: i32 [storage=static] = const<i32>(91) [linkage=external];
// DEFAULT-NEXT:     global %92 s92: i32 [storage=static] = const<i32>(92) [linkage=external];
// DEFAULT-NEXT:     global %93 s93: i32 [storage=static] = const<i32>(93) [linkage=external];
// DEFAULT-NEXT:     global %94 s94: i32 [storage=static] = const<i32>(94) [linkage=external];
// DEFAULT-NEXT:     global %95 s95: i32 [storage=static] = const<i32>(95) [linkage=external];
// DEFAULT-NEXT:     global %96 s96: i32 [storage=static] = const<i32>(96) [linkage=external];
// DEFAULT-NEXT:     global %97 s97: i32 [storage=static] = const<i32>(97) [linkage=external];
// DEFAULT-NEXT:     global %98 s98: i32 [storage=static] = const<i32>(98) [linkage=external];
// DEFAULT-NEXT:     global %99 s99: i32 [storage=static] = const<i32>(99) [linkage=external];
// DEFAULT-NEXT:     global %100 s100: i32 [storage=static] = const<i32>(100) [linkage=external];
// DEFAULT-NEXT:     global %101 s101: i32 [storage=static] = const<i32>(101) [linkage=external];
// DEFAULT-NEXT:     global %102 s102: i32 [storage=static] = const<i32>(102) [linkage=external];
// DEFAULT-NEXT:     global %103 s103: i32 [storage=static] = const<i32>(103) [linkage=external];
// DEFAULT-NEXT:     global %104 s104: i32 [storage=static] = const<i32>(104) [linkage=external];
// DEFAULT-NEXT:     global %105 s105: i32 [storage=static] = const<i32>(105) [linkage=external];
// DEFAULT-NEXT:     global %106 s106: i32 [storage=static] = const<i32>(106) [linkage=external];
// DEFAULT-NEXT:     global %107 s107: i32 [storage=static] = const<i32>(107) [linkage=external];
// DEFAULT-NEXT:     global %108 s108: i32 [storage=static] = const<i32>(108) [linkage=external];
// DEFAULT-NEXT:     global %109 s109: i32 [storage=static] = const<i32>(109) [linkage=external];
// DEFAULT-NEXT:     global %110 s110: i32 [storage=static] = const<i32>(110) [linkage=external];
// DEFAULT-NEXT:     global %111 s111: i32 [storage=static] = const<i32>(111) [linkage=external];
// DEFAULT-NEXT:     global %112 s112: i32 [storage=static] = const<i32>(112) [linkage=external];
// DEFAULT-NEXT:     global %113 s113: i32 [storage=static] = const<i32>(113) [linkage=external];
// DEFAULT-NEXT:     global %114 s114: i32 [storage=static] = const<i32>(114) [linkage=external];
// DEFAULT-NEXT:     global %115 s115: i32 [storage=static] = const<i32>(115) [linkage=external];
// DEFAULT-NEXT:     global %116 s116: i32 [storage=static] = const<i32>(116) [linkage=external];
// DEFAULT-NEXT:     global %117 s117: i32 [storage=static] = const<i32>(117) [linkage=external];
// DEFAULT-NEXT:     global %118 s118: i32 [storage=static] = const<i32>(118) [linkage=external];
// DEFAULT-NEXT:     global %119 s119: i32 [storage=static] = const<i32>(119) [linkage=external];
// DEFAULT-NEXT:     global %120 s120: i32 [storage=static] = const<i32>(120) [linkage=external];
// DEFAULT-NEXT:     global %121 s121: i32 [storage=static] = const<i32>(121) [linkage=external];
// DEFAULT-NEXT:     global %122 s122: i32 [storage=static] = const<i32>(122) [linkage=external];
// DEFAULT-NEXT:     global %123 s123: i32 [storage=static] = const<i32>(123) [linkage=external];
// DEFAULT-NEXT:     global %124 s124: i32 [storage=static] = const<i32>(124) [linkage=external];
// DEFAULT-NEXT:     global %125 s125: i32 [storage=static] = const<i32>(125) [linkage=external];
// DEFAULT-NEXT:     global %126 s126: i32 [storage=static] = const<i32>(126) [linkage=external];
// DEFAULT-NEXT:     global %127 s127: i32 [storage=static] = const<i32>(127) [linkage=external];
// DEFAULT-NEXT:     global %128 s128: i32 [storage=static] = const<i32>(128) [linkage=external];
// DEFAULT-NEXT:     global %129 s129: i32 [storage=static] = const<i32>(129) [linkage=external];
// DEFAULT-NEXT:     global %130 s130: i32 [storage=static] = const<i32>(130) [linkage=external];
// DEFAULT-NEXT:     global %131 s131: i32 [storage=static] = const<i32>(131) [linkage=external];
// DEFAULT-NEXT:     global %132 s132: i32 [storage=static] = const<i32>(132) [linkage=external];
// DEFAULT-NEXT:     global %133 s133: i32 [storage=static] = const<i32>(133) [linkage=external];
// DEFAULT-NEXT:     global %134 s134: i32 [storage=static] = const<i32>(134) [linkage=external];
// DEFAULT-NEXT:     global %135 s135: i32 [storage=static] = const<i32>(135) [linkage=external];
// DEFAULT-NEXT:     global %136 s136: i32 [storage=static] = const<i32>(136) [linkage=external];
// DEFAULT-NEXT:     global %137 s137: i32 [storage=static] = const<i32>(137) [linkage=external];
// DEFAULT-NEXT:     global %138 s138: i32 [storage=static] = const<i32>(138) [linkage=external];
// DEFAULT-NEXT:     global %139 s139: i32 [storage=static] = const<i32>(139) [linkage=external];
// DEFAULT-NEXT:     global %140 s140: i32 [storage=static] = const<i32>(140) [linkage=external];
// DEFAULT-NEXT:     global %141 s141: i32 [storage=static] = const<i32>(141) [linkage=external];
// DEFAULT-NEXT:     global %142 s142: i32 [storage=static] = const<i32>(142) [linkage=external];
// DEFAULT-NEXT:     global %143 s143: i32 [storage=static] = const<i32>(143) [linkage=external];
// DEFAULT-NEXT:     global %144 s144: i32 [storage=static] = const<i32>(144) [linkage=external];
// DEFAULT-NEXT:     global %145 s145: i32 [storage=static] = const<i32>(145) [linkage=external];
// DEFAULT-NEXT:     global %146 s146: i32 [storage=static] = const<i32>(146) [linkage=external];
// DEFAULT-NEXT:     global %147 s147: i32 [storage=static] = const<i32>(147) [linkage=external];
// DEFAULT-NEXT:     global %148 s148: i32 [storage=static] = const<i32>(148) [linkage=external];
// DEFAULT-NEXT:     global %149 s149: i32 [storage=static] = const<i32>(149) [linkage=external];
// DEFAULT-NEXT:     global %150 s150: i32 [storage=static] = const<i32>(150) [linkage=external];
// DEFAULT-NEXT:     global %151 s151: i32 [storage=static] = const<i32>(151) [linkage=external];
// DEFAULT-NEXT:     global %152 s152: i32 [storage=static] = const<i32>(152) [linkage=external];
// DEFAULT-NEXT:     global %153 s153: i32 [storage=static] = const<i32>(153) [linkage=external];
// DEFAULT-NEXT:     global %154 s154: i32 [storage=static] = const<i32>(154) [linkage=external];
// DEFAULT-NEXT:     global %155 s155: i32 [storage=static] = const<i32>(155) [linkage=external];
// DEFAULT-NEXT:     global %156 s156: i32 [storage=static] = const<i32>(156) [linkage=external];
// DEFAULT-NEXT:     global %157 s157: i32 [storage=static] = const<i32>(157) [linkage=external];
// DEFAULT-NEXT:     global %158 s158: i32 [storage=static] = const<i32>(158) [linkage=external];
// DEFAULT-NEXT:     global %159 s159: i32 [storage=static] = const<i32>(159) [linkage=external];
// DEFAULT-NEXT:     global %160 s160: i32 [storage=static] = const<i32>(160) [linkage=external];
// DEFAULT-NEXT:     global %161 s161: i32 [storage=static] = const<i32>(161) [linkage=external];
// DEFAULT-NEXT:     global %162 s162: i32 [storage=static] = const<i32>(162) [linkage=external];
// DEFAULT-NEXT:     global %163 s163: i32 [storage=static] = const<i32>(163) [linkage=external];
// DEFAULT-NEXT:     global %164 s164: i32 [storage=static] = const<i32>(164) [linkage=external];
// DEFAULT-NEXT:     global %165 s165: i32 [storage=static] = const<i32>(165) [linkage=external];
// DEFAULT-NEXT:     global %166 s166: i32 [storage=static] = const<i32>(166) [linkage=external];
// DEFAULT-NEXT:     global %167 s167: i32 [storage=static] = const<i32>(167) [linkage=external];
// DEFAULT-NEXT:     global %168 s168: i32 [storage=static] = const<i32>(168) [linkage=external];
// DEFAULT-NEXT:     global %169 s169: i32 [storage=static] = const<i32>(169) [linkage=external];
// DEFAULT-NEXT:     global %170 s170: i32 [storage=static] = const<i32>(170) [linkage=external];
// DEFAULT-NEXT:     global %171 s171: i32 [storage=static] = const<i32>(171) [linkage=external];
// DEFAULT-NEXT:     global %172 s172: i32 [storage=static] = const<i32>(172) [linkage=external];
// DEFAULT-NEXT:     global %173 s173: i32 [storage=static] = const<i32>(173) [linkage=external];
// DEFAULT-NEXT:     global %174 s174: i32 [storage=static] = const<i32>(174) [linkage=external];
// DEFAULT-NEXT:     global %175 s175: i32 [storage=static] = const<i32>(175) [linkage=external];
// DEFAULT-NEXT:     global %176 s176: i32 [storage=static] = const<i32>(176) [linkage=external];
// DEFAULT-NEXT:     global %177 s177: i32 [storage=static] = const<i32>(177) [linkage=external];
// DEFAULT-NEXT:     global %178 s178: i32 [storage=static] = const<i32>(178) [linkage=external];
// DEFAULT-NEXT:     global %179 s179: i32 [storage=static] = const<i32>(179) [linkage=external];
// DEFAULT-NEXT:     global %180 s180: i32 [storage=static] = const<i32>(180) [linkage=external];
// DEFAULT-NEXT:     global %181 s181: i32 [storage=static] = const<i32>(181) [linkage=external];
// DEFAULT-NEXT:     global %182 s182: i32 [storage=static] = const<i32>(182) [linkage=external];
// DEFAULT-NEXT:     global %183 s183: i32 [storage=static] = const<i32>(183) [linkage=external];
// DEFAULT-NEXT:     global %184 s184: i32 [storage=static] = const<i32>(184) [linkage=external];
// DEFAULT-NEXT:     global %185 s185: i32 [storage=static] = const<i32>(185) [linkage=external];
// DEFAULT-NEXT:     global %186 s186: i32 [storage=static] = const<i32>(186) [linkage=external];
// DEFAULT-NEXT:     global %187 s187: i32 [storage=static] = const<i32>(187) [linkage=external];
// DEFAULT-NEXT:     global %188 s188: i32 [storage=static] = const<i32>(188) [linkage=external];
// DEFAULT-NEXT:     global %189 s189: i32 [storage=static] = const<i32>(189) [linkage=external];
// DEFAULT-NEXT:     global %190 s190: i32 [storage=static] = const<i32>(190) [linkage=external];
// DEFAULT-NEXT:     global %191 s191: i32 [storage=static] = const<i32>(191) [linkage=external];
// DEFAULT-NEXT:     global %192 s192: i32 [storage=static] = const<i32>(192) [linkage=external];
// DEFAULT-NEXT:     global %193 s193: i32 [storage=static] = const<i32>(193) [linkage=external];
// DEFAULT-NEXT:     global %194 s194: i32 [storage=static] = const<i32>(194) [linkage=external];
// DEFAULT-NEXT:     global %195 s195: i32 [storage=static] = const<i32>(195) [linkage=external];
// DEFAULT-NEXT:     global %196 s196: i32 [storage=static] = const<i32>(196) [linkage=external];
// DEFAULT-NEXT:     global %197 s197: i32 [storage=static] = const<i32>(197) [linkage=external];
// DEFAULT-NEXT:     global %198 s198: i32 [storage=static] = const<i32>(198) [linkage=external];
// DEFAULT-NEXT:     global %199 s199: i32 [storage=static] = const<i32>(199) [linkage=external];
// DEFAULT-NEXT:     global %200 s200: i32 [storage=static] = const<i32>(200) [linkage=external];
// DEFAULT-NEXT:     global %201 s201: i32 [storage=static] = const<i32>(201) [linkage=external];
// DEFAULT-NEXT:     global %202 s202: i32 [storage=static] = const<i32>(202) [linkage=external];
// DEFAULT-NEXT:     global %203 s203: i32 [storage=static] = const<i32>(203) [linkage=external];
// DEFAULT-NEXT:     global %204 s204: i32 [storage=static] = const<i32>(204) [linkage=external];
// DEFAULT-NEXT:     global %205 s205: i32 [storage=static] = const<i32>(205) [linkage=external];
// DEFAULT-NEXT:     global %206 s206: i32 [storage=static] = const<i32>(206) [linkage=external];
// DEFAULT-NEXT:     global %207 s207: i32 [storage=static] = const<i32>(207) [linkage=external];
// DEFAULT-NEXT:     global %208 s208: i32 [storage=static] = const<i32>(208) [linkage=external];
// DEFAULT-NEXT:     global %209 s209: i32 [storage=static] = const<i32>(209) [linkage=external];
// DEFAULT-NEXT:     global %210 s210: i32 [storage=static] = const<i32>(210) [linkage=external];
// DEFAULT-NEXT:     global %211 s211: i32 [storage=static] = const<i32>(211) [linkage=external];
// DEFAULT-NEXT:     global %212 s212: i32 [storage=static] = const<i32>(212) [linkage=external];
// DEFAULT-NEXT:     global %213 s213: i32 [storage=static] = const<i32>(213) [linkage=external];
// DEFAULT-NEXT:     global %214 s214: i32 [storage=static] = const<i32>(214) [linkage=external];
// DEFAULT-NEXT:     global %215 s215: i32 [storage=static] = const<i32>(215) [linkage=external];
// DEFAULT-NEXT:     global %216 s216: i32 [storage=static] = const<i32>(216) [linkage=external];
// DEFAULT-NEXT:     global %217 s217: i32 [storage=static] = const<i32>(217) [linkage=external];
// DEFAULT-NEXT:     global %218 s218: i32 [storage=static] = const<i32>(218) [linkage=external];
// DEFAULT-NEXT:     global %219 s219: i32 [storage=static] = const<i32>(219) [linkage=external];
// DEFAULT-NEXT:     global %220 s220: i32 [storage=static] = const<i32>(220) [linkage=external];
// DEFAULT-NEXT:     global %221 s221: i32 [storage=static] = const<i32>(221) [linkage=external];
// DEFAULT-NEXT:     global %222 s222: i32 [storage=static] = const<i32>(222) [linkage=external];
// DEFAULT-NEXT:     global %223 s223: i32 [storage=static] = const<i32>(223) [linkage=external];
// DEFAULT-NEXT:     global %224 s224: i32 [storage=static] = const<i32>(224) [linkage=external];
// DEFAULT-NEXT:     global %225 s225: i32 [storage=static] = const<i32>(225) [linkage=external];
// DEFAULT-NEXT:     global %226 s226: i32 [storage=static] = const<i32>(226) [linkage=external];
// DEFAULT-NEXT:     global %227 s227: i32 [storage=static] = const<i32>(227) [linkage=external];
// DEFAULT-NEXT:     global %228 s228: i32 [storage=static] = const<i32>(228) [linkage=external];
// DEFAULT-NEXT:     global %229 s229: i32 [storage=static] = const<i32>(229) [linkage=external];
// DEFAULT-NEXT:     global %230 s230: i32 [storage=static] = const<i32>(230) [linkage=external];
// DEFAULT-NEXT:     global %231 s231: i32 [storage=static] = const<i32>(231) [linkage=external];
// DEFAULT-NEXT:     global %232 s232: i32 [storage=static] = const<i32>(232) [linkage=external];
// DEFAULT-NEXT:     global %233 s233: i32 [storage=static] = const<i32>(233) [linkage=external];
// DEFAULT-NEXT:     global %234 s234: i32 [storage=static] = const<i32>(234) [linkage=external];
// DEFAULT-NEXT:     global %235 s235: i32 [storage=static] = const<i32>(235) [linkage=external];
// DEFAULT-NEXT:     global %236 s236: i32 [storage=static] = const<i32>(236) [linkage=external];
// DEFAULT-NEXT:     global %237 s237: i32 [storage=static] = const<i32>(237) [linkage=external];
// DEFAULT-NEXT:     global %238 s238: i32 [storage=static] = const<i32>(238) [linkage=external];
// DEFAULT-NEXT:     global %239 s239: i32 [storage=static] = const<i32>(239) [linkage=external];
// DEFAULT-NEXT:     global %240 s240: i32 [storage=static] = const<i32>(240) [linkage=external];
// DEFAULT-NEXT:     global %241 s241: i32 [storage=static] = const<i32>(241) [linkage=external];
// DEFAULT-NEXT:     global %242 s242: i32 [storage=static] = const<i32>(242) [linkage=external];
// DEFAULT-NEXT:     global %243 s243: i32 [storage=static] = const<i32>(243) [linkage=external];
// DEFAULT-NEXT:     global %244 s244: i32 [storage=static] = const<i32>(244) [linkage=external];
// DEFAULT-NEXT:     global %245 s245: i32 [storage=static] = const<i32>(245) [linkage=external];
// DEFAULT-NEXT:     global %246 s246: i32 [storage=static] = const<i32>(246) [linkage=external];
// DEFAULT-NEXT:     global %247 s247: i32 [storage=static] = const<i32>(247) [linkage=external];
// DEFAULT-NEXT:     global %248 s248: i32 [storage=static] = const<i32>(248) [linkage=external];
// DEFAULT-NEXT:     global %249 s249: i32 [storage=static] = const<i32>(249) [linkage=external];
// DEFAULT-NEXT:     global %250 s250: i32 [storage=static] = const<i32>(250) [linkage=external];
// DEFAULT-NEXT:     global %251 s251: i32 [storage=static] = const<i32>(251) [linkage=external];
// DEFAULT-NEXT:     global %252 s252: i32 [storage=static] = const<i32>(252) [linkage=external];
// DEFAULT-NEXT:     global %253 s253: i32 [storage=static] = const<i32>(253) [linkage=external];
// DEFAULT-NEXT:     global %254 s254: i32 [storage=static] = const<i32>(254) [linkage=external];
// DEFAULT-NEXT:     global %255 s255: i32 [storage=static] = const<i32>(255) [linkage=external];
// DEFAULT-NEXT:     global %256 s256: i32 [storage=static] = const<i32>(256) [linkage=external];
// DEFAULT-NEXT:     global %257 s257: i32 [storage=static] = const<i32>(257) [linkage=external];
// DEFAULT-NEXT:     global %258 s258: i32 [storage=static] = const<i32>(258) [linkage=external];
// DEFAULT-NEXT:     global %259 s259: i32 [storage=static] = const<i32>(259) [linkage=external];
// DEFAULT-NEXT:     global %260 s260: i32 [storage=static] = const<i32>(260) [linkage=external];
// DEFAULT-NEXT:     global %261 s261: i32 [storage=static] = const<i32>(261) [linkage=external];
// DEFAULT-NEXT:     global %262 s262: i32 [storage=static] = const<i32>(262) [linkage=external];
// DEFAULT-NEXT:     global %263 s263: i32 [storage=static] = const<i32>(263) [linkage=external];
// DEFAULT-NEXT:     global %264 s264: i32 [storage=static] = const<i32>(264) [linkage=external];
// DEFAULT-NEXT:     global %265 s265: i32 [storage=static] = const<i32>(265) [linkage=external];
// DEFAULT-NEXT:     global %266 s266: i32 [storage=static] = const<i32>(266) [linkage=external];
// DEFAULT-NEXT:     global %267 s267: i32 [storage=static] = const<i32>(267) [linkage=external];
// DEFAULT-NEXT:     global %268 s268: i32 [storage=static] = const<i32>(268) [linkage=external];
// DEFAULT-NEXT:     global %269 s269: i32 [storage=static] = const<i32>(269) [linkage=external];
// DEFAULT-NEXT:     global %270 s270: i32 [storage=static] = const<i32>(270) [linkage=external];
// DEFAULT-NEXT:     global %271 s271: i32 [storage=static] = const<i32>(271) [linkage=external];
// DEFAULT-NEXT:     global %272 s272: i32 [storage=static] = const<i32>(272) [linkage=external];
// DEFAULT-NEXT:     global %273 s273: i32 [storage=static] = const<i32>(273) [linkage=external];
// DEFAULT-NEXT:     global %274 s274: i32 [storage=static] = const<i32>(274) [linkage=external];
// DEFAULT-NEXT:     global %275 s275: i32 [storage=static] = const<i32>(275) [linkage=external];
// DEFAULT-NEXT:     global %276 s276: i32 [storage=static] = const<i32>(276) [linkage=external];
// DEFAULT-NEXT:     global %277 s277: i32 [storage=static] = const<i32>(277) [linkage=external];
// DEFAULT-NEXT:     global %278 s278: i32 [storage=static] = const<i32>(278) [linkage=external];
// DEFAULT-NEXT:     global %279 s279: i32 [storage=static] = const<i32>(279) [linkage=external];
// DEFAULT-NEXT:     global %280 s280: i32 [storage=static] = const<i32>(280) [linkage=external];
// DEFAULT-NEXT:     global %281 s281: i32 [storage=static] = const<i32>(281) [linkage=external];
// DEFAULT-NEXT:     global %282 s282: i32 [storage=static] = const<i32>(282) [linkage=external];
// DEFAULT-NEXT:     global %283 s283: i32 [storage=static] = const<i32>(283) [linkage=external];
// DEFAULT-NEXT:     global %284 s284: i32 [storage=static] = const<i32>(284) [linkage=external];
// DEFAULT-NEXT:     global %285 s285: i32 [storage=static] = const<i32>(285) [linkage=external];
// DEFAULT-NEXT:     global %286 s286: i32 [storage=static] = const<i32>(286) [linkage=external];
// DEFAULT-NEXT:     global %287 s287: i32 [storage=static] = const<i32>(287) [linkage=external];
// DEFAULT-NEXT:     global %288 s288: i32 [storage=static] = const<i32>(288) [linkage=external];
// DEFAULT-NEXT:     global %289 s289: i32 [storage=static] = const<i32>(289) [linkage=external];
// DEFAULT-NEXT:     global %290 s290: i32 [storage=static] = const<i32>(290) [linkage=external];
// DEFAULT-NEXT:     global %291 s291: i32 [storage=static] = const<i32>(291) [linkage=external];
// DEFAULT-NEXT:     global %292 s292: i32 [storage=static] = const<i32>(292) [linkage=external];
// DEFAULT-NEXT:     global %293 s293: i32 [storage=static] = const<i32>(293) [linkage=external];
// DEFAULT-NEXT:     global %294 s294: i32 [storage=static] = const<i32>(294) [linkage=external];
// DEFAULT-NEXT:     global %295 s295: i32 [storage=static] = const<i32>(295) [linkage=external];
// DEFAULT-NEXT:     global %296 s296: i32 [storage=static] = const<i32>(296) [linkage=external];
// DEFAULT-NEXT:     global %297 s297: i32 [storage=static] = const<i32>(297) [linkage=external];
// DEFAULT-NEXT:     global %298 s298: i32 [storage=static] = const<i32>(298) [linkage=external];
// DEFAULT-NEXT:     global %299 s299: i32 [storage=static] = const<i32>(299) [linkage=external];
// DEFAULT-NEXT:     fn %300 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%0), read<i32>(%299));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
