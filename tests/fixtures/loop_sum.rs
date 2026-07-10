fn sum_to(n: i32) -> i32 {
    let mut total = 0;
    let mut i = 1;
    while i <= n {
        total += i;
        i += 1;
    }
    total
}

fn main() {
    println!("{}", sum_to(10));
    println!("{}", sum_to(1));
    println!("{}", sum_to(0));
}
