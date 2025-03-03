#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include <string>
#include <sstream>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "tokenizer.hpp"


// This is example unit test case with Catch 2
// You are free to uncomment and use this test case. 
TEST_CASE("evaluating add", "[interpreter]") {
    std::string program = "(1 2 +)";
    std::istringstream iss(program);

    Interpreter interpreter;

    REQUIRE(interpreter.parse(iss));
    auto result = interpreter.eval();
    REQUIRE(result == Expression(3.));
}

// TODO: Add further unit test cases here to fully cover your code.
static Expression run(const std::string& program) {
    std::istringstream iss(program);

    Interpreter interpreter;

    bool ok = interpreter.parse(iss);
    if (!ok) {
        std::cerr << "[TEST] Failed to parse: " << program << std::endl;
    }
    REQUIRE(ok == true);

    Expression result;
    REQUIRE_NOTHROW(result = interpreter.eval());

    return result;
}

// Helper function to run a program from a file through the interpreter
static Expression runfile(const std::string& fname) {
    std::ifstream ifs(fname);
    REQUIRE(ifs.good() == true);

    Interpreter interpreter;

    bool ok = interpreter.parse(ifs);
    if (!ok) {
        std::cerr << "[TEST] Failed to parse file: " << fname << std::endl;
    }
    REQUIRE(ok == true);

    Expression result;
    REQUIRE_NOTHROW(result = interpreter.eval());

    return result;
}

// Testing interpreter
TEST_CASE("Test Interpreter with valid input", "[interpreter]") {
    std::vector<std::string> programs = {
        "(1 2 +)",
       "((x 10 define) (x 2 *) begin)",

    };

    std::vector<Expression> expected_results = {
        Expression(3.0),
        Expression(20.0),
    };

    for (size_t i = 0; i < programs.size(); ++i) {
        std::istringstream iss(programs[i]);
        Interpreter interpreter;
        REQUIRE(interpreter.parse(iss));
        Expression result;
        REQUIRE_NOTHROW(result = interpreter.eval());
        REQUIRE(result == expected_results[i]);
    }
}

// Test parsing invalid expressions
TEST_CASE("Test Interpreter parser with invalid input", "[interpreter]") {
    std::vector<std::string> programs = {
        "(1 2 +",                       // Missing closing parenthesis
        "((x 10 define) (x 2 *) begin", // Missing closing parenthesis
        "(1 2 +) extra",                // Extra input after valid expression
        "()",                           // Empty expression
        "((1 2 +) (3 4 *)",             // Missing closing parenthesis
        "(1 2 unknown_op)"              // Unknown operator
        "(1 2 3 -)",                        // Too many operands for binary operator


    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interpreter;
        bool ok = interpreter.parse(iss);
        REQUIRE_FALSE(ok);
    }
}
// Test evaluation with semantic errors
TEST_CASE("Test define", "[interpreter]") {
    std::string program = "(a 1 define)";
    std::istringstream iss(program);

    Interpreter interpreter;

    REQUIRE(interpreter.parse(iss));
    auto result = interpreter.eval();
    REQUIRE(result == Expression(1.));
}

// Test evaluation with semantic errors
TEST_CASE("Test Interpreter evaluation with semantic errors", "[interpreter]") {
    std::vector<std::string> programs = {
        "(1 0 /)",                       // Division by zero
        "((x 10 define) (x x x + +) begin)", // Too many operands for binary operator
        "((x 10 define) (x y +) begin)", // Undefined symbol 'y'
        // "((x 10 define) (x 2 +) (x 3 define) begin)", // Redefinition of 'x'
         "((x 10 define) (x True +) begin)", // Invalid operand type
         "((x 10 define) (x 0 /) begin)",    // Division by zero in subexpression
         "((pi 3.14 define) (pi))",          // Redefinition of built-in symbol 'pi'
         "((if True 1) begin)",               // 'if' with incorrect number of arguments
          "(define x 10)",                     // Incorrect syntax for define
          "(1 2)",                        // Missing operator this

    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interpreter;
        REQUIRE(interpreter.parse(iss));
        REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError);
    }
}

