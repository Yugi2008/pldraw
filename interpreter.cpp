#include "interpreter.hpp"

// Constructor

Interpreter::Interpreter() {
     //env.reset();  // Initialize environment
}

// Parse Function
bool Interpreter::parse(std::istream& expression) noexcept {
    try {
        std::string input;
        std::getline(expression, input);
        std::istringstream input_stream(input);


        TokenSequenceType tokens = tokenize(input_stream); // tokenize input
        std::vector<std::string> tokens_vector(tokens.begin(), tokens.end());

        // check for empty input
        if (tokens.empty()) {
            throw InterpreterSemanticError("Error: Empty input");
        }

        size_t index = 0;
        ast = buildAST(tokens_vector, index);  // build AST from tokens


        if (index != tokens.size()) { // make sure there are no more tokens
            throw InterpreterSemanticError("Error: Extra tokens after input");
        }


    }
    catch (const InterpreterSemanticError& err) {
        std::cout << err.what() << std::endl;  // output coresponding error message
        return  false;
    }
    catch (const std::exception& err) {
        std::cout << "Unexpected error during parsing: " << err.what() << std::endl;
        return false;
    }

    return  true;
}

Expression Interpreter::buildAST(const std::vector<std::string>& tokens, size_t& index) {
    if (index >= tokens.size()) {
        throw InterpreterSemanticError("Error: Unexpected end of input");
    }

    if (tokens[index] == "(") { // checking for open paren
        paren = true;
        ++index;  // skip the opening parenthesis in index

        if (index >= tokens.size() || tokens[index] == ")") {        // check if expression is empty
            throw InterpreterSemanticError("Error: Empty or invalid expression");
        }

        std::vector<Expression> subExpressions;
        while (index < tokens.size() && tokens[index] != ")") { // get all sub-expressions until we until )
            subExpressions.push_back(buildAST(tokens, index));  // recursive call to handle inside expressions
        }

        if (index >= tokens.size() || tokens[index] != ")") {
            throw InterpreterSemanticError("Error: Mismatched parentheses");
        }

        ++index;  // skip the close parenthesis in index

        if (subExpressions.empty()) {
            throw InterpreterSemanticError("Error: No operands or operator in expression");
        }

        // The first expression in the list is the operator (head)
        Expression exp;
        exp.head = subExpressions.back().head;  // assign operator to the head
        subExpressions.erase(subExpressions.end() - 1);  // Remove the operator

        // rest are operands (tail)
        exp.tail = subExpressions;

        // Check if the head is a valid keyword
        if (exp.head.type == SymbolType && !env.isKeyword(exp.head.value.sym_value)) {
            throw InterpreterSemanticError("Error: Unrecognized keyword '" + exp.head.value.sym_value + "'");
        }

        return exp;
    }

    Expression exp;
    Atom atom;
    if (token_to_atom(tokens[index], atom)) {
        ++index;

        if (atom.type == NumberType || atom.type == BooleanType) {
            return Expression(atom);
        }
        if (atom.type == SymbolType) {
            if (!paren) {
                throw InterpreterSemanticError("Error: Invalid symbol '" + atom.value.sym_value + "'");
            }

            return Expression(atom); // Return the symbol expression
        }

        throw InterpreterSemanticError("Error: Unknown atom type");

    }
    throw InterpreterSemanticError("Error: Invalid token '" + tokens[index] + "'");


}

// Evaluate Function
Expression Interpreter::eval() {
    return evalExpression(ast);
}


Expression Interpreter::evalExpression(const Expression& exp) {
    if (exp.tail.empty()) {

        if (exp.head.type == SymbolType) {// check if number, boolean, or symbol      
            return env.get(exp.head.value.sym_value); // Look up the symbol in the env
        }


        return exp; // Returning the expression as is (number, boolean)

    }

    // last operand should be the head
    if (exp.head.type != SymbolType) {
        throw InterpreterSemanticError("Error: Operator must be a symbol");
    }

    std::string op = exp.head.value.sym_value;

    if (op == "define") { // define special form

        if (exp.tail.size() != 2) {
            throw InterpreterSemanticError("Error: 'define' expects exactly two arguments");
        }

        Expression symbolExp = exp.tail[0].head.value.sym_value;
        Expression valueExp = evalExpression(exp.tail[1]);
        if (symbolExp.head.type != SymbolType) {// should be a symbol
            throw InterpreterSemanticError("Error: 'define' requires a symbol as the first argument");
        }

        std::string symbol = symbolExp.head.value.sym_value;
        if (env.isKeyword(symbol)) {
            throw InterpreterSemanticError("Error: Invalid symbol, symbol is a keyword");
        }

        if (exp.tail[1].head.type == SymbolType && exp.tail[1].tail.empty() && !(env.isDefined(exp.tail[1].head.value.sym_value))) {// check for (define x 10) error
            throw InterpreterSemanticError("Error: Incorrect usage of 'define'. Correct syntax is '(symbol value define)'");
        }


        Expression valueResult = evalExpression(valueExp);// evaluate second expression

        env.define(symbol, valueResult);// add map to the env

        return valueExp;
    }
    if (op == "begin") {// begin special form

        if (exp.tail.empty()) {
            throw InterpreterSemanticError("Error: 'begin' expects at least one argument");
        }

        Expression result;
        for (const auto& expr : exp.tail) {
            result = evalExpression(expr);
        }
        return result;  // return result from last expression
    }
    if (op == "if") {// if special form

        if (exp.tail.size() != 3) {
            throw InterpreterSemanticError("Error: 'if' expects exactly three arguments");
        }

        Expression condition = evalExpression(exp.tail[0]);// eval first expression (condition)

        if (condition.head.type != BooleanType) {// check if condition is a boolean
            throw InterpreterSemanticError("Error: 'if' condition must be a boolean");
        }

        if (condition.head.value.bool_value) {// eval second expression
            return evalExpression(exp.tail[1]);

        }
        // eval third expression
        return evalExpression(exp.tail[2]);


    }

    if (env.isProcedure(op)) {// check if the operator is a recognized procedure

        auto procResult = env.getResult(op); // get procedure from the environment

        std::vector<Expression> evaluated_args;

        for (const auto& operand : exp.tail) {
            evaluated_args.push_back(evalExpression(operand));
        }
        // convert evaluated expressions to Atoms for the procedure call
        std::vector<Atom> argAtoms;
        for (const auto& arg : evaluated_args) {
            argAtoms.push_back(arg.head);  // use head of each evaluated expression
        }

        return procResult.proc(env, argAtoms); // call procedure with the arguments
    }

    throw InterpreterSemanticError("Error: Unknown procedure '" + op + "'");


}

// parse and eval the input string (for pldraw)
Expression Interpreter::parseAndEvaluate(const std::string &input) {
    std::istringstream stringS(input);
    if (!parse(stringS)) {
        throw InterpreterSemanticError("Parsing failed");
    }
    return eval();
}