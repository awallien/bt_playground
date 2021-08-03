///
/// file: main.rs
/// 
/// description: main program for sumdoku solver
/// 
/// author: awallien
/// 
/// date: 8/2/2021
/// 

use args::Args;
use getopts::Occur;
use std::{env,process::exit};

mod board;
mod bt;

fn main() {
    let args: Vec<String> = env::args().collect();
    let program: String = args[0].clone();
    let mut config_file_str: String = String::from("");

    let mut optargs = Args::new(program.as_str(), "Sumdoku Solver");
    optargs.option("f",
        "config-file",
        "configuration file containing sumdoku board",
        "FILE",
        Occur::Req,
        None);
    optargs.flag("h", "help", "output this menu");

    match optargs.parse(args) {
        Ok(_) => {}
        Err(_) => {
            eprintln!("{}",optargs.full_usage());
            exit(0);
        }
    }

    match optargs.value_of("config-file") {
        Ok(file) => {config_file_str = file;}
        Err(_) => {}
    }

    if !config_file_str.is_empty() {
        let mut board = board::SumdokuBoard::new(config_file_str);
        println!("{}olution found!", 
            if bt::solve(&mut board) {"S"} else {"No S"} );
    }
    
    
}
