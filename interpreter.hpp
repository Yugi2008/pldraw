#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// system includes
// TODO: Include C++ standard library headers as needed
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

// module includes
#include "expression.hpp"
#include "environment.hpp"
// TODO: Include firther custom header files if need
#include "tokenizer.hpp"
#include "interpreter_semantic_error.hpp"

// Interpreter has
// Environment, which starts at a default
// parse method, builds an internal AST
// eval method, updates Environment, returns last result
class Interpreter {
public:
	Interpreter();

	bool parse(std::istream& expression) noexcept;
	Expression eval();

	Expression parseAndEvaluate(const std::string& input);

private:

	Environment env;
	Expression ast;
	std::vector<std::string> tokens;
	Expression buildAST(const std::vector<std::string>& tokens, size_t& index);
	Expression evalExpression(const Expression& exp);
	bool paren = false;

};


#endif