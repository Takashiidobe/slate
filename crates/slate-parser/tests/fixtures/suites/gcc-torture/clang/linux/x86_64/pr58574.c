/* PR target/58574 */

__attribute__((noinline, noclone)) double foo(double x) {
  double t;
  switch ((int)x) {
  case 0:
    t = 2 * x - 1;
    return 0.70878e-3 +
           (0.71234e-3 +
            (0.35779e-5 +
             (0.17403e-7 +
              (0.81710e-10 + (0.36885e-12 + 0.15917e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 1:
    t = 2 * x - 3;
    return 0.21479e-2 +
           (0.72686e-3 +
            (0.36843e-5 +
             (0.18071e-7 +
              (0.85496e-10 + (0.38852e-12 + 0.16868e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 2:
    t = 2 * x - 5;
    return 0.36165e-2 +
           (0.74182e-3 +
            (0.37948e-5 +
             (0.18771e-7 +
              (0.89484e-10 + (0.40935e-12 + 0.17872e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 3:
    t = 2 * x - 7;
    return 0.51154e-2 +
           (0.75722e-3 +
            (0.39096e-5 +
             (0.19504e-7 +
              (0.93687e-10 + (0.43143e-12 + 0.18939e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 4:
    t = 2 * x - 9;
    return 0.66457e-2 +
           (0.77310e-3 +
            (0.40289e-5 +
             (0.20271e-7 +
              (0.98117e-10 + (0.45484e-12 + 0.20076e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 5:
    t = 2 * x - 11;
    return 0.82082e-2 +
           (0.78946e-3 +
            (0.41529e-5 +
             (0.21074e-7 +
              (0.10278e-9 + (0.47965e-12 + 0.21285e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 6:
    t = 2 * x - 13;
    return 0.98039e-2 +
           (0.80633e-3 +
            (0.42819e-5 +
             (0.21916e-7 +
              (0.10771e-9 + (0.50595e-12 + 0.22573e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 7:
    t = 2 * x - 15;
    return 0.11433e-1 +
           (0.82372e-3 +
            (0.44160e-5 +
             (0.22798e-7 +
              (0.11291e-9 + (0.53386e-12 + 0.23944e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 8:
    t = 2 * x - 17;
    return 0.13099e-1 +
           (0.84167e-3 +
            (0.45555e-5 +
             (0.23723e-7 +
              (0.11839e-9 + (0.56346e-12 + 0.25403e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 9:
    t = 2 * x - 19;
    return 0.14800e-1 +
           (0.86018e-3 +
            (0.47008e-5 +
             (0.24694e-7 +
              (0.12418e-9 + (0.59486e-12 + 0.26957e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 10:
    t = 2 * x - 21;
    return 0.16540e-1 +
           (0.87928e-3 +
            (0.48520e-5 +
             (0.25711e-7 +
              (0.13030e-9 + (0.62820e-12 + 0.28612e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 11:
    t = 2 * x - 23;
    return 0.18318e-1 +
           (0.89900e-3 +
            (0.50094e-5 +
             (0.26779e-7 +
              (0.13675e-9 + (0.66358e-12 + 0.30375e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 12:
    t = 2 * x - 25;
    return 0.20136e-1 +
           (0.91936e-3 +
            (0.51734e-5 +
             (0.27900e-7 +
              (0.14357e-9 + (0.70114e-12 + 0.32252e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 13:
    t = 2 * x - 27;
    return 0.21996e-1 +
           (0.94040e-3 +
            (0.53443e-5 +
             (0.29078e-7 +
              (0.15078e-9 + (0.74103e-12 + 0.34251e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 14:
    t = 2 * x - 29;
    return 0.23898e-1 +
           (0.96213e-3 +
            (0.55225e-5 +
             (0.30314e-7 +
              (0.15840e-9 + (0.78340e-12 + 0.36381e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 15:
    t = 2 * x - 31;
    return 0.25845e-1 +
           (0.98459e-3 +
            (0.57082e-5 +
             (0.31613e-7 +
              (0.16646e-9 + (0.82840e-12 + 0.38649e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 16:
    t = 2 * x - 33;
    return 0.27837e-1 +
           (0.10078e-2 +
            (0.59020e-5 +
             (0.32979e-7 +
              (0.17498e-9 + (0.87622e-12 + 0.41066e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 17:
    t = 2 * x - 35;
    return 0.29877e-1 +
           (0.10318e-2 +
            (0.61041e-5 +
             (0.34414e-7 +
              (0.18399e-9 + (0.92703e-12 + 0.43639e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 18:
    t = 2 * x - 37;
    return 0.31965e-1 +
           (0.10566e-2 +
            (0.63151e-5 +
             (0.35924e-7 +
              (0.19353e-9 + (0.98102e-12 + 0.46381e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 19:
    t = 2 * x - 39;
    return 0.34104e-1 +
           (0.10823e-2 +
            (0.65354e-5 +
             (0.37512e-7 +
              (0.20362e-9 + (0.10384e-11 + 0.49300e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 20:
    t = 2 * x - 41;
    return 0.36295e-1 +
           (0.11089e-2 +
            (0.67654e-5 +
             (0.39184e-7 +
              (0.21431e-9 + (0.10994e-11 + 0.52409e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 21:
    t = 2 * x - 43;
    return 0.38540e-1 +
           (0.11364e-2 +
            (0.70058e-5 +
             (0.40943e-7 +
              (0.22563e-9 + (0.11642e-11 + 0.55721e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 22:
    t = 2 * x - 45;
    return 0.40842e-1 +
           (0.11650e-2 +
            (0.72569e-5 +
             (0.42796e-7 +
              (0.23761e-9 + (0.12332e-11 + 0.59246e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 23:
    t = 2 * x - 47;
    return 0.43201e-1 +
           (0.11945e-2 +
            (0.75195e-5 +
             (0.44747e-7 +
              (0.25030e-9 + (0.13065e-11 + 0.63000e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 24:
    t = 2 * x - 49;
    return 0.45621e-1 +
           (0.12251e-2 +
            (0.77941e-5 +
             (0.46803e-7 +
              (0.26375e-9 + (0.13845e-11 + 0.66996e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 25:
    t = 2 * x - 51;
    return 0.48103e-1 +
           (0.12569e-2 +
            (0.80814e-5 +
             (0.48969e-7 +
              (0.27801e-9 + (0.14674e-11 + 0.71249e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 26:
    t = 2 * x - 59;
    return 0.58702e-1 +
           (0.13962e-2 +
            (0.93714e-5 +
             (0.58882e-7 +
              (0.34414e-9 + (0.18552e-11 + 0.91160e-14 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 30:
    t = 2 * x - 79;
    return 0.90908e-1 +
           (0.18544e-2 +
            (0.13903e-4 +
             (0.95549e-7 +
              (0.59752e-9 + (0.33656e-11 + 0.16815e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 40:
    t = 2 * x - 99;
    return 0.13443e0 +
           (0.25474e-2 +
            (0.21385e-4 +
             (0.15996e-6 +
              (0.10585e-8 + (0.61258e-11 + 0.30412e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 50:
    t = 2 * x - 119;
    return 0.19540e0 +
           (0.36342e-2 +
            (0.34096e-4 +
             (0.27479e-6 +
              (0.18934e-8 + (0.11021e-10 + 0.52931e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 60:
    t = 2 * x - 121;
    return 0.20281e0 +
           (0.37739e-2 +
            (0.35791e-4 +
             (0.29038e-6 +
              (0.20068e-8 + (0.11673e-10 + 0.55790e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 61:
    t = 2 * x - 123;
    return 0.21050e0 +
           (0.39206e-2 +
            (0.37582e-4 +
             (0.30691e-6 +
              (0.21270e-8 + (0.12361e-10 + 0.58770e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 62:
    t = 2 * x - 125;
    return 0.21849e0 +
           (0.40747e-2 +
            (0.39476e-4 +
             (0.32443e-6 +
              (0.22542e-8 + (0.13084e-10 + 0.61873e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 63:
    t = 2 * x - 127;
    return 0.22680e0 +
           (0.42366e-2 +
            (0.41477e-4 +
             (0.34300e-6 +
              (0.23888e-8 + (0.13846e-10 + 0.65100e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 64:
    t = 2 * x - 129;
    return 0.23545e0 +
           (0.44067e-2 +
            (0.43594e-4 +
             (0.36268e-6 +
              (0.25312e-8 + (0.14647e-10 + 0.68453e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 65:
    t = 2 * x - 131;
    return 0.24444e0 +
           (0.45855e-2 +
            (0.45832e-4 +
             (0.38352e-6 +
              (0.26819e-8 + (0.15489e-10 + 0.71933e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 66:
    t = 2 * x - 133;
    return 0.25379e0 +
           (0.47735e-2 +
            (0.48199e-4 +
             (0.40561e-6 +
              (0.28411e-8 + (0.16374e-10 + 0.75541e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 67:
    t = 2 * x - 135;
    return 0.26354e0 +
           (0.49713e-2 +
            (0.50702e-4 +
             (0.42901e-6 +
              (0.30095e-8 + (0.17303e-10 + 0.79278e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 68:
    t = 2 * x - 137;
    return 0.27369e0 +
           (0.51793e-2 +
            (0.53350e-4 +
             (0.45379e-6 +
              (0.31874e-8 + (0.18277e-10 + 0.83144e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 69:
    t = 2 * x - 139;
    return 0.28426e0 +
           (0.53983e-2 +
            (0.56150e-4 +
             (0.48003e-6 +
              (0.33752e-8 + (0.19299e-10 + 0.87139e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 70:
    t = 2 * x - 141;
    return 0.29529e0 +
           (0.56288e-2 +
            (0.59113e-4 +
             (0.50782e-6 +
              (0.35735e-8 + (0.20369e-10 + 0.91262e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 71:
    t = 2 * x - 143;
    return 0.30679e0 +
           (0.58714e-2 +
            (0.62248e-4 +
             (0.53724e-6 +
              (0.37827e-8 + (0.21490e-10 + 0.95513e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 72:
    t = 2 * x - 145;
    return 0.31878e0 +
           (0.61270e-2 +
            (0.65564e-4 +
             (0.56837e-6 +
              (0.40035e-8 + (0.22662e-10 + 0.99891e-13 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 73:
    t = 2 * x - 147;
    return 0.33130e0 +
           (0.63962e-2 +
            (0.69072e-4 +
             (0.60133e-6 +
              (0.42362e-8 + (0.23888e-10 + 0.10439e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 74:
    t = 2 * x - 149;
    return 0.34438e0 +
           (0.66798e-2 +
            (0.72783e-4 +
             (0.63619e-6 +
              (0.44814e-8 + (0.25168e-10 + 0.10901e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 75:
    t = 2 * x - 151;
    return 0.35803e0 +
           (0.69787e-2 +
            (0.76710e-4 +
             (0.67306e-6 +
              (0.47397e-8 + (0.26505e-10 + 0.11376e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 76:
    t = 2 * x - 153;
    return 0.37230e0 +
           (0.72938e-2 +
            (0.80864e-4 +
             (0.71206e-6 +
              (0.50117e-8 + (0.27899e-10 + 0.11862e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 77:
    t = 2 * x - 155;
    return 0.38722e0 +
           (0.76260e-2 +
            (0.85259e-4 +
             (0.75329e-6 +
              (0.52979e-8 + (0.29352e-10 + 0.12360e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 78:
    t = 2 * x - 157;
    return 0.40282e0 +
           (0.79762e-2 +
            (0.89909e-4 +
             (0.79687e-6 +
              (0.55989e-8 + (0.30866e-10 + 0.12868e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 79:
    t = 2 * x - 159;
    return 0.41914e0 +
           (0.83456e-2 +
            (0.94827e-4 +
             (0.84291e-6 +
              (0.59154e-8 + (0.32441e-10 + 0.13387e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 80:
    t = 2 * x - 161;
    return 0.43621e0 +
           (0.87352e-2 +
            (0.10002e-3 +
             (0.89156e-6 +
              (0.62480e-8 + (0.34079e-10 + 0.13917e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 81:
    t = 2 * x - 163;
    return 0.45409e0 +
           (0.91463e-2 +
            (0.10553e-3 +
             (0.94293e-6 +
              (0.65972e-8 + (0.35782e-10 + 0.14455e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 82:
    t = 2 * x - 165;
    return 0.47282e0 +
           (0.95799e-2 +
            (0.11135e-3 +
             (0.99716e-6 +
              (0.69638e-8 + (0.37549e-10 + 0.15003e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 83:
    t = 2 * x - 167;
    return 0.49243e0 +
           (0.10037e-1 +
            (0.11750e-3 +
             (0.10544e-5 +
              (0.73484e-8 + (0.39383e-10 + 0.15559e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 84:
    t = 2 * x - 169;
    return 0.51298e0 +
           (0.10520e-1 +
            (0.12400e-3 +
             (0.11147e-5 +
              (0.77517e-8 + (0.41283e-10 + 0.16122e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 85:
    t = 2 * x - 171;
    return 0.53453e0 +
           (0.11030e-1 +
            (0.13088e-3 +
             (0.11784e-5 +
              (0.81743e-8 + (0.43252e-10 + 0.16692e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 86:
    t = 2 * x - 173;
    return 0.55712e0 +
           (0.11568e-1 +
            (0.13815e-3 +
             (0.12456e-5 +
              (0.86169e-8 + (0.45290e-10 + 0.17268e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 87:
    t = 2 * x - 175;
    return 0.58082e0 +
           (0.12135e-1 +
            (0.14584e-3 +
             (0.13164e-5 +
              (0.90803e-8 + (0.47397e-10 + 0.17850e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 88:
    t = 2 * x - 177;
    return 0.60569e0 +
           (0.12735e-1 +
            (0.15396e-3 +
             (0.13909e-5 +
              (0.95651e-8 + (0.49574e-10 + 0.18435e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 89:
    t = 2 * x - 179;
    return 0.63178e0 +
           (0.13368e-1 +
            (0.16254e-3 +
             (0.14695e-5 +
              (0.10072e-7 + (0.51822e-10 + 0.19025e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 90:
    t = 2 * x - 181;
    return 0.65918e0 +
           (0.14036e-1 +
            (0.17160e-3 +
             (0.15521e-5 +
              (0.10601e-7 + (0.54140e-10 + 0.19616e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 91:
    t = 2 * x - 183;
    return 0.68795e0 +
           (0.14741e-1 +
            (0.18117e-3 +
             (0.16392e-5 +
              (0.11155e-7 + (0.56530e-10 + 0.20209e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 92:
    t = 2 * x - 185;
    return 0.71818e0 +
           (0.15486e-1 +
            (0.19128e-3 +
             (0.17307e-5 +
              (0.11732e-7 + (0.58991e-10 + 0.20803e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  case 93:
    t = 2 * x - 187;
    return 0.74993e0 +
           (0.16272e-1 +
            (0.20195e-3 +
             (0.18269e-5 +
              (0.12335e-7 + (0.61523e-10 + 0.21395e-12 * t) * t) * t) *
                 t) *
                t) *
               t;
  }
  return 1.0;
}

int main() {
#ifdef __s390x__
  {
    register unsigned long r5 __asm("r5");
    r5 = 0xdeadbeefUL;
    asm volatile("" : "+r"(r5));
  }
#endif
  double d = foo(78.4);
  if (d < 0.38 || d > 0.42)
    __builtin_abort();
  return 0;
}


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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: f64 [storage=automatic];
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_x]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(0):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00070878), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00071234), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.5779e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.7403e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.171e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.6885e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5917e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(1):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0021479), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00072686), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.6843e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.8071e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.5496e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.8852e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.6868e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(2):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(5))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0036165), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00074182), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.7948e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.8771e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.9484e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.0935e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.7872e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(3):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(7))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0051154), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00075722), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.9096e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.9504e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.3687e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.3143e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.8939e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(4):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(9))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0066457), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0007731), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.0289e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0271e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.8117e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.5484e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0076e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(5):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(11))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0082082), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00078946), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.1529e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.1074e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0278e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.7965e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.1285e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(6):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(13))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0098039), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00080633), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.2819e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.1916e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0771e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.0595e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.2573e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(7):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(15))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.011433), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00082372), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.416e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.2798e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1291e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.3386e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.3944e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(8):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(17))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.013099), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00084167), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.5555e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.3723e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1839e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.6346e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.5403e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(9):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(19))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0148), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00086018), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.7008e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.4694e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.2418e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.9486e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.6957e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(10):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(21))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.01654), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00087928), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.852e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.5711e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.303e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.282e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.8612e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(11):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(23))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.018318), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.000899), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.0094e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.6779e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3675e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.6358e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.0375e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(12):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(25))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.020136), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00091936), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.1734e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.79e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.4357e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.0114e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.2252e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(13):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(27))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.021996), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0009404), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.3443e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.9078e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5078e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.4103e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.4251e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(14):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(29))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.023898), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00096213), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.5225e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.0314e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.584e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.834e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.6381e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(15):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(31))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.025845), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00098459), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.7082e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.1613e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.6646e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.284e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.8649e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(16):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(33))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.027837), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0010078), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.902e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.2979e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.7498e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.7622e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.1066e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(17):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(35))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.029877), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0010318), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.1041e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.4414e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.8399e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.2703e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.3639e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(18):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(37))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.031965), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0010566), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.3151e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.5924e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.9353e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.8102e-13), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.6381e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(19):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(39))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.034104), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0010823), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.5354e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.7512e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0362e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0384e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.93e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(20):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(41))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.036295), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0011089), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.7654e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.9184e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.1431e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0994e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.2409e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(21):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(43))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.03854), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0011364), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.0058e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.0943e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.2563e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1642e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.5721e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(22):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(45))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.040842), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.001165), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.2569e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.2796e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.3761e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.2332e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.9246e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(23):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(47))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.043201), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0011945), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.5195e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.4747e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.503e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3065e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.3e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(24):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(49))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.045621), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0012251), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.7941e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.6803e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.6375e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3845e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.6996e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(25):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(51))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.048103), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0012569), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.0814e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.8969e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.7801e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.4674e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.1249e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(26):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(59))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.058702), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0013962), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.3714e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.8882e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.4414e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.8552e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.116e-15),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(30):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(79))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.090908), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0018544), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3903e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.5549e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.9752e-10), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.3656e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.6815e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(40):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(99))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.13443), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0025474), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.1385e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5996e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0585e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.1258e-12), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.0412e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(50):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(119))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.1954), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0036342), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.4096e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.7479e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.8934e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1021e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.2931e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(60):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(121))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.20281), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0037739), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.5791e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.9038e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0068e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1673e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.579e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(61):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(123))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.2105), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0039206), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.7582e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.0691e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.127e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.2361e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.877e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(62):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(125))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.21849), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0040747), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.9476e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.2443e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.2542e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3084e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.1873e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(63):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(127))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.2268), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0042366), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.1477e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.43e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.3888e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3846e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.51e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(64):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(129))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.23545), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0044067), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.3594e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.6268e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.5312e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.4647e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.8453e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(65):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(131))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.24444), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0045855), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.5832e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.8352e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.6819e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5489e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.1933e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(66):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(133))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.25379), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0047735), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.8199e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.0561e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.8411e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.6374e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.5541e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(67):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(135))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.26354), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0049713), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.0702e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.2901e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.0095e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.7303e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.9278e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(68):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(137))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.27369), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0051793), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.335e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.5379e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.1874e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.8277e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.3144e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(69):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(139))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.28426), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0053983), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.615e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.8003e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.3752e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.9299e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.7139e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(70):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(141))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.29529), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0056288), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.9113e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.0782e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.5735e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0369e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.1262e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(71):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(143))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.30679), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0058714), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.2248e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.3724e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.7827e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.149e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.5513e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(72):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(145))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.31878), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.006127), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.5564e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.6837e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.0035e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.2662e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.9891e-14),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(73):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(147))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.3313), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0063962), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.9072e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.0133e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.2362e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.3888e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0439e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(74):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(149))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.34438), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0066798), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.2783e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.3619e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.4814e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.5168e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0901e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(75):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(151))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.35803), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0069787), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.671e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.7306e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.7397e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.6505e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1376e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(76):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(153))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.3723), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0072938), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.0864e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.1206e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.0117e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.7899e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1862e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(77):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(155))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.38722), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.007626), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.5259e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.5329e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.2979e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.9352e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.236e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(78):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(157))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.40282), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0079762), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.9909e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.9687e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.5989e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.0866e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.2868e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(79):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(159))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.41914), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0083456), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.4827e-5), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.4291e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.9154e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.2441e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3387e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(80):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(161))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.43621), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0087352), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00010002), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.9156e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.248e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.4079e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3917e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(81):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(163))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.45409), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0091463), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00010553), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.4293e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.5972e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.5782e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.4455e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(82):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(165))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.47282), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0095799), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00011135), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.9716e-7), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.9638e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.7549e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5003e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(83):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(167))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.49243), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.010037), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0001175), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0544e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.3484e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(3.9383e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5559e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(84):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(169))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.51298), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.01052), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.000124), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1147e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(7.7517e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.1283e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.6122e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(85):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(171))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.53453), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.01103), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00013088), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1784e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.1743e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.3252e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.6692e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(86):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(173))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.55712), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.011568), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00013815), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.2456e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(8.6169e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.529e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.7268e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(87):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(175))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.58082), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.012135), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00014584), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3164e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.0803e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.7397e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.785e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(88):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(177))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.60569), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.012735), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00015396), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.3909e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(9.5651e-9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(4.9574e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.8435e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(89):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(179))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.63178), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.013368), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00016254), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.4695e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0072e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.1822e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.9025e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(90):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(181))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.65918), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.014036), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.0001716), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.5521e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0601e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.414e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.9616e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(91):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(183))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.68795), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.014741), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00018117), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.6392e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1155e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.653e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0209e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(92):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(185))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.71818), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.015486), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00019128), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.7307e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.1732e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.8991e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0803e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(93):
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_t]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f64>(%[[VALUE_x]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(187))));
// DEFAULT-NEXT:                 return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.74993), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.016272), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.00020195), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.8269e-6), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.2335e-8), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(6.1523e-11), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.1395e-13),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]]))),
// DEFAULT-SAME: read<f64>(%[[VALUE_t]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<f64>(1.0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%[[VALUE_foo]], const<f64>(78.4));
// DEFAULT-NEXT:         if logical_or<bool>(lt<f64, exceptions=ignore>(read<f64>(%[[VALUE_d]]), const<f64>(0.38)), gt<f64, exceptions=ignore>(read<f64>(%[[VALUE_d]]), const<f64>(0.42)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
