use std::path::PathBuf;

fn main() {
    println!("Hello World");
    let num: u16 = 5;
    println!("{}", num);
    let res: u16 = num.rotate_left(1);
    println!("{}", res);
    let mut path_buf = PathBuf::from("instr_timing.gb");
    println!("{}", path_buf.into_os_string().into_string().unwrap());
    //path_buf.set_extension("sav");
    //println!("{}", path_buf.into_os_string().into_string().unwrap());
}
