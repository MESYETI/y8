#![allow(non_snake_case)]

mod lexer;
mod error;
mod parser;

use std::env;
use std::process;

use crate::lexer::Lexer;
use crate::error::ErrorSystem;
use crate::parser::Parser;

fn main() {
	let args: Vec<_> = env::args().collect();

	let mut inFile:  Option<String> = None;
	let mut outFile: Option<String> = None;

	// flags
	let mut printTok = false;
	let mut printAST = false;

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

					outFile = Some(args[i].to_string());
				},
				"-ld" => {
					printTok = true;
				},
				"-pd" => {
					printAST = true;
				},
				_ => {
					eprintln!("Invalid flag: '{}'", args[i]);
					process::exit(1);
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

	let mut errorSys = ErrorSystem::new();

	let mut tokens = Vec::new();

	{
		let binding = inFile.unwrap();
		let mut lexer = Lexer::new(&binding, &mut errorSys);

		if !lexer.run() {
			errorSys.crash_if_error();

			eprintln!("Lexer failed");
			process::exit(1);
		}

		if printTok {
			lexer.print_result();
			process::exit(0);
		}

		tokens = lexer.get_tokens().to_owned();
	}

	{
		let mut parser = Parser::new(&tokens, &mut errorSys);

		if parser.parse().is_none() {
			errorSys.crash_if_error();

			eprintln!("Parser failed");
			process::exit(1);
		}

		if printAST {
			parser.print_ast();
			process::exit(0);
		}
	}
}
