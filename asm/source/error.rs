use std::process;

#[derive(Debug, Clone)]
pub struct ErrorInfo {
	pub file: usize,
	pub line: usize,
	pub col:  usize
}

impl ErrorInfo {
	pub fn new(file: usize, line: usize, col: usize) -> ErrorInfo {
		return ErrorInfo {file: file, line: line, col: col};
	}
}

pub struct Error {
	pub info: ErrorInfo,
	pub msg:  String
}

pub struct ErrorSystem {
	fileStack: Vec<String>,
	errors:    Vec<Error>
}

impl ErrorSystem {
	pub fn new() -> ErrorSystem {
		return ErrorSystem {fileStack: Vec::new(), errors: Vec::new()};
	}

	pub fn add_file(&mut self, file: &str) -> usize {
		self.fileStack.push(file.to_string());
		return self.fileStack.len() - 1;
	}

	pub fn get(&mut self, idx: usize) -> Option<&str> {
		if idx >= self.fileStack.len() {
			return None
		}
		else {
			return Some(&self.fileStack[idx]);
		}
	}

	pub fn add(&mut self, error: ErrorInfo, msg: &str) {
		self.errors.push(Error {info: error, msg: msg.to_string()});
	}

	pub fn print_error(&self, error: &Error) {
		let fileName = &self.fileStack[error.info.file];

		eprintln!(
			"\x1b[0;30merror:\x1b[0m {}:{}:{}: {}",
			fileName, error.info.line, error.info.col, error.msg
		);
	}

	pub fn print_errors(&self) {
		for error in self.errors.iter() {
			self.print_error(error);
		}
	}

	pub fn crash_if_error(&self) {
		if self.errors.is_empty() {
			return;
		}

		self.print_errors();
		process::exit(1);
	}
}
