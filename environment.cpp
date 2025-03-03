#include "environment.hpp"
#include "interpreter_semantic_error.hpp"
#include <cmath> 
#include <iostream>
// Example: Using function pointers


Environment::Environment() {
    this->reset();
}

void Environment::reset() {
    envmap.clear();

    //pi implementation
    Atom pi_atom;
    pi_atom.type = NumberType;
    pi_atom.value.num_value = std::atan2(0, -1);
    envmap["pi"] = EnvResult(ExpressionType, pi_atom);

    // Arithmetic operators
    envmap["+"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::add(args); // calls add with args
        });
    envmap["-"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::subtract(args);
        });
    envmap["*"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::multiply(args);
        });
    envmap["/"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::divide(args);
        });

    // Mathematical functions
    envmap["sqrt"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::sqrt(args);
        });
    envmap["log2"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::log2(args);
        });

    // Comparison operators
    envmap["<"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::less_than(args);
        });
    envmap["<="] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::less_than_equal(args);
        });
    envmap[">"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::greater_than(args);
        });
    envmap[">="] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::greater_than_equal(args);
        });
    envmap["=="] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::equal_to(args);
        });

    // Logical operators
    envmap["and"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::logical_and(args);
        });
    envmap["or"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::logical_or(args);
        });
    envmap["not"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::logical_not(args);
        });

    // trig functions
    envmap["sin"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::sin_func(args);
        });
    envmap["cos"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::cos_func(args);
        });
    envmap["arctan"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::arctan(args);
        });

    // graphics
    envmap["point"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::point(args);
        });

    envmap["line"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::line(args);
        });

    envmap["arc"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::arc(args);
        });

    envmap["draw"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::draw(args);
        });

    envmap["rect"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::rect(args); 
        });

    envmap["fill_rect"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::fill_rect(args);
        });
    envmap["ellipse"] = EnvResult(ProcedureType, [](Environment& env, const std::vector<Atom>& args) {
        return Environment::ellipse(args);
        });
    

}

Expression Environment::add(const std::vector<Atom>& args) {
    // check all aruments are numbers, while adding
    double result = 0;
    for (const auto& a : args) {
        if (a.type != NumberType) {
            throw InterpreterSemanticError("Error in call to add, argument not a number");
        }
        result += a.value.num_value;
    }

    return Expression(result);
};

// TODO: add further functions necessary to implement the requirements of Project 2.

Expression Environment::subtract(const std::vector<Atom>& args) {
    if (args.size() == 1) {
        if (args[0].type != NumberType) {
            throw InterpreterSemanticError("Error in call to subtract: argument not a number");
        }
        return Expression(-args[0].value.num_value);
    }
    if (args.size() == 2) {
        if (args[0].type != NumberType || args[1].type != NumberType) {
            throw InterpreterSemanticError("Error in call to subtract: arguments not numbers");
        }
        return Expression(args[0].value.num_value - args[1].value.num_value);
    }
    throw InterpreterSemanticError("Error in call to subtract: wrong number of arguments");
}


Expression Environment::multiply(const std::vector<Atom>& args) {
    double result = 1;
    for (const auto& a : args) {
        if (a.type != NumberType) {
            throw InterpreterSemanticError("Error in call to multiply: argument not a number");
        }
        result *= a.value.num_value;
    }
    return Expression(result);
}

Expression Environment::divide(const std::vector<Atom>& args) {
    if (args.size() != 2) {
        throw InterpreterSemanticError("Error in call to divide: wrong number of arguments");
    }
    if (args[0].type != NumberType || args[1].type != NumberType) {
        throw InterpreterSemanticError("Error in call to divide: argument not a number");
    }
    if (args[1].value.num_value == 0) {
        throw InterpreterSemanticError("Error in call to divide: division by zero");
    }
    return Expression(args[0].value.num_value / args[1].value.num_value);
}


Expression Environment::sqrt(const std::vector<Atom>& args) {
    if (args.size() != 1 || args[0].type != NumberType || args[0].value.num_value < 0) {
        throw InterpreterSemanticError("Error in call to sqrt, invalid argument");
    }
    return Expression(std::sqrt(args[0].value.num_value));
}

Expression Environment::log2(const std::vector<Atom>& args) {
    if (args.size() != 1 || args[0].type != NumberType || args[0].value.num_value <= 0) {
        throw InterpreterSemanticError("Error in call to log2, invalid argument");
    }
    return Expression(std::log2(args[0].value.num_value));
}

Expression Environment::less_than(const std::vector<Atom>& args) {
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType) {
        throw InterpreterSemanticError("Error in call to less_than, invalid arguments");
    }
    return Expression(args[0].value.num_value < args[1].value.num_value);
}


Expression Environment::less_than_equal(const std::vector<Atom>& args) {
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType) {
        throw InterpreterSemanticError("Error in call to less_than_equal, invalid arguments");
    }
    return Expression(args[0].value.num_value <= args[1].value.num_value);
}

