#![feature(register_tool)]
#![register_tool(slate)]

#[slate::sensitive(exhaustive)]
enum Abi {
    Register,
    Stack,
}

fn register_slots(abi: Abi) -> u32 {
    match abi {
        Abi::Register => 1,
        _ => 0,
    }
}

fn default_slots(abi: Abi) -> u32 {
    match abi {
        _ => 0,
    }
}

fn register_test(abi: Abi) -> bool {
    matches!(abi, Abi::Register)
}

fn nested(abi: Option<Abi>) -> u32 {
    match abi {
        Some(Abi::Register) => 1,
        _ => 0,
    }
}

fn let_else(abi: Abi) -> u32 {
    let Abi::Register = abi else { return 0 };
    1
}

fn let_else_rejects(abi: Abi) -> Result<u32, ()> {
    let Abi::Register = abi else {
        return Err(());
    };
    Ok(1)
}

fn let_else_covers_both(pair: (Abi, Option<u32>)) -> u32 {
    let (Abi::Register | Abi::Stack, Some(value)) = pair else {
        return 0;
    };
    value
}

fn main() {}
