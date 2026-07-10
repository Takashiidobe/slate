//! A stateful, csmith-style generator for Slate's supported C subset.
//!
//! Unlike the BNF expander (which stitches fixed template fragments together),
//! this generator maintains *generation state* — a stack of scopes holding the
//! variables that are currently declared, plus the signatures of functions
//! emitted so far — and composes expressions from whatever is live. That lets it
//! exercise variable scope, expression composition, reusable declarations, and
//! passing scoped locals to functions.
//!
//! Everything it emits is restricted to the subset Slate can translate today
//! (int arithmetic with `+`/`-`/`*`/`/`/`%`/bitwise ops/`++`/`+=`,
//! `float`/`double` arithmetic with `+`/`-`/`*`/`/`, comparisons, `for`/`while`,
//! arrays, pointers, structs, unions, typedef aliases, fixed-width typedefs,
//! `_Bool`/`bool`, `sizeof`, type qualifiers, static globals, enum constants, and
//! `printf("%d\n", ...)` / `printf("%f\n", ...)`).
//!
//! Correctness rests on keeping the two sides in agreement on the operations
//! the differential harness actually compares. Integer overflow is *not* one of
//! the things kept out of bounds: clang builds the C side at `-O0` (two's
//! complement wrap) and the Rust batch crate sets `overflow-checks = false`, so
//! both wrap identically. What the generator still guarantees is the arithmetic
//! that traps on both sides regardless of overflow flags: divisors are nonzero
//! constants (no division by zero, no `INT_MIN / -1`). It also carries a
//! conservative upper bound on every value ([`VALUE_CAP`]) to keep programs
//! tame, keeps array indices in range, and initializes every variable before it
//! is read.

#![allow(dead_code)]

/// Never let a tracked value bound exceed this; well under `i32::MAX` so no
/// addition can overflow.
const VALUE_CAP: i64 = 1_000_000;
/// Integer literals stay in `0..=CONST_MAX`.
const CONST_MAX: i64 = 9;
/// Function parameters are assumed to receive values in `0..=PARAM_MAX`.
const PARAM_MAX: i64 = 20;
/// Budget for a top-level declaration initializer.
const DECL_BUDGET: i64 = 200;
/// Budget for a loop step / compound-assignment right-hand side.
const STEP_BUDGET: i64 = 6;
/// Maximum expression tree depth.
const MAX_DEPTH: usize = 3;

/// Deterministic LCG. Same generator, same seed => byte-identical program.
struct Rng {
    state: u64,
}

impl Rng {
    fn new(seed: u64) -> Self {
        Self {
            state: seed ^ 0x9e37_79b9_7f4a_7c15,
        }
    }

    fn next(&mut self) -> u64 {
        self.state = self
            .state
            .wrapping_mul(6364136223846793005)
            .wrapping_add(1442695040888963407);
        self.state
    }

    /// Uniform in `0..n` (n must be > 0).
    fn below(&mut self, n: usize) -> usize {
        (self.next() >> 33) as usize % n
    }

    /// Uniform in `lo..=hi`.
    fn int_in(&mut self, lo: i64, hi: i64) -> i64 {
        if hi <= lo {
            return lo;
        }
        lo + (self.next() >> 33) as i64 % (hi - lo + 1)
    }

    /// True with probability `percent`/100.
    fn chance(&mut self, percent: u64) -> bool {
        self.next() % 100 < percent
    }
}

#[derive(Clone)]
struct Var {
    name: String,
    max_abs: i64,
}

#[derive(Clone)]
struct FuncSig {
    name: String,
    arity: usize,
    ret_max_abs: i64,
}

struct Gen {
    rng: Rng,
    out: String,
    indent: usize,
    scopes: Vec<Vec<Var>>,
    funcs: Vec<FuncSig>,
    var_counter: usize,
    // top-level feature flags, decided per program
    has_enum: bool,
    has_struct: bool,
    has_union: bool,
    has_global: bool,
    has_float: bool,
    has_char: bool,
    has_wideint: bool,
    has_typedef: bool,
    has_fixed_width: bool,
    has_bool: bool,
    has_qualified_types: bool,
    has_function_pointer: bool,
}

/// Generate a complete, self-contained C program for `seed`.
pub fn generate(seed: u64) -> String {
    let mut g = Gen {
        rng: Rng::new(seed),
        out: String::new(),
        indent: 0,
        scopes: Vec::new(),
        funcs: Vec::new(),
        var_counter: 0,
        has_enum: false,
        has_struct: false,
        has_union: false,
        has_global: false,
        has_float: false,
        has_char: false,
        has_wideint: false,
        has_typedef: false,
        has_fixed_width: false,
        has_bool: false,
        has_qualified_types: false,
        has_function_pointer: false,
    };
    g.program();
    g.out
}

