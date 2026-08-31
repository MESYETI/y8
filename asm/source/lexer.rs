use std::fs;

pub enum TokenType {
	Label,
	Hashtag,
	Integer,
	Identifier,
	Comma,
	LSquare,
	RSQuare,
	String,
	Line
}

pub struct Token {
	tokenType: TokenType,
	contents:  String
}

pub struct Lexer {
	file:    String,
	line:    u32,
	col:     u32,
	reading: String,
	tokens:  Vec<Token>
}

impl Lexer {
	pub fn new(file: String) -> Lexer {
		return Lexer {file, 0, 0, String::new(), Vec::new()};
	}

	pub fn add_token(&mut self, token: Token) {
		self.tokens.push(token);
	}

	pub fn lex_code(&mut self, code: String) -> Option<Vec<Token>> {
		let mut i = 0;

		while i < code.len() {
			i += 1;
		}

		return Some(self.tokens);
	}

	pub fn run(&mut self) -> Option<Vec<Token>> {
		return match fs::read_to_string(self.file.clone()) {
			Ok(code) => self.lex_code(code.clone()),
			Err      => None
		}
	}
}
