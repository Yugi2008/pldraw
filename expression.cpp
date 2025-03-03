#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>
#include <sstream>

Expression::Expression(bool tf) {
    head.type = BooleanType;
    head.value.bool_value = tf;
}

Expression::Expression(double num) {
    head.type = NumberType;
    head.value.num_value = num;
}

Expression::Expression(const std::string& sym) {
    head.type = SymbolType;
    head.value.sym_value = sym;
}

bool Expression::operator==(const Expression& exp) const noexcept {
    if (head.type != exp.head.type) {
        return false;
    }

    switch (head.type) {
    case NoneType:
        if (exp.head.type != NoneType) {
            return false;
        }
        break;
    case BooleanType:
        if (head.value.bool_value != exp.head.value.bool_value) {
            return false;
        }
        break;
    case NumberType: {
        double left = head.value.num_value;
        double right = exp.head.value.num_value;
        if (fabs(left - right) > std::numeric_limits<double>::epsilon()) {
            return false;
        }
    }
      break;
    case SymbolType:
        if (head.value.sym_value != exp.head.value.sym_value) {
            return false;
        }
        break;
        
    case PointType:
        return std::fabs(head.value.point_value.x - exp.head.value.point_value.x) < std::numeric_limits<double>::epsilon() &&
               std::fabs(head.value.point_value.y - exp.head.value.point_value.y) < std::numeric_limits<double>::epsilon();
        break;
    case LineType:
        return std::fabs(head.value.line_value.start.x - exp.head.value.line_value.start.x) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.line_value.start.y - exp.head.value.line_value.start.y) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.line_value.end.x - exp.head.value.line_value.end.x) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.line_value.end.y - exp.head.value.line_value.end.y) < std::numeric_limits<double>::epsilon();
        break;
    case ArcType:
        return std::fabs(head.value.arc_value.center.x - exp.head.value.arc_value.center.x) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.arc_value.center.y - exp.head.value.arc_value.center.y) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.arc_value.start.x - exp.head.value.arc_value.start.x) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.arc_value.start.y - exp.head.value.arc_value.start.y) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.arc_value.angle - exp.head.value.arc_value.angle) < std::numeric_limits<double>::epsilon();
        break;
    case RectType:
        return std::fabs(head.value.rect_value.point1.x - exp.head.value.rect_value.point1.x) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.rect_value.point1.y - exp.head.value.rect_value.point1.y) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.rect_value.point2.x - exp.head.value.rect_value.point2.x) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.rect_value.point2.y - exp.head.value.rect_value.point2.y) < std::numeric_limits<double>::epsilon();
        break;
    case FillRectType:
        return std::fabs(head.value.fill_rect_value.rect.point1.x - exp.head.value.fill_rect_value.rect.point1.x) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.fill_rect_value.rect.point1.y - exp.head.value.fill_rect_value.rect.point1.y) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.fill_rect_value.rect.point2.x - exp.head.value.fill_rect_value.rect.point2.x) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.fill_rect_value.rect.point2.y - exp.head.value.fill_rect_value.rect.point2.y) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.fill_rect_value.r - exp.head.value.fill_rect_value.r) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.fill_rect_value.g - exp.head.value.fill_rect_value.g) < std::numeric_limits<double>::epsilon() &&
            std::fabs(head.value.fill_rect_value.b - exp.head.value.fill_rect_value.b) < std::numeric_limits<double>::epsilon();
        break; 
    case EllipseType:
        return head.value.ellipse_value.rect.point1.x == exp.head.value.ellipse_value.rect.point1.x &&
            head.value.ellipse_value.rect.point1.y == exp.head.value.ellipse_value.rect.point1.y &&
            head.value.ellipse_value.rect.point2.x == exp.head.value.ellipse_value.rect.point2.x &&
            head.value.ellipse_value.rect.point2.y == exp.head.value.ellipse_value.rect.point2.y;
        break;

    default:
        return false;
    }

    return true;
}
std::string Expression::toString() const {
    std::ostringstream oss;

    switch (head.type) {
    case NoneType:
        oss << "()";
        break;
    case BooleanType:
        oss << "(" << (head.value.bool_value ? "True" : "False") << ")";
        break;
    case NumberType:
        oss << "(" << head.value.num_value << ")";
        break;
    case SymbolType:
        oss << "(" << head.value.sym_value << ")";
        break;
       
    case PointType:
        oss << "(" << head.value.point_value.x << "," << head.value.point_value.y << ")";
        break;
    case LineType:
        oss << "((" << head.value.line_value.start.x << "," << head.value.line_value.start.y << "),("
            << head.value.line_value.end.x << "," << head.value.line_value.end.y << "))";
        break;
    case ArcType:
        oss << "((" << head.value.arc_value.center.x << "," << head.value.arc_value.center.y << "),("
            << head.value.arc_value.start.x << "," << head.value.arc_value.start.y << ") "
            << head.value.arc_value.angle << ")";
        break;
    case RectType:
        oss << "((" << head.value.rect_value.point1.x << "," << head.value.rect_value.point1.y << "),("
            << head.value.rect_value.point2.x << "," << head.value.rect_value.point2.y << "))";
        break;
    case FillRectType:
        oss << "((" << head.value.fill_rect_value.rect.point1.x << "," << head.value.fill_rect_value.rect.point1.y << "),("
            << head.value.fill_rect_value.rect.point2.x << "," << head.value.fill_rect_value.rect.point2.y << ") ("
            << head.value.fill_rect_value.r << "," << head.value.fill_rect_value.g << ","
            << head.value.fill_rect_value.b << "))";
        break;
    case EllipseType:
        oss << "((" << head.value.ellipse_value.rect.point1.x << ","
            << head.value.ellipse_value.rect.point1.y << "),("
            << head.value.ellipse_value.rect.point2.x << ","
            << head.value.ellipse_value.rect.point2.y << "))";
        break;
        

    default:
        oss << "()";
        break;
    }

    return oss.str();
}
std::ostream& operator<<(std::ostream& out, const Expression& exp) {
    out << exp.toString();
    return out;
}
// initial expression constuctors are impplemented with help from AI the rest I implementied in
// the same format as the AI model
Expression::Expression(std::tuple<double, double> value) {
    head.type = PointType;
    head.value.point_value.x = std::get<0>(value); 
    head.value.point_value.y = std::get<1>(value);  
}