// Test parsing numerical literals
TEST_CASE("Test Interpreter parser with numerical literals", "[interpreter]") {
    std::vector<std::string> programs = {
        "(1)",
        "(+1)",
        "(+1e+0)",
        "(1e-0)",
        "(-3.14)",
        "(6.022e23)",
        "(-1.602e-19)"
    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interpreter;
        bool ok = interpreter.parse(iss);
        REQUIRE(ok == true);
        Expression result;
        REQUIRE_NOTHROW(result = interpreter.eval());
    }
}

// Test handling of empty and invalid inputs
TEST_CASE("Test Interpreter parser with empty and invalid inputs", "[interpreter]") {
    std::vector<std::string> programs = {
        "",           // Empty string
        " ",          // Whitespace
        "\n",         // Newline
        "(invalid)",  // Invalid symbol
        "(1 2 ?)"     // Invalid operator '?'
    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interpreter;
        bool ok = interpreter.parse(iss);
        if (program == "()" || program == "(   )") {
            // Empty expressions might be considered syntactically valid
            REQUIRE(ok == true);
            REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError);
        }
        else {
            REQUIRE_FALSE(ok);
        }
    }
}

// Test parsing and evaluating expressions from files
TEST_CASE("Test Interpreter with input from files", "[interpreter]") {
    // Create temporary files for testing
    std::string valid_filename = "test_valid.slp";
    std::string invalid_filename = "test_invalid.slp";

    // Valid program
    std::ofstream valid_file(valid_filename);
    valid_file << "((r 10 define) ((r r *) pi *) begin)";
    valid_file.close();

    // Invalid program (missing closing parenthesis)
    std::ofstream invalid_file(invalid_filename);
    invalid_file << "((r 10 define) ((r r *) pi *) begin";
    invalid_file.close();

    // Test valid file
    {
        Interpreter interpreter;
        std::ifstream ifs(valid_filename);
        REQUIRE(ifs.good() == true);
        bool ok = interpreter.parse(ifs);
        REQUIRE(ok == true);
        Expression result;
        REQUIRE_NOTHROW(result = interpreter.eval());
        REQUIRE(result == Expression(314.1592653589793)); // Assuming pi ≈ 3.141592653589793
    }

    // Test invalid file
    {
        Interpreter interpreter;
        std::ifstream ifs(invalid_filename);
        REQUIRE(ifs.good() == true);
        bool ok = interpreter.parse(ifs);
        REQUIRE(ok == false);
    }

    // Clean up temporary files
    remove(valid_filename.c_str());
    remove(invalid_filename.c_str());
}

// Test parser with extra input after a valid expression
TEST_CASE("Test Interpreter parser with extra input", "[interpreter]") {
    std::string program = "((x 10 define) (x x *) extra)";
    std::istringstream iss(program);

    Interpreter interpreter;

    bool ok = interpreter.parse(iss);

    REQUIRE(ok == false);
}

// Test parser with truncated input
TEST_CASE("Test Interpreter parser with truncated input", "[interpreter]") {
    {
        std::string program = "(1 2 +";
        std::istringstream iss(program);

        Interpreter interpreter;
        bool ok = interpreter.parse(iss);
        REQUIRE(ok == false);
    }
    {
        std::string program = "((x 10 define) (x x *) begin";
        std::istringstream iss(program);

        Interpreter interpreter;
        bool ok = interpreter.parse(iss);
        REQUIRE(ok == false);
    }
}

// Test evaluating expressions involving booleans and comparisons
TEST_CASE("Test Interpreter with boolean operations", "[interpreter]") {
    std::vector<std::string> programs = {
        "(True False and)",
        "(True False or)",
        "(True not)",
        "(1 2 <)",
        "(2 2 ==)",
        "(3 2 >)"
    };

    std::vector<Expression> expected_results = {
        Expression(false),
        Expression(true),
        Expression(false),
        Expression(true),
        Expression(true),
        Expression(true)
    };

    for (size_t i = 0; i < programs.size(); ++i) {
        std::istringstream iss(programs[i]);
        Interpreter interpreter;
        REQUIRE(interpreter.parse(iss));
        Expression result;
        REQUIRE_NOTHROW(result = interpreter.eval());
        REQUIRE(result == expected_results[i]);
    }
}