Expression Environment::greater_than(const std::vector<Atom>& args) {
    if (args.size() != 2) {
        throw InterpreterSemanticError("Error in call to greater_than: wrong number of arguments");
    }
    if (args[0].type != NumberType || args[1].type != NumberType) {
        throw InterpreterSemanticError("Error in call to greater_than: argument not a number");
    }
    return Expression(args[0].value.num_value > args[1].value.num_value);
}

Expression Environment::greater_than_equal(const std::vector<Atom>& args) {
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType) {
        throw InterpreterSemanticError("Error in call to greater_than_equal: wrong number of arguments or invalid type");
    }
    return Expression(args[0].value.num_value >= args[1].value.num_value);
}

Expression Environment::equal_to(const std::vector<Atom>& args) {
    if (args.size() != 2) {
        throw InterpreterSemanticError("Error in call to equal_to: wrong number of arguments");
    }
    if (args[0].type != NumberType || args[1].type != NumberType) {
        throw InterpreterSemanticError("Error in call to equal_to: argument not a number");
    }
    return Expression(fabs(args[0].value.num_value - args[1].value.num_value) < std::numeric_limits<double>::epsilon());
}

Expression Environment::logical_and(const std::vector<Atom>& args) {
    for (const auto& a : args) {
        if (a.type != BooleanType) {
            throw InterpreterSemanticError("Error in call to and: argument not a boolean");
        }
        if (!a.value.bool_value) {
            return Expression(false);
        }
    }
    return Expression(true);
}

Expression Environment::logical_or(const std::vector<Atom>& args) {
    for (const auto& a : args) {
        if (a.type != BooleanType) {
            throw InterpreterSemanticError("Error in call to or: argument not a boolean");
        }
        if (a.value.bool_value) {
            return Expression(true);
        }
    }
    return Expression(false);
}

Expression Environment::logical_not(const std::vector<Atom>& args) {
    if (args.size() != 1) {
        throw InterpreterSemanticError("Error in call to not: wrong number of arguments");
    }
    if (args[0].type != BooleanType) {
        throw InterpreterSemanticError("Error in call to not: argument not a boolean");
    }
    return Expression(!args[0].value.bool_value);
}

Expression Environment::sin_func(const std::vector<Atom>& args) {
    if (args.size() != 1) {
        throw InterpreterSemanticError("sin expects one argument");
    }
    return Expression(sin(args[0].value.num_value));
}

Expression Environment::cos_func(const std::vector<Atom>& args) {
    if (args.size() != 1) {
        throw InterpreterSemanticError("cos expects one argument");
    }
    return Expression(cos(args[0].value.num_value));
}

Expression Environment::arctan(const std::vector<Atom>& args) {
    if (args.size() != 2) {
        throw InterpreterSemanticError("arctan expects two arguments");
    }
    return Expression(atan2(args[0].value.num_value, args[1].value.num_value));
}

Expression Environment::point(const std::vector<Atom>& args) {
    if (args.size() != 2) { // takes two Numbers
        throw InterpreterSemanticError("point expects two arguments");
    }

    if (args[0].type != NumberType || args[1].type != NumberType) { // must be numbers
        throw InterpreterSemanticError("point arguments must be numbers");
    }

    // creating the PointType with  x and y coordinates
    Expression point(std::make_tuple(args[0].value.num_value, args[1].value.num_value));
    point.head.type = PointType; // make the head PointType
    return point;
}


Expression Environment::line(const std::vector<Atom>& args) {
    // outputs made with help of AI
    // if 2 inputs get the coordinates from the PointType atoms
    if (args.size() == 2 && args[0].type == PointType && args[1].type == PointType) {
        std::tuple<double, double> start(args[0].value.point_value.x, args[0].value.point_value.y);
        std::tuple<double, double> end(args[1].value.point_value.x, args[1].value.point_value.y);
        return Expression(start, end);
    }

    // make sure the arguments are numbers and tthere are 4
    if (args.size() == 4) {
        for (const auto& arg : args) {
            if (arg.type != NumberType) {
                throw InterpreterSemanticError("line arguments must be numbers");
            }
        }
        // line start and end points from the four args
        std::tuple<double, double> start(args[0].value.num_value, args[1].value.num_value);
        std::tuple<double, double> end(args[2].value.num_value, args[3].value.num_value);
        return Expression(start, end);
    }
    else {
        throw InterpreterSemanticError("line expects either four numbers or two points as arguments");
    }
}

Expression Environment::arc(const std::vector<Atom>& args) {
    // outputs made with help of AI
    // two points and an angle
    if (args.size() == 3 && args[0].type == PointType && args[1].type == PointType && args[2].type == NumberType) {
        std::tuple<double, double> center(args[0].value.point_value.x, args[0].value.point_value.y);
        std::tuple<double, double> start(args[1].value.point_value.x, args[1].value.point_value.y);
        double angle = args[2].value.num_value;

        return Expression(center, start, angle);
    }
    // five NumberType numeric args
    if (args.size() == 5) {
        for (const auto& arg : args) {
            if (arg.type != NumberType) {
                throw InterpreterSemanticError("arc arguments must be numbers");
            }
        }

        // center, start, and angle using the five args
        std::tuple<double, double> center(args[0].value.num_value, args[1].value.num_value);
        std::tuple<double, double> start(args[2].value.num_value, args[3].value.num_value);
        double angle = args[4].value.num_value;

        return Expression(center, start, angle);
    }
    else {
        throw InterpreterSemanticError("arc expects either five numbers or two points and an angle");
    }
}