impl Gen {
    fn program(&mut self) {
        self.has_enum = self.rng.chance(60);
        self.has_struct = self.rng.chance(70);
        self.has_union = self.rng.chance(60);
        self.has_global = self.rng.chance(70);
        self.has_float = self.rng.chance(70);
        self.has_char = self.rng.chance(70);
        self.has_wideint = self.rng.chance(70);
        self.has_typedef = self.rng.chance(70);
        self.has_fixed_width = self.rng.chance(70);
        self.has_bool = self.rng.chance(70);
        self.has_qualified_types = self.rng.chance(70);
        self.has_function_pointer = self.rng.chance(70);

        if self.has_bool {
            self.line("#include <stdbool.h>");
        }
        if self.has_fixed_width {
            self.line("#include <stddef.h>");
            self.line("#include <stdint.h>");
        }
        self.line("#include <stdio.h>");
        self.blank();

        if self.has_typedef {
            self.emit_typedef_decl();
        }
        if self.has_enum {
            self.emit_enum_decl();
        }
        if self.has_struct {
            self.emit_struct_decl();
        }
        if self.has_union {
            self.emit_union_decl();
        }
        if self.has_qualified_types {
            self.emit_qualified_struct_decl();
        }
        if self.has_global {
            self.emit_global_decl();
        } else if self.has_qualified_types {
            self.emit_qualified_global_decls();
        }
        if self.has_char {
            self.emit_char_fn();
        }
        if self.has_float {
            self.emit_float_fn();
        }
        if self.has_bool {
            self.emit_bool_fns();
        }
        if self.has_qualified_types {
            self.emit_qualified_fns();
        }
        if self.has_function_pointer {
            self.emit_function_pointer_fns();
        }

        // The static-counter function is emitted verbatim and, crucially, is
        // *not* registered in `funcs`, so random expressions never call it and
        // the global's value stays exactly `init + call count`.
        if self.has_global {
            self.emit_bump_fn();
        }

        let helper_count = self.rng.int_in(2, 4) as usize;
        for i in 0..helper_count {
            self.emit_helper(i);
        }

        self.emit_main();
    }

    // ----- top-level declarations (mirrors the proven fixture shapes) -----

    fn emit_enum_decl(&mut self) {
        self.line("enum FuzzEnum {");
        self.line("    FuzzZero,");
        self.line("    FuzzOne,");
        self.line("    FuzzFive = 5,");
        self.line("    FuzzSix,");
        self.line("    FuzzNeg = -2,");
        self.line("    FuzzNegNext");
        self.line("};");
        self.blank();
    }

    fn emit_typedef_decl(&mut self) {
        self.line("typedef int fuzz_int;");
        self.line("typedef unsigned char fuzz_byte;");
        self.blank();
    }

    fn emit_struct_decl(&mut self) {
        self.line("struct FuzzStruct {");
        self.line(&format!("    {} left;", self.int_type()));
        self.line(&format!("    {} right;", self.int_type()));
        if self.has_char {
            self.line("    char ch;");
        }
        if self.has_float {
            self.line("    double weight;");
        }
        if self.has_typedef {
            self.line("    fuzz_byte tag;");
        }
        self.line("};");
        self.blank();
    }

    fn emit_union_decl(&mut self) {
        self.line("union FuzzPair {");
        self.line("    int left;");
        self.line("    int right;");
        if self.has_char {
            self.line("    char ch;");
        }
        if self.has_float {
            self.line("    double weight;");
        }
        self.line("};");
        self.blank();
    }

    fn emit_qualified_struct_decl(&mut self) {
        self.line("struct FuzzQualified {");
        self.line("    volatile int count;");
        self.line("    volatile double weight;");
        self.line("};");
        self.blank();
    }

    fn emit_global_decl(&mut self) {
        let init = self.rng.int_in(0, CONST_MAX);
        self.line(&format!("static int fuzz_counter = {init};"));
        if self.has_qualified_types {
            self.emit_qualified_global_decls();
        }
        if self.has_char {
            let ch = self.rng.int_in(0, 63);
            self.line(&format!("static char fuzz_char_global = {ch};"));
        }
        if self.has_float {
            let fp = self.rng.int_in(1, CONST_MAX);
            self.line(&format!("static double fuzz_double_global = {fp}.25;"));
        }
        self.blank();
    }

    fn emit_qualified_global_decls(&mut self) {
        let fp = self.rng.int_in(1, CONST_MAX);
        self.line(&format!(
            "static volatile double fuzz_volatile_global = {fp}.5;"
        ));
        let bias = self.rng.int_in(1, CONST_MAX);
        self.line(&format!("static const int fuzz_const_bias = {bias};"));
        let atomic = self.rng.int_in(1, CONST_MAX);
        self.line(&format!(
            "static _Atomic int fuzz_atomic_counter = {atomic};"
        ));
    }

    fn emit_bump_fn(&mut self) {
        self.line("static int fuzz_bump(void) {");
        self.line("    fuzz_counter = fuzz_counter + 1;");
        self.line("    return fuzz_counter;");
        self.line("}");
        self.blank();
    }

