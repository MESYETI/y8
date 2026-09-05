#[derive(Debug)]
pub struct ErrorInfo {
	file: usize,
	line: usize,
	col:  usize
}

impl ErrorInfo {
	pub fn new(file: usize, line: usize, col: usize) -> ErrorInfo {
		return ErrorInfo {file: file, line: line, col: col};
	}
}

pub struct ErrorSystem {
	fileStack: Vec<String>
}

impl ErrorSystem {
	pub fn new() -> ErrorSystem {
		return ErrorSystem {fileStack: Vec::new()};
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
}
