use crate::Complex;

unsafe extern "C" {
    fn scalbn(x: f64, n: i32) -> f64;
    fn logb(x: f64) -> f64;
    fn scalbnf(x: f32, n: i32) -> f32;
    fn logbf(x: f32) -> f32;
}

macro_rules! float_ops {
    ($t:ident, $mul:ident, $div:ident, $scalbn:ident, $logb:ident) => {
        pub fn $mul(l: Complex<$t>, r: Complex<$t>, basic: bool) -> Complex<$t> {
            let (mut a, mut b, mut c, mut d) = (l.re, l.im, r.re, r.im);
            let (ac, bd, ad, bc) = (a * c, b * d, a * d, b * c);
            let mut x = ac - bd;
            let mut y = ad + bc;
            if basic || !(x.is_nan() && y.is_nan()) {
                return Complex { re: x, im: y };
            }
            let unit = |v: $t| (if v.is_infinite() { 1.0 } else { 0.0 as $t }).copysign(v);
            let mut recalc = false;
            if a.is_infinite() || b.is_infinite() {
                a = unit(a);
                b = unit(b);
                if c.is_nan() {
                    c = (0.0 as $t).copysign(c);
                }
                if d.is_nan() {
                    d = (0.0 as $t).copysign(d);
                }
                recalc = true;
            }
            if c.is_infinite() || d.is_infinite() {
                c = unit(c);
                d = unit(d);
                if a.is_nan() {
                    a = (0.0 as $t).copysign(a);
                }
                if b.is_nan() {
                    b = (0.0 as $t).copysign(b);
                }
                recalc = true;
            }
            if !recalc
                && (ac.is_infinite() || bd.is_infinite() || ad.is_infinite() || bc.is_infinite())
            {
                for v in [&mut a, &mut b, &mut c, &mut d] {
                    if v.is_nan() {
                        *v = (0.0 as $t).copysign(*v);
                    }
                }
                recalc = true;
            }
            if recalc {
                x = $t::INFINITY * (a * c - b * d);
                y = $t::INFINITY * (a * d + b * c);
            }
            Complex { re: x, im: y }
        }

        pub fn $div(l: Complex<$t>, r: Complex<$t>, basic: bool) -> Complex<$t> {
            let (mut a, mut b, mut c, mut d) = (l.re, l.im, r.re, r.im);
            if basic {
                let denom = c * c + d * d;
                return Complex {
                    re: (a * c + b * d) / denom,
                    im: (b * c - a * d) / denom,
                };
            }
            let mut ilogbw = 0;
            let logbw = unsafe { $logb(c.abs().max(d.abs())) };
            if logbw.is_finite() {
                ilogbw = logbw as i32;
                c = unsafe { $scalbn(c, -ilogbw) };
                d = unsafe { $scalbn(d, -ilogbw) };
            }
            let denom = c * c + d * d;
            let mut x = unsafe { $scalbn((a * c + b * d) / denom, -ilogbw) };
            let mut y = unsafe { $scalbn((b * c - a * d) / denom, -ilogbw) };
            if x.is_nan() && y.is_nan() {
                let unit = |v: $t| (if v.is_infinite() { 1.0 } else { 0.0 as $t }).copysign(v);
                if denom == 0.0 && (!a.is_nan() || !b.is_nan()) {
                    x = $t::INFINITY.copysign(c) * a;
                    y = $t::INFINITY.copysign(c) * b;
                } else if (a.is_infinite() || b.is_infinite()) && c.is_finite() && d.is_finite()
                {
                    a = unit(a);
                    b = unit(b);
                    x = $t::INFINITY * (a * c + b * d);
                    y = $t::INFINITY * (b * c - a * d);
                } else if logbw.is_infinite() && a.is_finite() && b.is_finite() {
                    c = unit(c);
                    d = unit(d);
                    x = 0.0 * (a * c + b * d);
                    y = 0.0 * (b * c - a * d);
                }
            }
            Complex { re: x, im: y }
        }
    };
}


float_ops!(f32, mul_f32, div_f32, scalbnf, logbf);
float_ops!(f64, mul_f64, div_f64, scalbn, logb);

macro_rules! float_methods {
    ($($t:ident, $mul:ident, $div:ident);*) => {$(
        impl Complex<$t> {
            #[inline]
            pub fn c_mul(self, other: Self, basic: bool) -> Self {
                $mul(self, other, basic)
            }

            #[inline]
            pub fn c_div(self, other: Self, basic: bool) -> Self {
                $div(self, other, basic)
            }
        }
    )*};
}

float_methods!(f32, mul_f32, div_f32; f64, mul_f64, div_f64);

#[cfg(has_f16)]
impl Complex<f16> {
    fn widen(self) -> Complex<f32> {
        Complex {
            re: self.re as f32,
            im: self.im as f32,
        }
    }

    fn narrow(wide: Complex<f32>) -> Self {
        Complex {
            re: wide.re as f16,
            im: wide.im as f16,
        }
    }

    #[inline]
    pub fn c_mul(self, other: Self, basic: bool) -> Self {
        let x = self.re * other.re - self.im * other.im;
        let y = self.re * other.im + self.im * other.re;
        if basic || !(x.is_nan() && y.is_nan()) {
            return Complex { re: x, im: y };
        }
        Self::narrow(mul_f32(self.widen(), other.widen(), false))
    }

    #[inline]
    pub fn c_div(self, other: Self, basic: bool) -> Self {
        Self::narrow(div_f32(self.widen(), other.widen(), basic))
    }
}

#[cfg(has_f128)]
unsafe extern "C" {
    fn __multc3(a: f128, b: f128, c: f128, d: f128) -> Complex<f128>;
    fn __divtc3(a: f128, b: f128, c: f128, d: f128) -> Complex<f128>;
}

#[cfg(has_f128)]
impl Complex<f128> {
    #[inline]
    pub fn c_mul(self, other: Self, basic: bool) -> Self {
        let x = self.re * other.re - self.im * other.im;
        let y = self.re * other.im + self.im * other.re;
        if basic || !(x.is_nan() && y.is_nan()) {
            return Complex { re: x, im: y };
        }
        unsafe { __multc3(self.re, self.im, other.re, other.im) }
    }

    #[inline]
    pub fn c_div(self, other: Self, basic: bool) -> Self {
        if basic {
            let denom = other.re * other.re + other.im * other.im;
            return Complex {
                re: (self.re * other.re + self.im * other.im) / denom,
                im: (self.im * other.re - self.re * other.im) / denom,
            };
        }
        unsafe { __divtc3(self.re, self.im, other.re, other.im) }
    }
}

macro_rules! int_methods {
    ($($t:ident),*) => {$(
        impl Complex<$t> {
            #[inline]
            pub fn c_mul(self, r: Self, _basic: bool) -> Self {
                Complex {
                    re: self.re.wrapping_mul(r.re).wrapping_sub(self.im.wrapping_mul(r.im)),
                    im: self.re.wrapping_mul(r.im).wrapping_add(self.im.wrapping_mul(r.re)),
                }
            }

            #[inline]
            pub fn c_div(self, r: Self, _basic: bool) -> Self {
                let denom = r.re.wrapping_mul(r.re).wrapping_add(r.im.wrapping_mul(r.im));
                Complex {
                    re: self.re.wrapping_mul(r.re).wrapping_add(self.im.wrapping_mul(r.im)) / denom,
                    im: self.im.wrapping_mul(r.re).wrapping_sub(self.re.wrapping_mul(r.im)) / denom,
                }
            }
        }
    )*};
}

int_methods!(i8, u8, i16, u16, i32, u32, i64, u64, i128, u128);