    fn emit_qualified_fns(&mut self) {
        self.line("static volatile int fuzz_volatile_return(int value) {");
        self.line("    return value + 1;");
        self.line("}");
        self.blank();
        self.line("static double fuzz_volatile_param(volatile double value) {");
        self.line("    return value + 0.5;");
        self.line("}");
        self.blank();
        self.line("static int fuzz_const_param(const int value) {");
        self.line("    const int local_bias = 2;");
        self.line("    return value + local_bias + fuzz_const_bias;");
        self.line("}");
        self.blank();
        self.line("static int fuzz_restrict_param(int *restrict value) {");
        self.line("    return *value + 1;");
        self.line("}");
        self.blank();
        self.line("static int fuzz_atomic_param(_Atomic int value) {");
        self.line("    _Atomic int local = value + fuzz_atomic_counter;");
        self.line("    return local;");
        self.line("}");
        self.blank();
    }

    fn emit_char_fn(&mut self) {
        self.line("static char fuzz_char_add(char a, char b) {");
        self.line("    return a + b;");
        self.line("}");
        self.blank();
    }

    fn emit_float_fn(&mut self) {
        self.line("static double fuzz_double_mix(double a, double b) {");
        self.line("    return a + b * 2.0;");
        self.line("}");
        self.blank();
    }

    fn emit_bool_fns(&mut self) {
        self.line("static bool fuzz_bool_from_int(int x) {");
        self.line("    bool b = x;");
        self.line("    return b;");
        self.line("}");
        self.blank();
        self.line("static int fuzz_bool_param(_Bool flag) {");
        self.line("    return flag;");
        self.line("}");
        self.blank();
    }

    fn emit_function_pointer_fns(&mut self) {
        self.line("static int fuzz_fp_add(int lhs, int rhs) {");
        self.line("    return lhs + rhs;");
        self.line("}");
        self.blank();
        self.line("static int fuzz_fp_apply(int (*op)(int, int), int lhs, int rhs) {");
        self.line("    return op(lhs, rhs);");
        self.line("}");
        self.blank();
    }

    // ----- helper functions -----

    fn emit_helper(&mut self, idx: usize) {
        let name = format!("fuzz_fn{idx}");
        let arity = self.rng.int_in(0, 3) as usize;

        self.push_scope();
        let params: Vec<String> = (0..arity)
            .map(|i| {
                let p = format!("p{i}");
                self.declare(&p, PARAM_MAX);
                format!("{} {p}", self.int_type())
            })
            .collect();
        let sig = if params.is_empty() {
            "void".to_string()
        } else {
            params.join(", ")
        };
        self.line(&format!("static {} {name}({sig}) {{", self.int_type()));
        self.indent = 1;

        // Always start with a declaration so there is something in scope.
        self.emit_decl_stmt();
        let extra = self.rng.int_in(1, 3);
        for _ in 0..extra {
            self.emit_helper_stmt();
        }

        let (ret_expr, ret_max) = self.gen_expr(0, VALUE_CAP);
        self.line(&format!("return {ret_expr};"));

        self.indent = 0;
        self.line("}");
        self.blank();
        self.pop_scope();

        self.funcs.push(FuncSig {
            name,
            arity,
            ret_max_abs: ret_max,
        });
    }

    fn emit_helper_stmt(&mut self) {
        match self.rng.below(4) {
            0 => self.emit_decl_stmt(),
            1 => self.emit_assign_stmt(),
            2 => self.emit_compound_stmt(),
            _ => self.emit_for_stmt(),
        }
    }

    fn emit_decl_stmt(&mut self) {
        let name = self.fresh("v");
        let (expr, max) = self.gen_expr(0, DECL_BUDGET);
        self.line(&format!("{} {name} = {expr};", self.int_type()));
        self.declare(&name, max);
    }

    fn emit_assign_stmt(&mut self) {
        let Some(target) = self.pick_var(VALUE_CAP) else {
            return self.emit_decl_stmt();
        };
        let (expr, max) = self.gen_expr(0, DECL_BUDGET);
        self.line(&format!("{} = {expr};", target.name));
        self.set_max_abs(&target.name, max);
    }

    fn emit_compound_stmt(&mut self) {
        let Some(target) = self.pick_var(VALUE_CAP - STEP_BUDGET) else {
            return self.emit_decl_stmt();
        };
        let (expr, max) = self.gen_expr(0, STEP_BUDGET);
        self.line(&format!("{} += {expr};", target.name));
        self.set_max_abs(&target.name, target.max_abs + max);
    }

