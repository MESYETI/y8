use std::fs;
use crate::error::ErrorSystem;
use crate::error::ErrorInfo;

#[derive(Debug, Clone, Copy)]
pub enum TokenType {
	Label,
	Hashtag,
	Integer,
	Identifier,
	Comma,
	LSquare,
	RSquare,
	LParen,
	RParen,
	String,
	Line
}

#[derive(PartialEq)]
pub enum LexerMode {
	Token,
	String
}

#[derive(Debug)]
pub struct Token {
	tokenType: TokenType,
	contents:  Option<String>,
	error:     ErrorInfo
}

pub struct Lexer<'a> {
	file:             String,
	fileIdx:          usize,
	line:             usize,
	col:              usize,
	reading:          String,
	tokens:           Vec<Token>,
	mode:             LexerMode,
	errorSys: &'a mut ErrorSystem
}

impl Lexer<'_> {
	pub fn new<'a>(file: &'a str, errorSys: &'a mut ErrorSystem) -> Lexer<'a> {
		return Lexer {
			file:     file.to_string(),
			fileIdx:  errorSys.add_file(file),
			line:     0,
			col:      0,
			reading:  String::new(),
			tokens:   Vec::new(),
			mode:     LexerMode::Token,
			errorSys: errorSys
		};
	}

	pub fn get_error(&mut self) -> ErrorInfo {
		return ErrorInfo::new(self.fileIdx, self.line, self.col);
	}

	pub fn add_token(&mut self, tokenType: TokenType, contents: Option<String>) {
		let error = self.get_error();

		self.tokens.push(Token {tokenType: tokenType, contents: contents, error: error});
	}

	pub fn add_reading(&mut self) {
		if self.reading.trim().is_empty() {
			return;
		}

		if self.reading.parse::<u64>().is_ok() {
			self.add_token(TokenType::Integer, Some(self.reading.trim().to_string()));
		}
		else {
			self.add_token(TokenType::Identifier, Some(self.reading.trim().to_string()));
		}

		self.reading = String::new();
	}

	pub fn lex_code(&mut self, code: String) -> bool {
		let mut i = 0;

		while i < code.len() {
			let ch = code.chars().nth(i).unwrap();

			if self.mode == LexerMode::Token {
				let symbolTok = match ch {
					'#' => Some(TokenType::Hashtag),
					',' => Some(TokenType::Comma),
					'[' => Some(TokenType::LSquare),
					']' => Some(TokenType::RSquare),
					'(' => Some(TokenType::LParen),
					')' => Some(TokenType::RParen),
					_   => None
				};

				if symbolTok.is_some() {
					self.add_reading();
					self.add_token(symbolTok.unwrap(), None);

					i += 1;
					continue;
				}

				match ch {
					' ' | '\n' | '\t' => 'label: {
						if self.reading.trim().is_empty() {
							break 'label;
						}

						self.add_reading();

						if ch == '\n' {
							self.add_token(TokenType::Line, None);
						}
					},
					':' => {
						self.add_token(TokenType::Label, Some(self.reading.clone()));

						self.reading = String::new();
					},
					'"' => {
						self.mode = LexerMode::String;
					},
					_ => {
						self.reading.push(ch);
					}
				}
			}
			else if self.mode == LexerMode::String {
				match ch {
					'"' => {
						self.add_token(TokenType::String, Some(self.reading.clone()));

						self.reading = String::new();
					},
					_ => {
						self.reading.push(ch);
					}
				}
			}

			i += 1;
		}

		return true;
	}

	pub fn run(&mut self) -> bool {
		return match fs::read_to_string(&self.file) {
			Ok(code) => self.lex_code(code),
			Err(_)   => false
		}
	}

	pub fn print_result(&mut self) {
		println!("Tokens");
		println!("======");
		for token in self.tokens.iter() {
			println!("{:#?}", token);
		}
	}
}
