use std::fmt;
use std::fmt::Formatter;
use crate::lexer::Token;
use crate::lexer::TokenType;
use crate::ErrorSystem;
use crate::error::ErrorInfo;

#[derive(Debug, Clone)]
pub enum NodeValue {
	Register(String),
	Int(u64),
	String(String),
	Label(String),
	Identifier(String),
	Instruction {name: String, params: Vec<Node>},
	Directive {name: String, params: Vec<Node>}
}

#[derive(Clone)]
pub struct Node {
	error: ErrorInfo,
	value: NodeValue
}

impl fmt::Debug for Node {
	fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), std::fmt::Error> {
		return self.value.fmt(f);
	}
}

impl Node {
	pub fn new(error: ErrorInfo, value: NodeValue) -> Node {
		return Node {error: error, value: value};
	}

	pub fn value(&self) -> &NodeValue {
		return &self.value;
	}

	pub fn error(&self) -> &ErrorInfo {
		return &self.error;
	}

	pub fn type_name(&self) -> &'static str {
		return match self.value {
			NodeValue::Register(_)      => "register",
			NodeValue::Int(_)           => "int",
			NodeValue::String(_)        => "string",
			NodeValue::Label(_)         => "label",
			NodeValue::Identifier(_)    => "identifier",
			NodeValue::Instruction {..} => "instruction",
			NodeValue::Directive {..}   => "directive"
		};
	}
}

pub struct Parser<'a> {
	pub nodes: Vec<Node>,

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

	fn new_node(&self, value: NodeValue, error: Option<ErrorInfo>) -> Node {
		return if error.is_some() {
			Node::new(error.unwrap(), value)
		}
		else {
			Node::new(self.get_error(), value)
		}
	}

	fn add_error(&mut self, msg: &str) {
		let error = self.get_error();

		self.errorSys.add(error, msg);
	}

	fn next(&mut self) -> Option<()> {
		self.i += 1;

		if self.i >= self.tokens.len() {
			self.i = self.tokens.len() - 1;
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

		match tok.tokenType {
			TokenType::Integer => {
				return Some(self.new_node(NodeValue::Int(
					tok.contents.as_ref().unwrap().parse::<u64>().unwrap()
				), None));
			},
			TokenType::Identifier => {
				let contents = tok.contents.as_ref().unwrap();

				return match contents.as_str() {
					"a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" |
					"ab" | "cd" | "ef" | "sp" => {
						return Some(self.new_node(
							NodeValue::Register(contents.to_string()), None
						));
					},
					_ => Some(self.new_node(NodeValue::Identifier(contents.to_string()), None))
				};
			},
			TokenType::String => {
				return Some(self.new_node(NodeValue::String(
					tok.contents.as_ref().unwrap().clone()
				), None));
			},
			_ => {
				self.add_error(&format!("Unexpected '{:#?}' token", tok.tokenType));
				return None;
			}
		};
	}

	fn parse_instruction_directive(&mut self, directive: bool) -> Option<Node> {
		let     name   = self.tokens[self.i].contents.as_ref().unwrap();
		let mut params = Vec::new();
		let     error  = self.get_error();

		self.next()?;

		while self.tokens[self.i].tokenType != TokenType::Line {
			params.push(self.parse_parameter()?);

			self.next()?;

			if self.tokens[self.i].tokenType != TokenType::Line {
				self.expect(TokenType::Comma)?;

				self.next()?;
			}
		}

		if directive {
			return Some(self.new_node(NodeValue::Directive {
				name: name.to_string(), params: params.to_owned()
			}, Some(error)));
		}
		else {
			return Some(self.new_node(NodeValue::Instruction {
				name: name.to_string(), params: params.to_owned()
			}, Some(error)));
		}
	}

	fn parse_label(&mut self) -> Option<Node> {
		let tok = &self.tokens[self.i];

		return Some(self.new_node(NodeValue::Label(tok.contents.as_ref().unwrap().clone()), None));
	}

	fn parse_node(&mut self) -> Option<Node> {
		let tok = &self.tokens[self.i];

		return match tok.tokenType {
			TokenType::Label      => self.parse_label(),
			TokenType::Identifier => self.parse_instruction_directive(false),
			TokenType::Hashtag => {
				self.next()?;
				self.expect(TokenType::Identifier)?;

				return self.parse_instruction_directive(true);
			}
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