    /// `int acc = 0; for (int i = 0; i <= N; i++) { acc += <step>; }`
    fn emit_for_stmt(&mut self) {
        let acc = self.fresh("acc");
        self.line(&format!("{} {acc} = 0;", self.int_type()));
        self.declare(&acc, 0);

        let bound = self.rng.int_in(0, 5);
        let idx = self.fresh("i");
        self.line(&format!(
            "for ({} {idx} = 0; {idx} <= {bound}; {idx}++) {{",
            self.int_type()
        ));
        self.indent += 1;

        self.push_scope();
        self.declare(&idx, bound);
        let (step, step_max) = self.gen_expr(0, STEP_BUDGET);
        self.line(&format!("{acc} += {step};"));
        self.pop_scope();

        self.indent -= 1;
        self.line("}");

        let acc_max = ((bound + 1) * step_max).min(VALUE_CAP);
        self.set_max_abs(&acc, acc_max);
    }

    // ----- expressions -----

    /// Returns `(c_text, max_abs)`; `max_abs` is a conservative bound on the
    /// absolute value of the expression, always `<= budget` and `<= VALUE_CAP`.
    fn gen_expr(&mut self, depth: usize, budget: i64) -> (String, i64) {
        let budget = budget.clamp(0, VALUE_CAP);
        if depth >= MAX_DEPTH || budget < 2 {
            return self.gen_leaf(budget);
        }
        match self.rng.below(14) {
            0 | 1 => self.gen_leaf(budget),
            2 => {
                // addition: split the budget so the sum stays within it
                let (le, lm) = self.gen_expr(depth + 1, budget / 2);
                let (re, rm) = self.gen_expr(depth + 1, budget - lm);
                (format!("({le} + {re})"), lm + rm)
            }
            3 => {
                // subtraction: each side is within budget, so |a - b| <= budget.
                // signed results may go negative, which is well-defined.
                let (le, lm) = self.gen_expr(depth + 1, budget);
                let (re, rm) = self.gen_expr(depth + 1, budget);
                (format!("({le} - {re})"), lm.max(rm))
            }
            4 => {
                // multiplication: bound the product by budget via factor budgets
                // (lm <= sqrt(budget), rm <= budget / lm => lm * rm <= budget).
                let (le, lm) = self.gen_expr(depth + 1, budget.isqrt());
                let rb = if lm > 0 { budget / lm } else { budget };
                let (re, rm) = self.gen_expr(depth + 1, rb);
                (format!("({le} * {re})"), (lm * rm).min(VALUE_CAP))
            }
            5 => {
                // division by a nonzero constant: never divides by zero and
                // avoids INT_MIN/-1; C and Rust both truncate toward zero.
                let (le, lm) = self.gen_expr(depth + 1, budget);
                let d = self.rng.int_in(1, CONST_MAX);
                (format!("({le} / {d})"), lm)
            }
            6 => {
                // modulo by a nonzero constant: |a % d| < d and <= |a|; C and
                // Rust both take the sign of the dividend.
                let (le, lm) = self.gen_expr(depth + 1, budget);
                let d = self.rng.int_in(1, CONST_MAX);
                (format!("({le} % {d})"), lm.min(d - 1).max(0))
            }
            7 => {
                let (le, lm) = self.gen_expr(depth + 1, budget);
                let mask = self.bit_mask(budget);
                (format!("({le} & {mask})"), lm.min(mask))
            }
            8 => {
                let mask = self.bit_mask(budget);
                let lhs_budget = (budget - mask).max(0);
                let (le, lm) = self.gen_expr(depth + 1, lhs_budget);
                (format!("({le} | {mask})"), (lm + mask).min(VALUE_CAP))
            }
            9 => {
                let mask = self.bit_mask(budget);
                let lhs_budget = (budget - mask).max(0);
                let (le, lm) = self.gen_expr(depth + 1, lhs_budget);
                (format!("({le} ^ {mask})"), (lm + mask).min(VALUE_CAP))
            }
            10 => {
                let mask = self.bit_mask(budget);
                let (le, _) = self.gen_expr(depth + 1, mask);
                (format!("(~({le}) & {mask})"), mask)
            }
            11 => {
                let shift = self.rng.int_in(0, 4);
                let lhs_budget = budget.checked_shr(shift as u32).unwrap_or(0).max(1);
                let mask = self.bit_mask(lhs_budget);
                let (le, _) = self.gen_expr(depth + 1, mask);
                let max = (mask << shift).min(VALUE_CAP);
                (format!("(({le} & {mask}) << {shift})"), max)
            }
            12 => {
                let shift = self.rng.int_in(0, 4);
                let mask = self.bit_mask(budget);
                let shifted_mask = (mask << shift).min(VALUE_CAP);
                let (le, _) = self.gen_expr(depth + 1, shifted_mask);
                (format!("(({le} & {shifted_mask}) >> {shift})"), mask)
            }
            _ => self.gen_call(depth, budget),
        }
    }

    fn bit_mask(&mut self, budget: i64) -> i64 {
        let max = budget.clamp(1, 255);
        let bits = (0..=7)
            .rev()
            .find(|bits| (1_i64 << bits) - 1 <= max)
            .unwrap_or(1);
        (1_i64 << self.rng.int_in(1, bits)) - 1
    }