// Add these test cases to your existing test suite

// Test parsing invalid symbols and numbers
TEST_CASE("Test Interpreter parser with invalid symbols and numbers", "[interpreter]") {
    std::vector<std::string> programs = {
        "(1abc)",          // Invalid number format
        "(#)",             // Invalid symbol
        "(123abc)",        // Invalid number with letters
        "(abc$)",          // Invalid symbol with special character
        "(3.14.15)",       // Invalid number with multiple decimals
        "(@)",             // Invalid symbol '@'
    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interp;
        bool ok = interp.parse(iss);
        REQUIRE(ok == false);
    }
}

// Test parsing with missing or extra parentheses
TEST_CASE("Test Interpreter parser with missing or extra parentheses", "[interpreter]") {
    std::vector<std::string> programs = {
        "(1 2 +",          // Missing closing parenthesis
        "1 2 +)",          // Missing opening parenthesis
        "((1 2 +)",        // Missing closing parenthesis
        "(1 2 +))",        // Extra closing parenthesis
        "((1 2 +)) extra", // Extra input after closing parenthesis
    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interp;
        bool ok = interp.parse(iss);
        REQUIRE(ok == false);
    }
}

// Test evaluating expressions with undefined symbols
TEST_CASE("Test Interpreter evaluation with undefined symbols", "[interpreter]") {
    std::vector<std::string> programs = {
        "(1 x +)",             // 'x' is undefined  
        "((x 10 define) (x y + ) begin)", // 'y' is undefined
    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interp;
        REQUIRE(interp.parse(iss));
        REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
    }
}

// Test evaluating expressions with invalid operand types
TEST_CASE("Test Interpreter evaluation with invalid operand types", "[interpreter]") {
    std::vector<std::string> programs = {
        "(True 1 +)",         // Adding boolean and number
        "(1 False *)",        // Multiplying number and boolean
        "(1 0 not)",          // Applying 'not' to number
        "(True False +)",     // Adding booleans
        "(\"string\" 1 +)",   // Adding string and number (if strings are supported)
    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interp;
        REQUIRE(interp.parse(iss));
        REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
    }
}

// Test evaluating nested expressions
TEST_CASE("Test Interpreter evaluation with nested expressions", "[interpreter]") {
    {
        std::string program = "((1 2 +) (3 4 +) *)";
        Expression result = run(program);
        REQUIRE(result == Expression(21.));
    }
    {
        std::string program = "(((1 2 +) (3 4 +) *) (5 6 +) +)";
        Expression result = run(program);
        REQUIRE(result == Expression(32.));
    }
    {
        std::string program = "(((1 2 +) (3 4 +) *) ((5 6 +) 2 /) -)";
        Expression result = run(program);
        REQUIRE(result == Expression(15.5));
    }
}

// Test evaluating expressions with the 'if' special form
TEST_CASE("Test Interpreter special form: if with complex conditions", "[interpreter]") {
    {
        std::string program = "((1 2 <) (10) (20) if)";
        Expression result = run(program);
        REQUIRE(result == Expression(10.));
    }
    {
        std::string program = "((1 2 >) (10) (20) if)";
        Expression result = run(program);
        REQUIRE(result == Expression(20.));
    }
}

// Test defining and using variables
TEST_CASE("Test Interpreter define and use variables", "[interpreter]") {
    {
        std::string program = "((x 10 define) (y 20 define) (x y +))";
        Expression result = run(program);
        REQUIRE(result == Expression(30.));
    }

}

// Test redefinition errors
TEST_CASE("Test Interpreter handling of redefinition errors", "[interpreter]") {
    std::vector<std::string> programs = {
        "((x 10 define) (x 20 define))",   // Redefinition of 'x'
        "((pi 3.14 define))",              // Redefinition of 'pi'
        "((True False define))",           // Redefinition of 'True'
    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interp;
        REQUIRE(interp.parse(iss));
        REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
    }
}

