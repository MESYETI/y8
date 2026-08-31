mod lexer;

use std::env;
use std::process;

fn main() {
    let args: Vec<_> = env::args().collect();

    let inFile:  Option<String> = None;
    let outFile: Option<String> = None;

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
			
		}

    	i += 1;
    }
}
