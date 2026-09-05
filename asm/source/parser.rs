struct InstructionNode {
	name: String,
	params: Vec<Box<Node>>
}

enum Node {
	Register(String),
	Int(u64),
	String(String),
	Label(String),
	Instruction(InstructionNode)
}

struct Parser {
	nodes: Vec<Node>,
	i:     usize
}