    fn gen_leaf(&mut self, budget: i64) -> (String, i64) {
        if self.rng.chance(50)
            && let Some(v) = self.pick_var(budget)
        {
            return (v.name, v.max_abs);
        }
        let c = self.rng.int_in(0, CONST_MAX.min(budget));
        (c.to_string(), c)
    }

    fn gen_call(&mut self, depth: usize, budget: i64) -> (String, i64) {
        let callable: Vec<FuncSig> = self
            .funcs
            .iter()
            .filter(|f| f.ret_max_abs <= budget)
            .cloned()
            .collect();
        if callable.is_empty() {
            return self.gen_leaf(budget);
        }
        let f = callable[self.rng.below(callable.len())].clone();
        // Arguments are kept within the callee's assumed parameter range so the
        // recorded return bound stays valid.
        let args: Vec<String> = (0..f.arity)
            .map(|_| self.gen_expr(depth + 1, PARAM_MAX).0)
            .collect();
        (format!("{}({})", f.name, args.join(", ")), f.ret_max_abs)
    }

    // ----- main -----

    fn emit_main(&mut self) {
        self.push_scope();
        self.line("int main(void) {");
        self.indent = 1;

        // A couple of scoped locals main can pass to functions.
        self.emit_decl_stmt();
        if self.rng.chance(70) {
            self.emit_decl_stmt();
        }
        if self.rng.chance(50) {
            self.emit_for_stmt();
        }

        // Print each helper's result, feeding scoped values as arguments.
        let funcs = self.funcs.clone();
        for f in &funcs {
            let args: Vec<String> = (0..f.arity)
                .map(|_| self.gen_expr(0, PARAM_MAX).0)
                .collect();
            self.printf(&format!("{}({})", f.name, args.join(", ")));
        }

        if self.has_global {
            self.printf("fuzz_bump()");
            self.printf("fuzz_bump()");
        }
        if self.has_enum {
            self.printf("FuzzFive");
        }
        if self.has_struct {
            self.emit_struct_use();
        }
        if self.has_union {
            self.emit_union_use();
        }
        if self.has_float {
            self.emit_float_use();
        }
        if self.has_char {
            self.emit_char_use();
        }
        if self.has_wideint {
            self.emit_wideint_use();
        }
        if self.has_fixed_width {
            self.emit_fixed_width_use();
        }
        if self.has_bool {
            self.emit_bool_use();
        }
        if self.has_qualified_types {
            self.emit_qualified_type_use();
        }
        if self.has_function_pointer {
            self.emit_function_pointer_use();
        }
        self.emit_array_use();
        self.emit_pointer_use();

        self.line("return 0;");
        self.indent = 0;
        self.line("}");
        self.pop_scope();
    }

    fn emit_struct_use(&mut self) {
        let a = self.rng.int_in(0, CONST_MAX);
        let b = self.rng.int_in(0, CONST_MAX);
        self.line("struct FuzzStruct s;");
        self.line(&format!("s.left = {a};"));
        self.line(&format!("s.right = {b};"));
        self.printf("(s.left + s.right)");
        if self.has_char {
            let ch = self.rng.int_in(0, 63);
            self.line(&format!("s.ch = {ch};"));
            self.printf("s.ch");
        }
        if self.has_float {
            let weight = self.rng.int_in(1, CONST_MAX);
            self.line(&format!("s.weight = {weight}.5;"));
            self.printf_float("s.weight");
        }
        if self.has_typedef {
            let tag = self.rng.int_in(0, 200);
            self.line(&format!("s.tag = {tag};"));
            self.printf("s.tag");
        }
    }

    fn emit_union_use(&mut self) {
        let a = self.rng.int_in(0, CONST_MAX);
        self.line("union FuzzPair u;");
        self.line(&format!("u.left = {a};"));
        self.printf("u.left");
        if self.has_char {
            let ch = self.rng.int_in(0, 63);
            self.line(&format!("u.ch = {ch};"));
            self.printf("u.ch");
        }
        if self.has_float {
            let weight = self.rng.int_in(1, CONST_MAX);
            self.line(&format!("u.weight = {weight}.25;"));
            self.printf_float("u.weight");
        }
    }

