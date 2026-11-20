#![feature(register_tool)]
#![register_tool(slate)]
#![allow(dead_code)]

#[slate::sensitive]
enum Kind {
    Int,
    Float,
    Ptr(Box<Kind>),
}

enum Plain {
    A,
    B,
}

struct Vector {
    #[slate::sensitive]
    scalable: bool,
    lanes: u32,
}

enum Error {
    Rejected,
}

fn wildcard(k: &Kind) -> u8 {
    match k {
        Kind::Int => 1,
        Kind::Float => 2,
        _ => 0,
    }
}

fn binding(k: &Kind) -> u8 {
    match k {
        Kind::Int => 1,
        Kind::Float => 2,
        other => size(other),
    }
}

fn size(_: &Kind) -> u8 {
    0
}

fn rejecting(k: &Kind) -> Result<u8, Error> {
    match k {
        Kind::Int => Ok(1),
        _ => Err(Error::Rejected),
    }
}

fn returning(k: &Kind) -> Result<u8, Error> {
    match k {
        Kind::Int => Ok(1),
        _ => return Err(Error::Rejected),
    }
}

fn panicking(k: &Kind) -> u8 {
    match k {
        Kind::Int => 1,
        _ => unreachable!(),
    }
}

fn exhaustive(k: &Kind) -> u8 {
    match k {
        Kind::Int => 1,
        Kind::Float => 2,
        Kind::Ptr(_) => 3,
    }
}

fn nested(k: &Kind) -> u8 {
    match k {
        Kind::Ptr(inner) if matches!(**inner, Kind::Int) => 1,
        Kind::Ptr(inner) => size(inner),
        Kind::Int | Kind::Float => 0,
    }
}

fn projection(k: &Kind) -> Option<&Kind> {
    match k {
        Kind::Ptr(inner) => Some(inner),
        _ => None,
    }
}

fn projection_macro(k: &Kind) -> bool {
    matches!(k, Kind::Ptr(_))
}

fn nested_elided(k: &Kind) -> u8 {
    match k {
        Kind::Ptr(inner) => match **inner {
            Kind::Int | Kind::Float => 1,
            _ => 2,
        },
        _ => 0,
    }
}

fn tuple(a: &Kind, b: &Kind) -> u8 {
    match (a, b) {
        (Kind::Int, Kind::Float) => 1,
        (Kind::Float, _) => 2,
        _ => 0,
    }
}

fn macro_form(k: &Kind) -> bool {
    matches!(k, Kind::Int | Kind::Float)
}

fn plain(p: &Plain) -> u8 {
    match p {
        Plain::A => 1,
        _ => 0,
    }
}

fn expected_arm(k: &Kind) -> u8 {
    match k {
        Kind::Int => 1,
        Kind::Float => 2,
        #[expect(slate::sensitive_wildcard, reason = "default is correct for any variant other than Int and Float")]
        _ => 0,
    }
}

fn rest(v: &Vector) -> u32 {
    let Vector { lanes, .. } = v;
    *lanes
}

fn rest_considered(v: &Vector) -> u32 {
    let Vector { lanes, scalable: _ } = v;
    *lanes
}

fn main() {}
