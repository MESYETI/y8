#[allow(non_snake_case)]

mod lexer;

use std::env;
use std::process;

use crate::lexer::Lexer;

fn main() {
    let args: Vec<_> = env::args().collect();

    let mut inFile:  Option<String> = None;
    let mut outFile: Option<String> = None;

    let mut i = 1;

    while i < args.len() {
    	if args[i].len() == 0 {
    		continue;
    	}

		if args[i].chars().nth(0).unwrap() == '-' {
			match args[i].as_str() {
				"-o" => {
					i += 1;

					if i == args.len() {
						eprintln!("-o flag expects FILE parameter");
						process::exit(1);
					}
				},
				_ => {
					eprintln!("Invalid flag: '{}'", args[i]);
				}
			}
		}
		else {
			inFile = Some(args[i].to_string());
		}

    	i += 1;
    }

    if inFile.is_none() {
    	eprintln!("Assembler requires input file");
    	process::exit(1);
    }

    let mut lexer = Lexer::new(&inFile.unwrap());

    if !lexer.run() {
    	eprintln!("Lexer failed");
    	process::exit(1);
    }

    lexer.print_result();
}