// Constructor for a Line
Expression::Expression(std::tuple<double, double> start, std::tuple<double, double> end) {
    head.type = LineType;  
    head.value.line_value.start.x = std::get<0>(start); 
    head.value.line_value.start.y = std::get<1>(start); 
    head.value.line_value.end.x = std::get<0>(end);  
    head.value.line_value.end.y = std::get<1>(end);  
}



// constructor for an Arc
Expression::Expression(std::tuple<double, double> center, std::tuple<double, double> start, double angle) {
    head.type = ArcType;  
    head.value.arc_value.center.x = std::get<0>(center);  
    head.value.arc_value.center.y = std::get<1>(center);  
    head.value.arc_value.start.x = std::get<0>(start);  
    head.value.arc_value.start.y = std::get<1>(start);
    head.value.arc_value.angle = angle;  
}


// Constructor for Rectangle
Expression::Expression(Point point1, Point point2) {
    head.type = RectType;  
    head.value.rect_value.point1 = point1;
    head.value.rect_value.point2 = point2;
}

// Constructor for fill rectangle
Expression::Expression(Rectt rect, double r, double g, double b) {
    head.type = FillRectType;

    head.value.fill_rect_value.rect = rect; // rectangle values

    // color values
    head.value.fill_rect_value.r = r;
    head.value.fill_rect_value.g = g;
    head.value.fill_rect_value.b = b;
}


Expression::Expression(const Rectt& boundingRect) {
    head.type = EllipseType;
    head.value.ellipse_value.rect = boundingRect;
}



bool token_to_atom(const std::string& token, Atom& atom) {
    // is token boolean keyword
    if (token == "True") {
        atom.type = BooleanType;
        atom.value.bool_value = true;
    }
    else if (token == "False") {
        atom.type = BooleanType;
        atom.value.bool_value = false;
    }
    else {
        // is token a number
        double temp;

        std::istringstream iss(token);
        if (iss >> temp) {
            // check for trailing characters if >> succeeds
            if (iss.rdbuf()->in_avail() != 0) {
                return false;
            }

            atom.type = NumberType;
            atom.value.num_value = temp;
        }
        else {
            // else assume symbol
            atom.type = SymbolType;
            atom.value.sym_value = token;

            // make sure does not start with number
            if (std::isdigit(token[0]) != 0) {
                return false;
            }
        }
    }

    return true;
}