// Test logical operations with more operands
TEST_CASE("Test Interpreter logical operations with multiple operands", "[interpreter]") {
    {
        std::string program = "(True True True and)";
        Expression result = run(program);
        REQUIRE(result == Expression(true));
    }
    {
        std::string program = "(True False True and)";
        Expression result = run(program);
        REQUIRE(result == Expression(false));
    }
    {
        std::string program = "(False False False or)";
        Expression result = run(program);
        REQUIRE(result == Expression(false));
    }
    {
        std::string program = "(False True False or)";
        Expression result = run(program);
        REQUIRE(result == Expression(true));
    }
}

// Test mathematical functions with invalid arguments
TEST_CASE("Test Interpreter math functions with invalid arguments", "[interpreter]") {
    std::vector<std::string> programs = {
        "(0 log2)",         // log2(0) is undefined
        "(-1 sqrt)",        // sqrt of negative number
        "(True sqrt)",      // sqrt of boolean
        "(\"string\" log2)" // log2 of a string (if strings are supported)
    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interp;
        REQUIRE(interp.parse(iss));
        REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
    }
}

// Test complex mathematical expressions
TEST_CASE("Test Interpreter with complex mathematical expressions", "[interpreter]") {
    {
        std::string program = "(((25 sqrt) 5 *) 100 +)";
        Expression result = run(program);
        REQUIRE(result == Expression(125.));
    }

}

// Test handling of large numbers and floating-point precision
TEST_CASE("Test Interpreter with large numbers and precision", "[interpreter]") {
    {
        std::string program = "(1e10 1e10 +)";
        Expression result = run(program);
        REQUIRE(result == Expression(2e10));
    }
    {
        std::string program = "(1e-10 1e-10 +)";
        Expression result = run(program);
        REQUIRE(result == Expression(2e-10));
    }
    {
        std::string program = "(3.1415926535 2 *)";
        Expression result = run(program);
        REQUIRE(result == Expression(6.283185307));
    }
}

// Test using 'begin' with multiple expressions
TEST_CASE("Test Interpreter 'begin' special form with multiple expressions", "[interpreter]") {
    {
        std::string program = "((x 10 define) (y 20 define) (x y +) begin)";
        Expression result = run(program);
        REQUIRE(result == Expression(30.));
    }

}

// Test error when 'begin' is used incorrectly
TEST_CASE("Test Interpreter 'begin' used incorrectly", "[interpreter]") {
    std::vector<std::string> programs = {
        "(begin)",                    // 'begin' with no arguments
        "(1 2 + begin)",              // 'begin' at the end without expressions
        "((x 10 define) begin begin)" // Multiple 'begin's without expressions
    };

    for (auto program : programs) {
        std::istringstream iss(program);
        Interpreter interp;
        REQUIRE(interp.parse(iss));
        REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
    }
}

// expression tests
TEST_CASE("Test Type Inference", "[types]") {
    Atom a;

    std::string token = "True";
    REQUIRE(token_to_atom(token, a));
    REQUIRE(a.type == BooleanType);
    REQUIRE(a.value.bool_value == true);

    token = "False";
    REQUIRE(token_to_atom(token, a));
    REQUIRE(a.type == BooleanType);
    REQUIRE(a.value.bool_value == false);

    token = "1";
    REQUIRE(token_to_atom(token, a));
    REQUIRE(a.type == NumberType);
    REQUIRE(a.value.num_value == 1);

    token = "-1";
    REQUIRE(token_to_atom(token, a));
    REQUIRE(a.type == NumberType);
    REQUIRE(a.value.num_value == -1);

    token = "var";
    REQUIRE(token_to_atom(token, a));
    REQUIRE(a.type == SymbolType);
    REQUIRE(a.value.sym_value == "var");

    token = "1abc";
    REQUIRE(!token_to_atom(token, a));

    token = "var1";
    REQUIRE(token_to_atom(token, a));
    REQUIRE(a.type == SymbolType);
    REQUIRE(a.value.sym_value == token);
}

TEST_CASE("Test Expression Constructors", "[types]") {
    Expression exp1;

    REQUIRE(exp1 == Expression());
}