    fn emit_array_use(&mut self) {
        let n = self.rng.int_in(2, 4);
        self.line(&format!("int arr[{n}];"));
        for i in 0..n {
            let v = self.rng.int_in(0, CONST_MAX);
            self.line(&format!("arr[{i}] = {v};"));
        }
        let idx = self.rng.int_in(0, n - 1);
        self.printf(&format!("arr[{idx}]"));
        if self.has_char {
            let ch = self.rng.int_in(0, 63);
            self.line("char chars[2];");
            self.line(&format!("chars[0] = {ch};"));
            self.line("chars[1] = chars[0] + 1;");
            self.printf("chars[1]");
        }
        if self.has_float {
            let weight = self.rng.int_in(1, CONST_MAX);
            self.line("double weights[2];");
            self.line(&format!("weights[0] = {weight}.5;"));
            self.line("weights[1] = weights[0] + 1.25;");
            self.printf_float("weights[1]");
        }

        // sizeof over primitive + the array is a compile-time constant on both
        // sides, so C and translated Rust agree.
        if self.rng.chance(50) {
            self.line(&format!(
                "{} sz = sizeof({}) + sizeof(arr);",
                self.int_type(),
                self.int_type()
            ));
            self.printf("sz");
        }

        // a volatile local, exercised like the volatile fixture
        if self.rng.chance(50) {
            let init = self.rng.int_in(0, CONST_MAX);
            let bump = self.rng.int_in(0, CONST_MAX);
            self.line(&format!("volatile int slot = {init};"));
            self.line(&format!("slot = slot + {bump};"));
            self.printf("slot");
        }
    }

    fn emit_pointer_use(&mut self) {
        let init = self.rng.int_in(0, CONST_MAX);
        let bump = self.rng.int_in(0, CONST_MAX);
        let idx = self.rng.int_in(0, 2);
        self.line(&format!("int pointed = {init};"));
        self.line("int *ptr = &pointed;");
        self.line(&format!("*ptr = *ptr + {bump};"));
        self.printf("pointed");
        self.line("int pointer_values[3];");
        self.line("pointer_values[0] = 11;");
        self.line("pointer_values[1] = 22;");
        self.line("pointer_values[2] = 33;");
        self.line("int *walk = pointer_values;");
        self.printf(&format!("*(walk + {idx})"));
    }

    fn emit_function_pointer_use(&mut self) {
        self.line("int (*fp)(int, int) = fuzz_fp_add;");
        self.printf("fp(2, 3)");
        self.printf("fuzz_fp_apply(fp, 4, 5)");
    }

    // Floats print through the same libc::printf on both sides, so `%f` output
    // is byte-identical; identical IEEE-754 f64 ops in the same order agree.
    // Divisors are held nonzero to keep every value well-defined and finite.
    fn emit_float_use(&mut self) {
        let a = self.rng.int_in(1, CONST_MAX);
        let b = self.rng.int_in(1, CONST_MAX);
        self.line(&format!("double da = {a}.0;"));
        self.line(&format!("double db = {b};"));
        self.line("double dc = (da + db) * 2.0 - db / 4.0;");
        self.printf_float("dc");
        self.printf_float("da / db");

        let c = self.rng.int_in(1, CONST_MAX);
        self.line(&format!("float fa = {c}.5f;"));
        self.printf_float("fa * 1.5f");

        if self.has_global {
            self.line("fuzz_double_global = fuzz_double_mix(fuzz_double_global, 1.5);");
            self.printf_float("fuzz_double_global");
        }
    }

    // Char arithmetic promotes to int in C, so operands stay small enough that
    // the result fits in a signed char and no overflow occurs. Values print via
    // `%d` (numeric) and `%c` (byte) identically on both sides. The `%c` value
    // is kept in the printable 'A'..='Z' range so output is stable.
    fn emit_char_use(&mut self) {
        let a = self.rng.int_in(0, 63);
        let b = self.rng.int_in(0, 63);
        self.line(&format!("char ca = {a};"));
        self.line(&format!("signed char cb = {b};"));
        self.line("char cc = ca + cb;");
        self.printf("cc");

        let u = self.rng.int_in(0, 200);
        self.line(&format!("unsigned char uc = {u};"));
        self.printf("uc");

        let off = self.rng.int_in(0, 25);
        self.line(&format!("char letter = 'A' + {off};"));
        self.printf_char("letter");

        if self.has_global {
            self.line("fuzz_char_global = fuzz_char_add(fuzz_char_global, 1);");
            self.printf("fuzz_char_global");
        }
    }

    // short/long/long long map to Rust i16/i64 and unsigned variants to
    // u16/u32/u64. short arithmetic promotes to int (so small operands never
    // overflow); unsigned overflow is defined-wrap on both sides, matched by
    // Slate's wrapping_add lowering. Each value prints with the width-correct
    // conversion so C and Rust emit identical bytes.
    fn emit_wideint_use(&mut self) {
        let a = self.rng.int_in(0, 10000);
        let b = self.rng.int_in(0, 10000);
        self.line(&format!("short sha = {a};"));
        self.line(&format!("unsigned short shb = {b};"));
        self.line("short shc = sha + shb;");
        self.printf("shc");
        self.printf("shb");

        let u = self.rng.int_in(0, CONST_MAX);
        self.line(&format!("unsigned int uia = 4000000000u + {u}u;"));
        self.printf_fmt("%u", "uia");

        let la = self.rng.int_in(0, CONST_MAX);
        self.line(&format!("long lla = 5000000000L + {la};"));
        self.line("unsigned long lua = 9000000000UL;");
        self.printf_fmt("%ld", "lla");
        self.printf_fmt("%lu", "lua");

        let lb = self.rng.int_in(0, CONST_MAX);
        self.line(&format!("long long llb = 9000000000000LL + {lb};"));
        self.line("unsigned long long llub = 18000000000000ULL;");
        self.printf_fmt("%lld", "llb");
        self.printf_fmt("%llu", "llub");
    }

