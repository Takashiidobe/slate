fn add(a: i32, b: i32) -> i32 {
    let c = a + b;
    c
}

fn main() {
    println!("{}", add(2, 3));
    println!("{}", add(-10, 4));
    println!("{}", add(0, 0));
}
