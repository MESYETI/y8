use crate::lexer::Token;
use crate::lexer::TokenType;
use crate::ErrorSystem;
use crate::error::ErrorInfo;

#[derive(Debug, Clone)]
pub struct InstructionNode {
	name: String,
	params: Vec<Node>
}

#[derive(Debug, Clone)]
pub enum Node {
	Register(String),
	Int(u64),
	String(String),
	Label(String),
	Identifier(String),
	Instruction(InstructionNode)
}

pub struct Parser<'a> {
	nodes:            Vec<Node>,
	i:                usize,
	tokens:   &'a     Vec<Token>,
	errorSys: &'a mut ErrorSystem
}

impl Parser<'_> {
	pub fn new<'a>(tokens: &'a Vec<Token>, errorSys: &'a mut ErrorSystem) -> Parser<'a> {
		return Parser {
			nodes: Vec::new(),
			i: 0,
			tokens: tokens,
			errorSys: errorSys
		};
	}

	fn get_error(&self) -> ErrorInfo {
		return self.tokens[self.i].error.clone();
	}

	fn add_error(&mut self, msg: &str) {
		let error = self.get_error();

		self.errorSys.add(error, msg);
	}

	fn next(&mut self) -> Option<()> {
		self.i += 1;

		if self.i >= self.tokens.len() {
			self.add_error("Unexpected EOF");
			return None;
		}

		return Some(());
	}

	fn expect(&mut self, tokenType: TokenType) -> Option<()> {
		if self.tokens[self.i].tokenType != tokenType {
			self.add_error(&format!(
				"Expected {:?}, got {:?}", tokenType, self.tokens[self.i].tokenType
			));
			return None
		}

		return Some(());
	}

	fn parse_parameter(&mut self) -> Option<Node> {
		let tok = &self.tokens[self.i];

		return match tok.tokenType {
			TokenType::Integer => {
				return Some(Node::Int(tok.contents.as_ref().unwrap().parse::<u64>().unwrap()));
			},
			TokenType::Identifier => {
				let contents = tok.contents.as_ref().unwrap();

				return match contents.as_str() {
					"a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" |
					"ab" | "cd" | "ef" | "sp" => {
						return Some(Node::Register(contents.to_string()))
					},
					_ => Some(Node::Identifier(contents.to_string()))
				}
			},
			TokenType::String => Some(Node::String(tok.contents.as_ref().unwrap().clone())),
			_ => {
				self.add_error(&format!("Unexpected '{:#?}' token", tok.tokenType));
				return None;
			}
		}
	}

	fn parse_instruction(&mut self) -> Option<Node> {
		let     name   = self.tokens[self.i].contents.as_ref().unwrap();
		let mut params = Vec::new();

		self.next()?;

		while self.tokens[self.i].tokenType != TokenType::Line {
			params.push(self.parse_parameter()?);

			self.next()?;

			if self.tokens[self.i].tokenType != TokenType::Line {
				self.expect(TokenType::Comma)?;

				self.next()?;
			}
		}

		return Some(Node::Instruction(InstructionNode {
			name: name.to_string(), params: params.to_owned()
		}));
	}

	fn parse_label(&mut self) -> Option<Node> {
		let tok = &self.tokens[self.i];

		return Some(Node::Label(tok.contents.as_ref().unwrap().clone()));
	}

	fn parse_node(&mut self) -> Option<Node> {
		let tok = &self.tokens[self.i];

		return match tok.tokenType {
			TokenType::Label      => self.parse_label(),
			TokenType::Identifier => self.parse_instruction(),
			_ => {
				self.add_error(&format!("Unexpected '{:#?}' token", tok.tokenType));
				return None;
			}
		}
	}

	pub fn parse(&mut self) -> Option<()> {
		while self.i < self.tokens.len() {
			let node = self.parse_node()?;

			self.nodes.push(node);
			self.i += 1;
		}

		return Some(());
	}

	pub fn print_ast(&self) {
		for node in self.nodes.iter() {
			println!("{:#?}", node);
		}
	}
}