    fn emit_fixed_width_use(&mut self) {
        let small = self.rng.int_in(-9, 9);
        let byte = self.rng.int_in(0, 200);
        let count = self.rng.int_in(1, CONST_MAX);
        self.line(&format!("int8_t fw_small = {small};"));
        self.line(&format!("uint8_t fw_byte = {byte};"));
        self.line("int16_t fw_short = 1200;");
        self.line("uint32_t fw_u32 = 4000000000u;");
        self.line(&format!("size_t fw_count = {count};"));
        self.line("uint64_t fw_wide = fw_u32 + fw_count;");
        self.printf("(fw_small + fw_byte + fw_short)");
        self.printf_fmt("%u", "fw_u32");
        self.printf_fmt("%lu", "fw_count");
        self.printf_fmt("%lu", "fw_wide");
    }

    fn emit_bool_use(&mut self) {
        self.printf("fuzz_bool_from_int(0)");
        self.printf("fuzz_bool_from_int(5)");
        self.printf("fuzz_bool_param(0)");
        self.printf("fuzz_bool_param(2)");
    }

    fn emit_qualified_type_use(&mut self) {
        self.line("struct FuzzQualified vf;");
        self.line("vf.count = fuzz_volatile_return(4);");
        self.line("vf.weight = fuzz_volatile_param(fuzz_volatile_global);");
        self.printf("vf.count");
        self.printf_float("vf.weight");
        self.printf("fuzz_const_param(3)");
        self.line("int restrict_value = 8;");
        self.printf("fuzz_restrict_param(&restrict_value)");
        self.printf("fuzz_atomic_param(5)");
    }

    fn printf(&mut self, expr: &str) {
        self.line(&format!("printf(\"%d\\n\", {expr});"));
    }

    fn printf_char(&mut self, expr: &str) {
        self.line(&format!("printf(\"%c\\n\", {expr});"));
    }

    fn printf_fmt(&mut self, spec: &str, expr: &str) {
        self.line(&format!("printf(\"{spec}\\n\", {expr});"));
    }

    fn printf_float(&mut self, expr: &str) {
        self.line(&format!("printf(\"%f\\n\", {expr});"));
    }

