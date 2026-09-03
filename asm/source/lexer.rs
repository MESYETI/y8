use std::fs;
use std::collections::HashMap;

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
	contents:  Option<String>
}

pub struct Lexer {
	file:    String,
	line:    u32,
	col:     u32,
	reading: String,
	tokens:  Vec<Token>,
	mode:    LexerMode
}

impl Lexer {
	pub fn new(file: &str) -> Lexer {
		return Lexer {
			file:    file.to_string(),
			line:    0,
			col:     0,
			reading: String::new(),
			tokens:  Vec::new(),
			mode:    LexerMode::Token
		};
	}

	pub fn add_token(&mut self, token: Token) {
		self.tokens.push(token);
	}

	pub fn add_reading(&mut self) {
		if self.reading.parse::<u64>().is_ok() {
			self.add_token(Token {
				tokenType: TokenType::Integer, contents: Some(self.reading.trim().to_string())
			});
		}
		else {
			self.add_token(Token {
				tokenType: TokenType::Identifier, contents: Some(self.reading.trim().to_string())
			});
		}

		self.reading = String::new();
	}

	pub fn lex_code(&mut self, code: String) -> bool {
		let mut i = 0;

		let mut symbolTokens = HashMap::from([
			('#', TokenType::Hashtag),
			(',', TokenType::Comma),
			('[', TokenType::LSquare),
			(']', TokenType::RSquare),
			('(', TokenType::LParen),
			(')', TokenType::RParen)
		]);

		while i < code.len() {
			let ch = code.chars().nth(i).unwrap();

			if symbolTokens.contains_key(&ch) {
				self.add_token(Token {
					tokenType: *symbolTokens.get(&ch).unwrap(), contents: None
				});
				continue;
			}

			if self.mode == LexerMode::Token {
				match ch {
					' ' | '\n' | '\t' => 'label: {
						if self.reading.trim().is_empty() {
							break 'label;
						}

						self.add_reading();

						if ch == '\n' {
							self.add_token(Token {tokenType: TokenType::Line, contents: None});
						}
					},
					':' => {
						self.add_token(Token {
							tokenType: TokenType::Label, contents: Some(self.reading.clone())
						});

						self.reading = String::new();
					},
					'#' => {
						// TODO: check if reading is empty or not
						self.add_token(Token {tokenType: TokenType::Hashtag, contents: None});
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
						self.add_token(Token {
							tokenType: TokenType::String, contents: Some(self.reading.clone())
						});

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
