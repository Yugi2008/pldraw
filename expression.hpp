#ifndef TYPES_HPP
#define TYPES_HPP

// system includes
#include <string>
#include <vector>
#include <tuple>
#include <iostream>


// A Type is a literal boolean, literal number, or symbol
enum Type {
    NoneType,
    BooleanType,
    NumberType,
    ListType,
    SymbolType,
    PointType,
    LineType,
    ArcType,
    RectType,
    FillRectType,
    EllipseType
};

// A Boolean is a C++ bool
typedef bool Boolean;

// A Number is a C++ double
typedef double Number;

// A Symbol is a string
typedef std::string Symbol;


struct Point {
    double x;
    double y;
};

struct Line {
    Point start;
    Point end;
};

struct Arcn {
    Point center;
    Point start;
    double angle; // angle in radians
};

struct Rectt {
    Point point1;
    Point point2;
};
struct FillRectt {
    Rectt rect;
    double r;
    double g;
    double b;
};
struct Ellipsee {
    Rectt rect; 
};

// A Value is a boolean, number, or symbol
// cannot use a union because symbol is non-POD
// this wastes space but is simple 
struct Value {
    Boolean bool_value;
    Number num_value;
    Symbol sym_value;
    Point point_value;  
    Line line_value;    
    Arcn arc_value;
    Rectt rect_value;
    FillRectt fill_rect_value;
    Ellipsee ellipse_value;
};

// An Atom has a type and value
struct Atom {
    Type type;
    Value value;
};

// An expression is an atom called the head
// followed by a (possibly empty) list of expressions
// called the tail
struct Expression {
    Atom head;
    std::vector<Expression> tail;

    Expression() {
        head.type = NoneType;
    };

    Expression(const Atom& atom) : head(atom) {
    };

    Expression(bool tf);

    Expression(double num);

    Expression(const std::string& sym);

    // constuctor for point
    Expression(std::tuple<double, double> value); 

    // Constuctor for line
    Expression(std::tuple<double, double> start, std::tuple<double, double> end); 
        

    // constructor for arc
    Expression(std::tuple<double, double> center, std::tuple<double, double> start, double angle);


    // constructor for rectangle
    Expression(Point point1, Point point2);

    // constructor for fill rect 
    Expression(Rectt rect, double r, double g, double b);

    // constructor for ellipse
    Expression(const Rectt& boundingRect);

    bool operator==(const Expression& exp) const noexcept;

    std::string toString() const;

};


// A Procedure is a C++ function pointer taking
// a vector of Atoms as arguments
typedef Expression(*Procedure)(const std::vector<Atom>& args);

// format an expression for output
std::ostream& operator<<(std::ostream& out, const Expression& exp);

// map a token to an Atom
bool token_to_atom(const std::string& token, Atom& atom);

#endif
