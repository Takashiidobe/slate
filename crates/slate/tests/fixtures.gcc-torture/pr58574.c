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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/58574
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: f64) -> f64 {
// CHECK-NEXT:     let mut t: f64 = 0 as f64;
// CHECK-NEXT:     match x as i32 {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((1 as i32) as f64);
// CHECK-NEXT:             return 0.00070878f64
// CHECK-NEXT:                 + (0.00071234f64
// CHECK-NEXT:                     + (3.5779e-6f64
// CHECK-NEXT:                         + (1.7403e-8f64
// CHECK-NEXT:                             + (8.171e-11f64 + (3.6885e-13f64 + 1.5917e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((3 as i32) as f64);
// CHECK-NEXT:             return 0.0021479f64
// CHECK-NEXT:                 + (0.00072686f64
// CHECK-NEXT:                     + (3.6843e-6f64
// CHECK-NEXT:                         + (1.8071e-8f64
// CHECK-NEXT:                             + (8.5496e-11f64 + (3.8852e-13f64 + 1.6868e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((5 as i32) as f64);
// CHECK-NEXT:             return 0.0036165f64
// CHECK-NEXT:                 + (0.00074182f64
// CHECK-NEXT:                     + (3.7948e-6f64
// CHECK-NEXT:                         + (1.8771e-8f64
// CHECK-NEXT:                             + (8.9484e-11f64 + (4.0935e-13f64 + 1.7872e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         3 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((7 as i32) as f64);
// CHECK-NEXT:             return 0.0051154f64
// CHECK-NEXT:                 + (0.00075722f64
// CHECK-NEXT:                     + (3.9096e-6f64
// CHECK-NEXT:                         + (1.9504e-8f64
// CHECK-NEXT:                             + (9.3687e-11f64 + (4.3143e-13f64 + 1.8939e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((9 as i32) as f64);
// CHECK-NEXT:             return 0.0066457f64
// CHECK-NEXT:                 + (0.0007731f64
// CHECK-NEXT:                     + (4.0289e-6f64
// CHECK-NEXT:                         + (2.0271e-8f64
// CHECK-NEXT:                             + (9.8117e-11f64 + (4.5484e-13f64 + 2.0076e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         5 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((11 as i32) as f64);
// CHECK-NEXT:             return 0.0082082f64
// CHECK-NEXT:                 + (0.00078946f64
// CHECK-NEXT:                     + (4.1529e-6f64
// CHECK-NEXT:                         + (2.1074e-8f64
// CHECK-NEXT:                             + (1.0278e-10f64 + (4.7965e-13f64 + 2.1285e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         6 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((13 as i32) as f64);
// CHECK-NEXT:             return 0.0098039f64
// CHECK-NEXT:                 + (0.00080633f64
// CHECK-NEXT:                     + (4.2819e-6f64
// CHECK-NEXT:                         + (2.1916e-8f64
// CHECK-NEXT:                             + (1.0771e-10f64 + (5.0595e-13f64 + 2.2573e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         7 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((15 as i32) as f64);
// CHECK-NEXT:             return 0.011433f64
// CHECK-NEXT:                 + (0.00082372f64
// CHECK-NEXT:                     + (4.416e-6f64
// CHECK-NEXT:                         + (2.2798e-8f64
// CHECK-NEXT:                             + (1.1291e-10f64 + (5.3386e-13f64 + 2.3944e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         8 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((17 as i32) as f64);
// CHECK-NEXT:             return 0.013099f64
// CHECK-NEXT:                 + (0.00084167f64
// CHECK-NEXT:                     + (4.5555e-6f64
// CHECK-NEXT:                         + (2.3723e-8f64
// CHECK-NEXT:                             + (1.1839e-10f64 + (5.6346e-13f64 + 2.5403e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         9 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((19 as i32) as f64);
// CHECK-NEXT:             return 0.0148f64
// CHECK-NEXT:                 + (0.00086018f64
// CHECK-NEXT:                     + (4.7008e-6f64
// CHECK-NEXT:                         + (2.4694e-8f64
// CHECK-NEXT:                             + (1.2418e-10f64 + (5.9486e-13f64 + 2.6957e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         10 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((21 as i32) as f64);
// CHECK-NEXT:             return 0.01654f64
// CHECK-NEXT:                 + (0.00087928f64
// CHECK-NEXT:                     + (4.852e-6f64
// CHECK-NEXT:                         + (2.5711e-8f64
// CHECK-NEXT:                             + (1.303e-10f64 + (6.282e-13f64 + 2.8612e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         11 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((23 as i32) as f64);
// CHECK-NEXT:             return 0.018318f64
// CHECK-NEXT:                 + (0.000899f64
// CHECK-NEXT:                     + (5.0094e-6f64
// CHECK-NEXT:                         + (2.6779e-8f64
// CHECK-NEXT:                             + (1.3675e-10f64 + (6.6358e-13f64 + 3.0375e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         12 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((25 as i32) as f64);
// CHECK-NEXT:             return 0.020136f64
// CHECK-NEXT:                 + (0.00091936f64
// CHECK-NEXT:                     + (5.1734e-6f64
// CHECK-NEXT:                         + (2.79e-8f64
// CHECK-NEXT:                             + (1.4357e-10f64 + (7.0114e-13f64 + 3.2252e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         13 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((27 as i32) as f64);
// CHECK-NEXT:             return 0.021996f64
// CHECK-NEXT:                 + (0.0009404f64
// CHECK-NEXT:                     + (5.3443e-6f64
// CHECK-NEXT:                         + (2.9078e-8f64
// CHECK-NEXT:                             + (1.5078e-10f64 + (7.4103e-13f64 + 3.4251e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         14 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((29 as i32) as f64);
// CHECK-NEXT:             return 0.023898f64
// CHECK-NEXT:                 + (0.00096213f64
// CHECK-NEXT:                     + (5.5225e-6f64
// CHECK-NEXT:                         + (3.0314e-8f64
// CHECK-NEXT:                             + (1.584e-10f64 + (7.834e-13f64 + 3.6381e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         15 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((31 as i32) as f64);
// CHECK-NEXT:             return 0.025845f64
// CHECK-NEXT:                 + (0.00098459f64
// CHECK-NEXT:                     + (5.7082e-6f64
// CHECK-NEXT:                         + (3.1613e-8f64
// CHECK-NEXT:                             + (1.6646e-10f64 + (8.284e-13f64 + 3.8649e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         16 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((33 as i32) as f64);
// CHECK-NEXT:             return 0.027837f64
// CHECK-NEXT:                 + (0.0010078f64
// CHECK-NEXT:                     + (5.902e-6f64
// CHECK-NEXT:                         + (3.2979e-8f64
// CHECK-NEXT:                             + (1.7498e-10f64 + (8.7622e-13f64 + 4.1066e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         17 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((35 as i32) as f64);
// CHECK-NEXT:             return 0.029877f64
// CHECK-NEXT:                 + (0.0010318f64
// CHECK-NEXT:                     + (6.1041e-6f64
// CHECK-NEXT:                         + (3.4414e-8f64
// CHECK-NEXT:                             + (1.8399e-10f64 + (9.2703e-13f64 + 4.3639e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         18 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((37 as i32) as f64);
// CHECK-NEXT:             return 0.031965f64
// CHECK-NEXT:                 + (0.0010566f64
// CHECK-NEXT:                     + (6.3151e-6f64
// CHECK-NEXT:                         + (3.5924e-8f64
// CHECK-NEXT:                             + (1.9353e-10f64 + (9.8102e-13f64 + 4.6381e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         19 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((39 as i32) as f64);
// CHECK-NEXT:             return 0.034104f64
// CHECK-NEXT:                 + (0.0010823f64
// CHECK-NEXT:                     + (6.5354e-6f64
// CHECK-NEXT:                         + (3.7512e-8f64
// CHECK-NEXT:                             + (2.0362e-10f64 + (1.0384e-12f64 + 4.93e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         20 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((41 as i32) as f64);
// CHECK-NEXT:             return 0.036295f64
// CHECK-NEXT:                 + (0.0011089f64
// CHECK-NEXT:                     + (6.7654e-6f64
// CHECK-NEXT:                         + (3.9184e-8f64
// CHECK-NEXT:                             + (2.1431e-10f64 + (1.0994e-12f64 + 5.2409e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         21 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((43 as i32) as f64);
// CHECK-NEXT:             return 0.03854f64
// CHECK-NEXT:                 + (0.0011364f64
// CHECK-NEXT:                     + (7.0058e-6f64
// CHECK-NEXT:                         + (4.0943e-8f64
// CHECK-NEXT:                             + (2.2563e-10f64 + (1.1642e-12f64 + 5.5721e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         22 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((45 as i32) as f64);
// CHECK-NEXT:             return 0.040842f64
// CHECK-NEXT:                 + (0.001165f64
// CHECK-NEXT:                     + (7.2569e-6f64
// CHECK-NEXT:                         + (4.2796e-8f64
// CHECK-NEXT:                             + (2.3761e-10f64 + (1.2332e-12f64 + 5.9246e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         23 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((47 as i32) as f64);
// CHECK-NEXT:             return 0.043201f64
// CHECK-NEXT:                 + (0.0011945f64
// CHECK-NEXT:                     + (7.5195e-6f64
// CHECK-NEXT:                         + (4.4747e-8f64
// CHECK-NEXT:                             + (2.503e-10f64 + (1.3065e-12f64 + 6.3e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         24 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((49 as i32) as f64);
// CHECK-NEXT:             return 0.045621f64
// CHECK-NEXT:                 + (0.0012251f64
// CHECK-NEXT:                     + (7.7941e-6f64
// CHECK-NEXT:                         + (4.6803e-8f64
// CHECK-NEXT:                             + (2.6375e-10f64 + (1.3845e-12f64 + 6.6996e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         25 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((51 as i32) as f64);
// CHECK-NEXT:             return 0.048103f64
// CHECK-NEXT:                 + (0.0012569f64
// CHECK-NEXT:                     + (8.0814e-6f64
// CHECK-NEXT:                         + (4.8969e-8f64
// CHECK-NEXT:                             + (2.7801e-10f64 + (1.4674e-12f64 + 7.1249e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         26 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((59 as i32) as f64);
// CHECK-NEXT:             return 0.058702f64
// CHECK-NEXT:                 + (0.0013962f64
// CHECK-NEXT:                     + (9.3714e-6f64
// CHECK-NEXT:                         + (5.8882e-8f64
// CHECK-NEXT:                             + (3.4414e-10f64 + (1.8552e-12f64 + 9.116e-15f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         30 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((79 as i32) as f64);
// CHECK-NEXT:             return 0.090908f64
// CHECK-NEXT:                 + (0.0018544f64
// CHECK-NEXT:                     + (1.3903e-5f64
// CHECK-NEXT:                         + (9.5549e-8f64
// CHECK-NEXT:                             + (5.9752e-10f64 + (3.3656e-12f64 + 1.6815e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         40 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((99 as i32) as f64);
// CHECK-NEXT:             return 0.13443f64
// CHECK-NEXT:                 + (0.0025474f64
// CHECK-NEXT:                     + (2.1385e-5f64
// CHECK-NEXT:                         + (1.5996e-7f64
// CHECK-NEXT:                             + (1.0585e-9f64 + (6.1258e-12f64 + 3.0412e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         50 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((119 as i32) as f64);
// CHECK-NEXT:             return 0.1954f64
// CHECK-NEXT:                 + (0.0036342f64
// CHECK-NEXT:                     + (3.4096e-5f64
// CHECK-NEXT:                         + (2.7479e-7f64
// CHECK-NEXT:                             + (1.8934e-9f64 + (1.1021e-11f64 + 5.2931e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         60 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((121 as i32) as f64);
// CHECK-NEXT:             return 0.20281f64
// CHECK-NEXT:                 + (0.0037739f64
// CHECK-NEXT:                     + (3.5791e-5f64
// CHECK-NEXT:                         + (2.9038e-7f64
// CHECK-NEXT:                             + (2.0068e-9f64 + (1.1673e-11f64 + 5.579e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         61 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((123 as i32) as f64);
// CHECK-NEXT:             return 0.2105f64
// CHECK-NEXT:                 + (0.0039206f64
// CHECK-NEXT:                     + (3.7582e-5f64
// CHECK-NEXT:                         + (3.0691e-7f64
// CHECK-NEXT:                             + (2.127e-9f64 + (1.2361e-11f64 + 5.877e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         62 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((125 as i32) as f64);
// CHECK-NEXT:             return 0.21849f64
// CHECK-NEXT:                 + (0.0040747f64
// CHECK-NEXT:                     + (3.9476e-5f64
// CHECK-NEXT:                         + (3.2443e-7f64
// CHECK-NEXT:                             + (2.2542e-9f64 + (1.3084e-11f64 + 6.1873e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         63 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((127 as i32) as f64);
// CHECK-NEXT:             return 0.2268f64
// CHECK-NEXT:                 + (0.0042366f64
// CHECK-NEXT:                     + (4.1477e-5f64
// CHECK-NEXT:                         + (3.43e-7f64
// CHECK-NEXT:                             + (2.3888e-9f64 + (1.3846e-11f64 + 6.51e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         64 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((129 as i32) as f64);
// CHECK-NEXT:             return 0.23545f64
// CHECK-NEXT:                 + (0.0044067f64
// CHECK-NEXT:                     + (4.3594e-5f64
// CHECK-NEXT:                         + (3.6268e-7f64
// CHECK-NEXT:                             + (2.5312e-9f64 + (1.4647e-11f64 + 6.8453e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         65 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((131 as i32) as f64);
// CHECK-NEXT:             return 0.24444f64
// CHECK-NEXT:                 + (0.0045855f64
// CHECK-NEXT:                     + (4.5832e-5f64
// CHECK-NEXT:                         + (3.8352e-7f64
// CHECK-NEXT:                             + (2.6819e-9f64 + (1.5489e-11f64 + 7.1933e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         66 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((133 as i32) as f64);
// CHECK-NEXT:             return 0.25379f64
// CHECK-NEXT:                 + (0.0047735f64
// CHECK-NEXT:                     + (4.8199e-5f64
// CHECK-NEXT:                         + (4.0561e-7f64
// CHECK-NEXT:                             + (2.8411e-9f64 + (1.6374e-11f64 + 7.5541e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         67 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((135 as i32) as f64);
// CHECK-NEXT:             return 0.26354f64
// CHECK-NEXT:                 + (0.0049713f64
// CHECK-NEXT:                     + (5.0702e-5f64
// CHECK-NEXT:                         + (4.2901e-7f64
// CHECK-NEXT:                             + (3.0095e-9f64 + (1.7303e-11f64 + 7.9278e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         68 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((137 as i32) as f64);
// CHECK-NEXT:             return 0.27369f64
// CHECK-NEXT:                 + (0.0051793f64
// CHECK-NEXT:                     + (5.335e-5f64
// CHECK-NEXT:                         + (4.5379e-7f64
// CHECK-NEXT:                             + (3.1874e-9f64 + (1.8277e-11f64 + 8.3144e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         69 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((139 as i32) as f64);
// CHECK-NEXT:             return 0.28426f64
// CHECK-NEXT:                 + (0.0053983f64
// CHECK-NEXT:                     + (5.615e-5f64
// CHECK-NEXT:                         + (4.8003e-7f64
// CHECK-NEXT:                             + (3.3752e-9f64 + (1.9299e-11f64 + 8.7139e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         70 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((141 as i32) as f64);
// CHECK-NEXT:             return 0.29529f64
// CHECK-NEXT:                 + (0.0056288f64
// CHECK-NEXT:                     + (5.9113e-5f64
// CHECK-NEXT:                         + (5.0782e-7f64
// CHECK-NEXT:                             + (3.5735e-9f64 + (2.0369e-11f64 + 9.1262e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         71 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((143 as i32) as f64);
// CHECK-NEXT:             return 0.30679f64
// CHECK-NEXT:                 + (0.0058714f64
// CHECK-NEXT:                     + (6.2248e-5f64
// CHECK-NEXT:                         + (5.3724e-7f64
// CHECK-NEXT:                             + (3.7827e-9f64 + (2.149e-11f64 + 9.5513e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         72 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((145 as i32) as f64);
// CHECK-NEXT:             return 0.31878f64
// CHECK-NEXT:                 + (0.006127f64
// CHECK-NEXT:                     + (6.5564e-5f64
// CHECK-NEXT:                         + (5.6837e-7f64
// CHECK-NEXT:                             + (4.0035e-9f64 + (2.2662e-11f64 + 9.9891e-14f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         73 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((147 as i32) as f64);
// CHECK-NEXT:             return 0.3313f64
// CHECK-NEXT:                 + (0.0063962f64
// CHECK-NEXT:                     + (6.9072e-5f64
// CHECK-NEXT:                         + (6.0133e-7f64
// CHECK-NEXT:                             + (4.2362e-9f64 + (2.3888e-11f64 + 1.0439e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         74 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((149 as i32) as f64);
// CHECK-NEXT:             return 0.34438f64
// CHECK-NEXT:                 + (0.0066798f64
// CHECK-NEXT:                     + (7.2783e-5f64
// CHECK-NEXT:                         + (6.3619e-7f64
// CHECK-NEXT:                             + (4.4814e-9f64 + (2.5168e-11f64 + 1.0901e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         75 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((151 as i32) as f64);
// CHECK-NEXT:             return 0.35803f64
// CHECK-NEXT:                 + (0.0069787f64
// CHECK-NEXT:                     + (7.671e-5f64
// CHECK-NEXT:                         + (6.7306e-7f64
// CHECK-NEXT:                             + (4.7397e-9f64 + (2.6505e-11f64 + 1.1376e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         76 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((153 as i32) as f64);
// CHECK-NEXT:             return 0.3723f64
// CHECK-NEXT:                 + (0.0072938f64
// CHECK-NEXT:                     + (8.0864e-5f64
// CHECK-NEXT:                         + (7.1206e-7f64
// CHECK-NEXT:                             + (5.0117e-9f64 + (2.7899e-11f64 + 1.1862e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         77 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((155 as i32) as f64);
// CHECK-NEXT:             return 0.38722f64
// CHECK-NEXT:                 + (0.007626f64
// CHECK-NEXT:                     + (8.5259e-5f64
// CHECK-NEXT:                         + (7.5329e-7f64
// CHECK-NEXT:                             + (5.2979e-9f64 + (2.9352e-11f64 + 1.236e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         78 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((157 as i32) as f64);
// CHECK-NEXT:             return 0.40282f64
// CHECK-NEXT:                 + (0.0079762f64
// CHECK-NEXT:                     + (8.9909e-5f64
// CHECK-NEXT:                         + (7.9687e-7f64
// CHECK-NEXT:                             + (5.5989e-9f64 + (3.0866e-11f64 + 1.2868e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         79 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((159 as i32) as f64);
// CHECK-NEXT:             return 0.41914f64
// CHECK-NEXT:                 + (0.0083456f64
// CHECK-NEXT:                     + (9.4827e-5f64
// CHECK-NEXT:                         + (8.4291e-7f64
// CHECK-NEXT:                             + (5.9154e-9f64 + (3.2441e-11f64 + 1.3387e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         80 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((161 as i32) as f64);
// CHECK-NEXT:             return 0.43621f64
// CHECK-NEXT:                 + (0.0087352f64
// CHECK-NEXT:                     + (0.00010002f64
// CHECK-NEXT:                         + (8.9156e-7f64
// CHECK-NEXT:                             + (6.248e-9f64 + (3.4079e-11f64 + 1.3917e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         81 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((163 as i32) as f64);
// CHECK-NEXT:             return 0.45409f64
// CHECK-NEXT:                 + (0.0091463f64
// CHECK-NEXT:                     + (0.00010553f64
// CHECK-NEXT:                         + (9.4293e-7f64
// CHECK-NEXT:                             + (6.5972e-9f64 + (3.5782e-11f64 + 1.4455e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         82 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((165 as i32) as f64);
// CHECK-NEXT:             return 0.47282f64
// CHECK-NEXT:                 + (0.0095799f64
// CHECK-NEXT:                     + (0.00011135f64
// CHECK-NEXT:                         + (9.9716e-7f64
// CHECK-NEXT:                             + (6.9638e-9f64 + (3.7549e-11f64 + 1.5003e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         83 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((167 as i32) as f64);
// CHECK-NEXT:             return 0.49243f64
// CHECK-NEXT:                 + (0.010037f64
// CHECK-NEXT:                     + (0.0001175f64
// CHECK-NEXT:                         + (1.0544e-6f64
// CHECK-NEXT:                             + (7.3484e-9f64 + (3.9383e-11f64 + 1.5559e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         84 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((169 as i32) as f64);
// CHECK-NEXT:             return 0.51298f64
// CHECK-NEXT:                 + (0.01052f64
// CHECK-NEXT:                     + (0.000124f64
// CHECK-NEXT:                         + (1.1147e-6f64
// CHECK-NEXT:                             + (7.7517e-9f64 + (4.1283e-11f64 + 1.6122e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         85 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((171 as i32) as f64);
// CHECK-NEXT:             return 0.53453f64
// CHECK-NEXT:                 + (0.01103f64
// CHECK-NEXT:                     + (0.00013088f64
// CHECK-NEXT:                         + (1.1784e-6f64
// CHECK-NEXT:                             + (8.1743e-9f64 + (4.3252e-11f64 + 1.6692e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         86 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((173 as i32) as f64);
// CHECK-NEXT:             return 0.55712f64
// CHECK-NEXT:                 + (0.011568f64
// CHECK-NEXT:                     + (0.00013815f64
// CHECK-NEXT:                         + (1.2456e-6f64
// CHECK-NEXT:                             + (8.6169e-9f64 + (4.529e-11f64 + 1.7268e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         87 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((175 as i32) as f64);
// CHECK-NEXT:             return 0.58082f64
// CHECK-NEXT:                 + (0.012135f64
// CHECK-NEXT:                     + (0.00014584f64
// CHECK-NEXT:                         + (1.3164e-6f64
// CHECK-NEXT:                             + (9.0803e-9f64 + (4.7397e-11f64 + 1.785e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         88 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((177 as i32) as f64);
// CHECK-NEXT:             return 0.60569f64
// CHECK-NEXT:                 + (0.012735f64
// CHECK-NEXT:                     + (0.00015396f64
// CHECK-NEXT:                         + (1.3909e-6f64
// CHECK-NEXT:                             + (9.5651e-9f64 + (4.9574e-11f64 + 1.8435e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         89 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((179 as i32) as f64);
// CHECK-NEXT:             return 0.63178f64
// CHECK-NEXT:                 + (0.013368f64
// CHECK-NEXT:                     + (0.00016254f64
// CHECK-NEXT:                         + (1.4695e-6f64
// CHECK-NEXT:                             + (1.0072e-8f64 + (5.1822e-11f64 + 1.9025e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         90 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((181 as i32) as f64);
// CHECK-NEXT:             return 0.65918f64
// CHECK-NEXT:                 + (0.014036f64
// CHECK-NEXT:                     + (0.0001716f64
// CHECK-NEXT:                         + (1.5521e-6f64
// CHECK-NEXT:                             + (1.0601e-8f64 + (5.414e-11f64 + 1.9616e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         91 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((183 as i32) as f64);
// CHECK-NEXT:             return 0.68795f64
// CHECK-NEXT:                 + (0.014741f64
// CHECK-NEXT:                     + (0.00018117f64
// CHECK-NEXT:                         + (1.6392e-6f64
// CHECK-NEXT:                             + (1.1155e-8f64 + (5.653e-11f64 + 2.0209e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         92 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((185 as i32) as f64);
// CHECK-NEXT:             return 0.71818f64
// CHECK-NEXT:                 + (0.015486f64
// CHECK-NEXT:                     + (0.00019128f64
// CHECK-NEXT:                         + (1.7307e-6f64
// CHECK-NEXT:                             + (1.1732e-8f64 + (5.8991e-11f64 + 2.0803e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         93 => {
// CHECK-NEXT:             t = ((2 as i32) as f64) * x - ((187 as i32) as f64);
// CHECK-NEXT:             return 0.74993f64
// CHECK-NEXT:                 + (0.016272f64
// CHECK-NEXT:                     + (0.00020195f64
// CHECK-NEXT:                         + (1.8269e-6f64
// CHECK-NEXT:                             + (1.2335e-8f64 + (6.1523e-11f64 + 2.1395e-13f64 * t) * t) * t)
// CHECK-NEXT:                             * t)
// CHECK-NEXT:                         * t)
// CHECK-NEXT:                     * t;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     1.0f64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: f64 = foo(78.4f64);
// CHECK-NEXT:     if d < 0.38f64 || d > 0.42f64 {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