Expression Environment::draw(const std::vector<Atom>& args) {
    if (args.size() != 1) {
        throw InterpreterSemanticError("draw expects one argument");
    }

    // check if graphic type, PointType, LineType, ArcType
    if (args[0].type == PointType || args[0].type == LineType || args[0].type == ArcType || args[0].type == RectType || args[0].type == FillRectType || args[0].type == EllipseType) {
        return Expression(args[0]);  // Return Expression type
    }
    
    throw InterpreterSemanticError("draw can only be used with graphical types");
    
}

Expression Environment::rect(const std::vector<Atom>& args) {
    if (args.size() != 2) { // two arguments
        throw InterpreterSemanticError("rect expects two arguments: top-left and bottom-right points");
    }

    if (args[0].type != PointType || args[1].type != PointType) { // both args Points have to be points
        throw InterpreterSemanticError("rect arguments must be points");
    }

    // get the two point values
    Point point1 = args[0].value.point_value;
    Point point2 = args[1].value.point_value;

    return Expression(point1, point2);
}

Expression Environment::fill_rect(const std::vector<Atom>& args) {
    if (args.size() != 4 && args.size() != 7) { // args cant be 4 or 7 
        throw InterpreterSemanticError("fill_rect expects a rect and three color values (r, g, b)");
    }

    if (args[0].type != RectType) { // must be a rect type
        throw InterpreterSemanticError("First argument to fill_rect must be a rectangle");
    }

    Rectt rect = args[0].value.rect_value; // first arg is rect 

    // rest are color values
    double r = args[1].value.num_value;
    double g = args[2].value.num_value;
    double b = args[3].value.num_value;

    // color range must be in range 0-255
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        throw InterpreterSemanticError("Color values must be in the range [0, 255]");
    }

    return Expression(rect, r, g, b);
}
Expression Environment::ellipse(const std::vector<Atom>& args) {
    if (args.size() != 1 || args[0].type != RectType) { // only arg is a rect
        throw InterpreterSemanticError("ellipse expects one argument of type rect");
    }

    Ellipsee ellipse;
    ellipse.rect = args[0].value.rect_value; // the only arg is rect


    Expression ellipseExp;
    ellipseExp.head.type = EllipseType;
    ellipseExp.head.value.ellipse_value = ellipse;
    return ellipseExp;
}






void Environment::define(const Symbol& sym, const Expression& exp) {
    envmap[sym] = EnvResult(ExpressionType, exp);
}

// making a new procedure mapping
void Environment::define(const Symbol& sym, std::function<Expression(Environment&, const std::vector<Atom>&)> proc) {
    envmap[sym] = EnvResult(ProcedureType, proc);
}

bool Environment::isDefined(const Symbol& sym) const {
    return envmap.find(sym) != envmap.end();
}



// get a mapping
Expression Environment::get(const Symbol& sym) const {
    auto it = envmap.find(sym);
    if (it == envmap.end()) {
        throw InterpreterSemanticError("Error: Symbol '" + sym + "' not found");
    }
    if (it->second.type == ExpressionType) {
        return it->second.exp;  // return expression
    }
    throw InterpreterSemanticError("Error: Symbol '" + sym + "' is not an expression");
}


bool Environment::isProcedure(const Symbol& sym) const {
    auto it = envmap.find(sym);
    return it != envmap.end() && it->second.type == ProcedureType;
}
EnvResult Environment::getResult(const Symbol& sym) const {
    auto it = envmap.find(sym);
    if (it == envmap.end()) {
        throw InterpreterSemanticError("Error: Symbol '" + sym + "' not found");
    }
    return it->second;  // returns procedure or expression
}


bool Environment::isKeyword(const std::string& symbol) {
    static const std::set<std::string> keywords = { "+", "-", "*", "/", "sqrt", "log2", "and", "or", "not", "<", "<=", ">", ">=", "==" , "define", "begin", "if", "pi", "True", "False", "sin", "cos", "arctan", "point", "line", "arc", "draw", "rect", "fill_rect", "ellipse"};
    return keywords.find(symbol) != keywords.end();
}


EnvResult::EnvResult() : type(ExpressionType), exp(Expression()), proc(nullptr) {}

EnvResult::EnvResult(EnvResultType eType, Expression eExp)
    : type(eType), exp(eExp), proc(nullptr) {}

EnvResult::EnvResult(EnvResultType eType, std::function<Expression(Environment&, const std::vector<Atom>&)> eProc)
    : type(eType), exp(Expression()), proc(eProc) {}