// tokenizer tests
TEST_CASE("Test Tokenizer with expected input", "[tokenize]") {
    std::string program = "(begin (define r 10) (* pi (* r r)))";

    std::istringstream iss(program);

    TokenSequenceType tokens = tokenize(iss);

    REQUIRE(tokens.size() == 17);
    REQUIRE(tokens[0] == "(");
    REQUIRE(tokens[1] == "begin");
    REQUIRE(tokens[2] == "(");
    REQUIRE(tokens[3] == "define");
    REQUIRE(tokens[4] == "r");
    REQUIRE(tokens[5] == "10");
    REQUIRE(tokens[6] == ")");
    REQUIRE(tokens[7] == "(");
    REQUIRE(tokens[8] == "*");
    REQUIRE(tokens[9] == "pi");
    REQUIRE(tokens[10] == "(");
    REQUIRE(tokens[11] == "*");
    REQUIRE(tokens[12] == "r");
    REQUIRE(tokens[13] == "r");
    REQUIRE(tokens[14] == ")");
    REQUIRE(tokens[15] == ")");
    REQUIRE(tokens[16] == ")");
}

TEST_CASE("Test Tokenizer with truncated input", "[tokenize]") {
    std::string program = "(f";

    std::istringstream iss(program);

    TokenSequenceType tokens = tokenize(iss);

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == "(");
    REQUIRE(tokens[1] == "f");
}
/*
TEST_CASE("Test Tokenizer with comment", "[tokenize]") {
    std::string program = ";hi \n h";

    std::istringstream iss(program);

    TokenSequenceType tokens = tokenize(iss);

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == "1");

}
*/
TEST_CASE("Test Tokenizer with single non-keyword", "[tokenize]") {
    std::string program = "hello";

    std::istringstream iss(program);

    TokenSequenceType tokens = tokenize(iss);

    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens[0] == "hello");
}

TEST_CASE("Test Tokenizer with empty input", "[tokenize]") {
    std::string program;

    std::istringstream iss(program);

    TokenSequenceType tokens = tokenize(iss);

    REQUIRE(tokens.empty());
}

TEST_CASE("Test Tokenizer with empty expression", "[tokenize]") {
    std::string program = "( )";

    std::istringstream iss(program);

    TokenSequenceType tokens = tokenize(iss);

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == "(");
    REQUIRE(tokens[1] == ")");
}


// Testing interpreter
TEST_CASE("Test Interpreter extra tests", "[interpreter]") {
    std::vector<std::string> programs = {
        "(1 2 >=)",
       "(1 2 <=)"

    };

    std::vector<Expression> expected_results = {
        Expression(false),
        Expression(true),
    };

    for (size_t i = 0; i < programs.size(); ++i) {
        std::istringstream iss(programs[i]);
        Interpreter interpreter;
        REQUIRE(interpreter.parse(iss));
        Expression result;
        REQUIRE_NOTHROW(result = interpreter.eval());
        REQUIRE(result == expected_results[i]);
    }
}

TEST_CASE("Test Interpreter trigonometric functions", "[interpreter][trigonometry]") {
    std::vector<std::string> programs = {
        "(0 sin)",                  // sin(0) = 0
        "(0 cos)",   // cos(pi) = -1

        "(0 1 arctan)"              // arctan(0, 1) = 0
    };

    std::vector<Expression> expected_results = {
        Expression(0.0),
        Expression(1.0),
        Expression(0.0)
    };

    for (size_t i = 0; i < programs.size(); ++i) {
        std::istringstream iss(programs[i]);
        Interpreter interpreter;
        REQUIRE(interpreter.parse(iss));
        Expression result;
        REQUIRE_NOTHROW(result = interpreter.eval());
        REQUIRE(result == expected_results[i]);
    }
}

TEST_CASE("Test Interpreter trigonometric function errors", "[interpreter][trigonometry][errors]") {
    std::vector<std::string> programs = {
        "(1 2 sin)",         // Too many arguments for sin
        "(cos)",             // No arguments for cos
        "(1 1 1 arctan)",    // Too many arguments for arctan
        "(1 arctan)"         // Too few arguments for arctan
    };

    for (const auto& program : programs) {
        std::istringstream iss(program);
        Interpreter interpreter;
        REQUIRE(interpreter.parse(iss));
        REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError);
    }
}