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
// DEFAULT: decl[0]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Floating(
// DEFAULT-NEXT:                   Double,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               attributes: [
// DEFAULT-NEXT:                   NoInline,
// DEFAULT-NEXT:                   NoClone,
// DEFAULT-NEXT:               ],
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "foo",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclaration {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Floating(
// DEFAULT-NEXT:                                   Double,
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Name(
// DEFAULT-NEXT:                               "x",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "t",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Switch {
// DEFAULT-NEXT:                   discriminant: Cast {
// DEFAULT-NEXT:                       ty: TypeName {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Integer(
// DEFAULT-NEXT:                                   Ranked {
// DEFAULT-NEXT:                                       rank: Int,
// DEFAULT-NEXT:                                       signed: true,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Abstract,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       value: Identifier(
// DEFAULT-NEXT:                           "x",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   body: [
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 0,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 1,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "1",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.70878e-3",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.71234e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.35779e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.17403e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.81710e-10",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.36885e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.15917e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 1,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "1",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 3,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "3",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.21479e-2",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.72686e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.36843e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.18071e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.85496e-10",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.38852e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.16868e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 2,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "2",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 5,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "5",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.36165e-2",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.74182e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.37948e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.18771e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.89484e-10",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.40935e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.17872e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 3,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "3",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 7,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "7",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.51154e-2",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.75722e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.39096e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.19504e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.93687e-10",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.43143e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.18939e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 4,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "4",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 9,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "9",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.66457e-2",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.77310e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.40289e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.20271e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.98117e-10",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.45484e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.20076e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 5,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "5",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 11,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "11",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.82082e-2",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.78946e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.41529e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.21074e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.10278e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.47965e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.21285e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 6,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "6",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 13,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "13",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.98039e-2",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.80633e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.42819e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.21916e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.10771e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.50595e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.22573e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 7,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "7",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 15,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "15",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.11433e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.82372e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.44160e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.22798e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.11291e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.53386e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.23944e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 8,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "8",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 17,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "17",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.13099e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.84167e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.45555e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.23723e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.11839e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.56346e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.25403e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 9,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "9",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 19,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "19",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.14800e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.86018e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.47008e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.24694e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.12418e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.59486e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.26957e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 10,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "10",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 21,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "21",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.16540e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.87928e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.48520e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.25711e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.13030e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.62820e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.28612e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 11,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "11",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 23,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "23",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.18318e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.89900e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.50094e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.26779e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.13675e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.66358e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.30375e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 12,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "12",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 25,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "25",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.20136e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.91936e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.51734e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.27900e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.14357e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.70114e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.32252e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 13,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "13",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 27,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "27",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.21996e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.94040e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.53443e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.29078e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.15078e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.74103e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.34251e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 14,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "14",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 29,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "29",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.23898e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.96213e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.55225e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.30314e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.15840e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.78340e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.36381e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 15,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "15",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 31,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "31",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.25845e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.98459e-3",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.57082e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.31613e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.16646e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.82840e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.38649e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 16,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "16",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 33,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "33",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.27837e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.10078e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.59020e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.32979e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.17498e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.87622e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.41066e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 17,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "17",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 35,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "35",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.29877e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.10318e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.61041e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.34414e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.18399e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.92703e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.43639e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 18,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "18",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 37,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "37",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.31965e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.10566e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.63151e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.35924e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.19353e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.98102e-12",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.46381e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 19,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "19",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 39,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "39",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.34104e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.10823e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.65354e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.37512e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.20362e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.10384e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.49300e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 20,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "20",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 41,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "41",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.36295e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.11089e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.67654e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.39184e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.21431e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.10994e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.52409e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 21,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "21",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 43,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "43",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.38540e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.11364e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.70058e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.40943e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.22563e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.11642e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.55721e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 22,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "22",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 45,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "45",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.40842e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.11650e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.72569e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.42796e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.23761e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.12332e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.59246e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 23,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "23",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 47,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "47",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.43201e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.11945e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.75195e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.44747e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.25030e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.13065e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.63000e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 24,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "24",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 49,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "49",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.45621e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.12251e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.77941e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.46803e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.26375e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.13845e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.66996e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 25,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "25",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 51,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "51",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.48103e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.12569e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.80814e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.48969e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.27801e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.14674e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.71249e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 26,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "26",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 59,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "59",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.58702e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.13962e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.93714e-5",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.58882e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.34414e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.18552e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.91160e-14",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 30,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "30",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 79,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "79",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.90908e-1",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.18544e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.13903e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.95549e-7",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.59752e-9",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.33656e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.16815e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 40,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "40",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 99,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "99",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.13443e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.25474e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.21385e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.15996e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.10585e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.61258e-11",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.30412e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 50,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "50",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 119,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "119",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.19540e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.36342e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.34096e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.27479e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.18934e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.11021e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.52931e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 60,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "60",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 121,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "121",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.20281e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.37739e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.35791e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.29038e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.20068e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.11673e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.55790e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 61,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "61",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 123,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "123",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.21050e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.39206e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.37582e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.30691e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.21270e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.12361e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.58770e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 62,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "62",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 125,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "125",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.21849e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.40747e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.39476e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.32443e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.22542e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.13084e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.61873e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 63,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "63",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 127,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "127",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.22680e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.42366e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.41477e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.34300e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.23888e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.13846e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.65100e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 64,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "64",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 129,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "129",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.23545e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.44067e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.43594e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.36268e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.25312e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.14647e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.68453e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 65,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "65",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 131,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "131",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.24444e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.45855e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.45832e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.38352e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.26819e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.15489e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.71933e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 66,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "66",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 133,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "133",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.25379e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.47735e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.48199e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.40561e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.28411e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.16374e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.75541e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 67,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "67",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 135,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "135",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.26354e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.49713e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.50702e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.42901e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.30095e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.17303e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.79278e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 68,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "68",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 137,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "137",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.27369e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.51793e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.53350e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.45379e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.31874e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.18277e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.83144e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 69,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "69",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 139,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "139",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.28426e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.53983e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.56150e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.48003e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.33752e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.19299e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.87139e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 70,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "70",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 141,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "141",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.29529e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.56288e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.59113e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.50782e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.35735e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.20369e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.91262e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 71,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "71",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 143,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "143",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.30679e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.58714e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.62248e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.53724e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.37827e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.21490e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.95513e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 72,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "72",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 145,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "145",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.31878e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.61270e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.65564e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.56837e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.40035e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.22662e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.99891e-13",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 73,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "73",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 147,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "147",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.33130e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.63962e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.69072e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.60133e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.42362e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.23888e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.10439e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 74,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "74",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 149,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "149",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.34438e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.66798e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.72783e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.63619e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.44814e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.25168e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.10901e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 75,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "75",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 151,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "151",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.35803e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.69787e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.76710e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.67306e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.47397e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.26505e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.11376e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 76,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "76",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 153,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "153",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.37230e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.72938e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.80864e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.71206e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.50117e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.27899e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.11862e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 77,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "77",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 155,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "155",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.38722e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.76260e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.85259e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.75329e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.52979e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.29352e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.12360e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 78,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "78",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 157,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "157",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.40282e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.79762e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.89909e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.79687e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.55989e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.30866e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.12868e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 79,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "79",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 159,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "159",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.41914e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.83456e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.94827e-4",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.84291e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.59154e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.32441e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.13387e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 80,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "80",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 161,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "161",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.43621e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.87352e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.10002e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.89156e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.62480e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.34079e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.13917e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 81,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "81",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 163,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "163",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.45409e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.91463e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.10553e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.94293e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.65972e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.35782e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.14455e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 82,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "82",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 165,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "165",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.47282e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.95799e-2",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.11135e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.99716e-6",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.69638e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.37549e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.15003e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 83,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "83",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 167,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "167",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.49243e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.10037e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.11750e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.10544e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.73484e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.39383e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.15559e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 84,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "84",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 169,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "169",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.51298e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.10520e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.12400e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.11147e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.77517e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.41283e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.16122e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 85,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "85",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 171,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "171",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.53453e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.11030e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.13088e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.11784e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.81743e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.43252e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.16692e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 86,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "86",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 173,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "173",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.55712e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.11568e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.13815e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.12456e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.86169e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.45290e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.17268e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 87,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "87",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 175,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "175",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.58082e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.12135e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.14584e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.13164e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.90803e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.47397e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.17850e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 88,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "88",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 177,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "177",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.60569e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.12735e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.15396e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.13909e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.95651e-8",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.49574e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.18435e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 89,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "89",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 179,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "179",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.63178e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.13368e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.16254e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.14695e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.10072e-7",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.51822e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.19025e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 90,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "90",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 181,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "181",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.65918e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.14036e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.17160e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.15521e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.10601e-7",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.54140e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.19616e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 91,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "91",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 183,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "183",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.68795e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.14741e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.18117e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.16392e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.11155e-7",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.56530e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.20209e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 92,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "92",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 185,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "185",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.71818e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.15486e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.19128e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.17307e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.11732e-7",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.58991e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.20803e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       SwitchLabel {
// DEFAULT-NEXT:                           label: Case(
// DEFAULT-NEXT:                               IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 93,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "93",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           body: Expr(
// DEFAULT-NEXT:                               Assign {
// DEFAULT-NEXT:                                   op: Assign,
// DEFAULT-NEXT:                                   target: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   value: Binary {
// DEFAULT-NEXT:                                       op: Sub,
// DEFAULT-NEXT:                                       left: Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Identifier(
// DEFAULT-NEXT:                                               "x",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 187,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "187",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       Return(
// DEFAULT-NEXT:                           Binary {
// DEFAULT-NEXT:                               op: Add,
// DEFAULT-NEXT:                               left: FloatLiteral(
// DEFAULT-NEXT:                                   FloatLiteral {
// DEFAULT-NEXT:                                       spelling: "0.74993e0",
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               right: Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: FloatLiteral(
// DEFAULT-NEXT:                                               FloatLiteral {
// DEFAULT-NEXT:                                                   spelling: "0.16272e-1",
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: Binary {
// DEFAULT-NEXT:                                               op: Mul,
// DEFAULT-NEXT:                                               left: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Add,
// DEFAULT-NEXT:                                                       left: FloatLiteral(
// DEFAULT-NEXT:                                                           FloatLiteral {
// DEFAULT-NEXT:                                                               spelling: "0.20195e-3",
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: None,
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Mul,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Binary {
// DEFAULT-NEXT:                                                                   op: Add,
// DEFAULT-NEXT:                                                                   left: FloatLiteral(
// DEFAULT-NEXT:                                                                       FloatLiteral {
// DEFAULT-NEXT:                                                                           spelling: "0.18269e-5",
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   right: Binary {
// DEFAULT-NEXT:                                                                       op: Mul,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Binary {
// DEFAULT-NEXT:                                                                               op: Add,
// DEFAULT-NEXT:                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                       spelling: "0.12335e-7",
// DEFAULT-NEXT:                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               right: Binary {
// DEFAULT-NEXT:                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                           left: FloatLiteral(
// DEFAULT-NEXT:                                                                                               FloatLiteral {
// DEFAULT-NEXT:                                                                                                   spelling: "0.61523e-10",
// DEFAULT-NEXT:                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                   suffix: None,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                               left: FloatLiteral(
// DEFAULT-NEXT:                                                                                                   FloatLiteral {
// DEFAULT-NEXT:                                                                                                       spelling: "0.21395e-12",
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: None,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                   "t",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                       "t",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "t",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Identifier(
// DEFAULT-NEXT:                                                               "t",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Identifier(
// DEFAULT-NEXT:                                                   "t",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Identifier(
// DEFAULT-NEXT:                                       "t",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   FloatLiteral(
// DEFAULT-NEXT:                       FloatLiteral {
// DEFAULT-NEXT:                           spelling: "1.0",
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: None,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 2,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[1]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "main",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Empty,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Floating(
// DEFAULT-NEXT:                               Double,
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclarator {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "d",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Call {
// DEFAULT-NEXT:                                           callee: Identifier(
// DEFAULT-NEXT:                                               "foo",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           arguments: [
// DEFAULT-NEXT:                                               FloatLiteral(
// DEFAULT-NEXT:                                                   FloatLiteral {
// DEFAULT-NEXT:                                                       spelling: "78.4",
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               If {
// DEFAULT-NEXT:                   condition: Binary {
// DEFAULT-NEXT:                       op: Or,
// DEFAULT-NEXT:                       left: Binary {
// DEFAULT-NEXT:                           op: Less,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "d",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: FloatLiteral(
// DEFAULT-NEXT:                               FloatLiteral {
// DEFAULT-NEXT:                                   spelling: "0.38",
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       right: Binary {
// DEFAULT-NEXT:                           op: Greater,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "d",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: FloatLiteral(
// DEFAULT-NEXT:                               FloatLiteral {
// DEFAULT-NEXT:                                   spelling: "0.42",
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:                   then_branch: [
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Call {
// DEFAULT-NEXT:                               callee: Identifier(
// DEFAULT-NEXT:                                   "__builtin_abort",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               arguments: [],
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:                   else_branch: None,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               Return(
// DEFAULT-NEXT:                   IntegerLiteral(
// DEFAULT-NEXT:                       IntegerLiteral {
// DEFAULT-NEXT:                           value: 0,
// DEFAULT-NEXT:                           radix: Decimal,
// DEFAULT-NEXT:                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                               unsigned: false,
// DEFAULT-NEXT:                               size: None,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           spelling: "0",
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:           provenance: Provenance {
// DEFAULT-NEXT:               file: FileId(
// DEFAULT-NEXT:                   3,
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               kind: User,
// DEFAULT-NEXT:               line: 649,
// DEFAULT-NEXT:               header: None,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
