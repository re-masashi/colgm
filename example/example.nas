use std::json::{stringify, parse}
use std::out::println
use std::io::*

pub enum example_enum {
    aaa,
    bbb,
    ccc
}

pub struct position {
    pub line: u64,
    pub column: u64
}

pub struct span {
    pub begin: position,
    pub end: position
}

pub struct example {
    type: example_enum,
    content: str,
    location: span
}

impl example {
    pub func new() -> example {
        var begin = position {line: 1, column: 1};
        var end = position {line: 2, column: 2};
        return example {
            type: example_enum::aaa,
            content: "test",
            location: span {begin: begin, end: end}
        };
    }
    pub func get_type(self) -> example_enum {
        return self.type;
    }
    pub func get_content(self) -> str {
        return self.str;
    }
    pub func get_location(self) -> span {
        return self.span;
    }
}

func void_test_function(a: i8, b: i16, c: i32, d: i64) {
    var variable0: i32 = 1;
    var fvar: f32 = 1.0;
    var variable0 = 1;
    var fvar = 1.0;
}

func i32_test_function(a: f32, b: f64) -> i32 {
    var s: str = "12345";
    var s = "";
    var c: i8 = s[0];
    var c = s[0];
    var s: vec<i32> = [];
    var s = [1]; # maybe need at least one literal to get type infer
}

func u32_test_function(c: u8, d: u16, e: u32, f: u64) -> u32 {
    return 0x100000000;
    return 2147483648;
    return 1%5;
}

func f32_test_function(a: example_enum) -> f32 {
    match(a) {
        aaa => return 1.0,
        bbb => return 2.0,
        ccc => return 3.0
    }
    return 1.0/10.0;
}

func main(argc: i32, argv: vec<str>) -> i32 {
    println("hello world");
    io::open("test.txt", "w");
    return 0;
}