    fn int_type(&self) -> &'static str {
        if self.has_typedef { "fuzz_int" } else { "int" }
    }

    // ----- scope / emission helpers -----

    fn push_scope(&mut self) {
        self.scopes.push(Vec::new());
    }

    fn pop_scope(&mut self) {
        self.scopes.pop();
    }

    fn declare(&mut self, name: &str, max_abs: i64) {
        let max_abs = max_abs.clamp(0, VALUE_CAP);
        self.scopes.last_mut().expect("scope").push(Var {
            name: name.to_string(),
            max_abs,
        });
    }

    fn set_max_abs(&mut self, name: &str, max_abs: i64) {
        let max_abs = max_abs.clamp(0, VALUE_CAP);
        for scope in self.scopes.iter_mut().rev() {
            if let Some(v) = scope.iter_mut().find(|v| v.name == name) {
                v.max_abs = max_abs;
                return;
            }
        }
    }

    fn fresh(&mut self, prefix: &str) -> String {
        let name = format!("{prefix}{}", self.var_counter);
        self.var_counter += 1;
        name
    }

    /// A random in-scope variable whose value bound fits within `budget`.
    fn pick_var(&mut self, budget: i64) -> Option<Var> {
        let candidates: Vec<Var> = self
            .scopes
            .iter()
            .flatten()
            .filter(|v| v.max_abs <= budget)
            .cloned()
            .collect();
        if candidates.is_empty() {
            None
        } else {
            Some(candidates[self.rng.below(candidates.len())].clone())
        }
    }

    fn line(&mut self, text: &str) {
        for _ in 0..self.indent {
            self.out.push_str("    ");
        }
        self.out.push_str(text);
        self.out.push('\n');
    }

    fn blank(&mut self) {
        self.out.push('\n');
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn deterministic_for_a_seed() {
        assert_eq!(generate(1), generate(1));
        assert_ne!(generate(1), generate(2));
    }

    #[test]
    fn emits_expected_skeleton() {
        let src = generate(7);
        assert!(src.contains("#include <stdio.h>"));
        assert!(src.contains("int main(void) {"));
        assert!(src.contains("fuzz_fn0("));
        // multi-function: at least two helpers plus main
        assert!(src.matches("fuzz_fn").count() >= 2);
        assert!(src.contains("printf(\"%d\\n\""));
    }

    #[test]
    fn never_exceeds_value_cap() {
        // The bound tracker must keep every recorded max under the cap for a
        // wide sweep of seeds; if it did not, silent overflow could slip in.
        for seed in 0..256u64 {
            let mut g = Gen {
                rng: Rng::new(seed),
                out: String::new(),
                indent: 0,
                scopes: Vec::new(),
                funcs: Vec::new(),
                var_counter: 0,
                has_enum: false,
                has_struct: false,
                has_union: false,
                has_global: false,
                has_float: false,
                has_char: false,
                has_wideint: false,
                has_typedef: false,
                has_fixed_width: false,
                has_bool: false,
                has_qualified_types: false,
                has_function_pointer: false,
            };
            g.program();
            for f in &g.funcs {
                assert!(
                    f.ret_max_abs <= VALUE_CAP,
                    "seed {seed}: return bound too large"
                );
            }
        }
    }

    #[test]
    fn emits_supported_bitwise_ops() {
        let corpus = (0..2048u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains(" & "));
        assert!(corpus.contains(" | "));
        assert!(corpus.contains(" ^ "));
        assert!(corpus.contains("~("));
        assert!(corpus.contains(" << "));
        assert!(corpus.contains(" >> "));
    }

    #[test]
    fn emits_typedef_aliases() {
        let corpus = (0..256u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains("typedef int fuzz_int;"));
        assert!(corpus.contains("fuzz_byte tag;"));
    }

    #[test]
    fn emits_fixed_width_typedefs() {
        let corpus = (0..512u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains("#include <stdint.h>"));
        assert!(corpus.contains("#include <stddef.h>"));
        assert!(corpus.contains("int8_t fw_small = "));
        assert!(corpus.contains("uint32_t fw_u32 = 4000000000u;"));
        assert!(corpus.contains("size_t fw_count = "));
    }

    #[test]
    fn emits_non_int_signatures_and_globals() {
        let corpus = (0..512u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains("static char fuzz_char_add(char a, char b)"));
        assert!(corpus.contains("static double fuzz_double_mix(double a, double b)"));
        assert!(corpus.contains("static char fuzz_char_global"));
        assert!(corpus.contains("static double fuzz_double_global"));
    }

    #[test]
    fn emits_non_int_aggregate_fields() {
        let corpus = (0..512u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains("char ch;"));
        assert!(corpus.contains("double weight;"));
        assert!(corpus.contains("s.weight = "));
        assert!(corpus.contains("u.weight = "));
    }

    #[test]
    fn emits_non_int_array_declarations() {
        let corpus = (0..512u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains("char chars[2];"));
        assert!(corpus.contains("double weights[2];"));
        assert!(corpus.contains("chars[1] = chars[0] + 1;"));
        assert!(corpus.contains("weights[1] = weights[0] + 1.25;"));
    }

    #[test]
    fn emits_pointer_uses() {
        let corpus = (0..64u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains("int *ptr = &pointed;"));
        assert!(corpus.contains("*ptr = *ptr + "));
        assert!(corpus.contains("int *walk = pointer_values;"));
        assert!(corpus.contains("*(walk + "));
    }

    #[test]
    fn emits_bool_uses() {
        let corpus = (0..512u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains("#include <stdbool.h>"));
        assert!(corpus.contains("static bool fuzz_bool_from_int(int x)"));
        assert!(corpus.contains("static int fuzz_bool_param(_Bool flag)"));
        assert!(corpus.contains("fuzz_bool_param(2)"));
    }

    #[test]
    fn emits_qualified_type_uses() {
        let corpus = (0..512u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains("volatile double weight;"));
        assert!(corpus.contains("static volatile double fuzz_volatile_global"));
        assert!(corpus.contains("static const int fuzz_const_bias"));
        assert!(corpus.contains("static _Atomic int fuzz_atomic_counter"));
        assert!(corpus.contains("static volatile int fuzz_volatile_return(int value)"));
        assert!(corpus.contains("static double fuzz_volatile_param(volatile double value)"));
        assert!(corpus.contains("static int fuzz_const_param(const int value)"));
        assert!(corpus.contains("static int fuzz_restrict_param(int *restrict value)"));
        assert!(corpus.contains("static int fuzz_atomic_param(_Atomic int value)"));
        assert!(corpus.contains("vf.weight = fuzz_volatile_param(fuzz_volatile_global);"));
        assert!(corpus.contains("fuzz_restrict_param(&restrict_value)"));
    }

    #[test]
    fn emits_function_pointer_uses() {
        let corpus = (0..512u64).map(generate).collect::<Vec<_>>().join("\n");
        assert!(corpus.contains("static int fuzz_fp_add(int lhs, int rhs)"));
        assert!(corpus.contains("static int fuzz_fp_apply(int (*op)(int, int), int lhs, int rhs)"));
        assert!(corpus.contains("int (*fp)(int, int) = fuzz_fp_add;"));
        assert!(corpus.contains("fp(2, 3)"));
        assert!(corpus.contains("fuzz_fp_apply(fp, 4, 5)"));
    }
}
