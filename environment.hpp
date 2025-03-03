#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <unordered_map>
#include <string>
#include <vector>
#include <set> 
#include <functional>

// module includes
#include "expression.hpp"

class Environment;

enum EnvResultType { ExpressionType, ProcedureType };

struct EnvResult {
    EnvResultType type;
    Expression exp;
    std::function<Expression(Environment&, const std::vector<Atom>&)> proc;

    EnvResult(); // Default constructor
    EnvResult(EnvResultType eType, Expression eExp); // for ExpressionType
    EnvResult(EnvResultType eType, std::function<Expression(Environment&, const std::vector<Atom>&)> eProc); // for ProcedureType
};

class Environment {
public:
    std::unordered_map<Symbol, EnvResult> envmap;

    Environment();
    void reset();

    void define(const Symbol& sym, const Expression& exp);
    void define(const Symbol& sym, std::function<Expression(Environment&, const std::vector<Atom>&)> proc);
    bool isDefined(const Symbol& sym) const;
    Expression get(const Symbol& sym) const;

    EnvResult getResult(const Symbol& sym) const;
    bool isProcedure(const Symbol& sym) const;
    static bool isKeyword(const std::string& symbol);


private:
    // Arithmetic operations
    static Expression add(const std::vector<Atom>& args);
    static Expression subtract(const std::vector<Atom>& args);
    static Expression multiply(const std::vector<Atom>& args);
    static Expression divide(const std::vector<Atom>& args);

    // Comparison operations
    static Expression less_than(const std::vector<Atom>& args);
    static Expression less_than_equal(const std::vector<Atom>& args);
    static Expression greater_than(const std::vector<Atom>& args);
    static Expression greater_than_equal(const std::vector<Atom>& args);
    static Expression equal_to(const std::vector<Atom>& args);

    // Logical operations
    static Expression logical_and(const std::vector<Atom>& args);
    static Expression logical_or(const std::vector<Atom>& args);
    static Expression logical_not(const std::vector<Atom>& args);
    static Expression sqrt(const std::vector<Atom>& args);
    static Expression log2(const std::vector<Atom>& args);

    // Trig Functions
    static Expression sin_func(const std::vector<Atom>& args);
    static Expression cos_func(const std::vector<Atom>& args);
    static Expression arctan(const std::vector<Atom>& args);

    // graphicss
    static Expression point(const std::vector<Atom>& args);
    static Expression line(const std::vector<Atom>& args);
    static Expression arc(const std::vector<Atom>& args);
    static Expression rect(const std::vector<Atom>& args);
    static Expression fill_rect(const std::vector<Atom>& args);
    static Expression ellipse(const std::vector<Atom>& args);


    static Expression draw(const std::vector<Atom>& args);
};

#endif