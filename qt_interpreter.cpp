#include "qt_interpreter.hpp"


// implemented with help form AI (primarly with the Brush and Pen aspect)

// default constuctor
QtInterpreter::QtInterpreter(QObject* parent) : QObject(parent) {}

void QtInterpreter::parseAndEvaluate(QString entry) {
    try {

        // parse and eval the input, converts from QString to std::string
        Expression result = interpreter.parseAndEvaluate(entry.toStdString());

        // emit graphics, messages based on eval result
        if (result.head.type == PointType) { // making the point
            // point with a small circle (ellipse) for this chatgpt helped me to get the -2.5 for the point values
            auto* point = new QGraphicsEllipseItem(result.head.value.point_value.x - 2.5, result.head.value.point_value.y - 2.5, 10, 10);
            point->setBrush(Qt::black); // set the color as black 
            emit drawGraphic(point); // emit to draw on canvas
        }
        else if (result.head.type == LineType) { // making a line
            // start and end points from result
            QPointF start(result.head.value.line_value.start.x, result.head.value.line_value.start.y);
            QPointF end(result.head.value.line_value.end.x, result.head.value.line_value.end.y);

            // create the QGraphicsLineItem from start to end
            auto* line = new QGraphicsLineItem(QLineF(start, end));
            line->setPen(QPen(Qt::black, 3)); // set color and thickness
            line->setPen(QPen(Qt::black, 3)); // set color and thickness
            emit drawGraphic(line); // emit to draw on canvas
        } 
        else if (result.head.type == ArcType) { // making an arc (with help from AI to fix previous code)
            // getting x and y center cordinates values
            double centerX = result.head.value.arc_value.center.x;
            double centerY = result.head.value.arc_value.center.y;

            // calculating the radius of the arc (center ot the start point)
            double radius = std::hypot(result.head.value.arc_value.start.x - centerX,
                result.head.value.arc_value.start.y - centerY);

            // box for the arc
            double x = centerX - radius; // left side
            double y = centerY - radius; // top side
            double di = radius * 2; // diameter of the box

            // starting angle for box 
            double startAngle = atan2(result.head.value.arc_value.start.y - centerY,
                result.head.value.arc_value.start.x - centerX) * (180 / std::atan2(0, -1));
            // calculatin gthe span angle 
            double spanAngle = result.head.value.arc_value.angle * (180 / std::atan2(0, -1));

            auto* arcItem = new QGraphicsArcItem(x, y, di, di);
            // start and span angles (have to convert to Qt's 1/16 degree units)
            arcItem->setStartAngle(startAngle * 16); // Use Qt's 1/16th degree units
            arcItem->setSpanAngle(spanAngle * 16);
            arcItem->setPen(QPen(Qt::black, 3)); // set color and thickness

            emit drawGraphic(arcItem); // emit to draw on canvas
        }
        // below is worked on after beta
        else if (result.head.type == RectType) {
            // two points for rect 
            double x1 = result.head.value.rect_value.point1.x;
            double y1 = result.head.value.rect_value.point1.y;
            double x2 = result.head.value.rect_value.point2.x;
            double y2 = result.head.value.rect_value.point2.y;

            // get the dimensions for QGraphicsRectItem function
            double x = std::min(x1, x2);
            double y = std::min(y1, y2);
            double width = std::fabs(x2 - x1);
            double height = std::fabs(y2 - y1);

            // Creating rectangle
            auto* rectItem = new QGraphicsRectItem(x, y, width, height);
            rectItem->setPen(QPen(Qt::black, 3)); // set color and thickness
            rectItem->setBrush(Qt::NoBrush);  // no fill 
            emit drawGraphic(rectItem);      // emit to draw on canvas
        }
        else if (result.head.type == FillRectType) {
            // same beginning as rect
            double x1 = result.head.value.fill_rect_value.rect.point1.x;
            double y1 = result.head.value.fill_rect_value.rect.point1.y;
            double x2 = result.head.value.fill_rect_value.rect.point2.x;
            double y2 = result.head.value.fill_rect_value.rect.point2.y;

            double x = std::min(x1, x2);
            double y = std::min(y1, y2);
            double width = std::fabs(x2 - x1);
            double height = std::fabs(y2 - y1);

            // RGB vals
            int r = static_cast<int>(result.head.value.fill_rect_value.r);
            int g = static_cast<int>(result.head.value.fill_rect_value.g);
            int b = static_cast<int>(result.head.value.fill_rect_value.b);

            auto* fillRectItem = new QGraphicsRectItem(x, y, width, height);
            fillRectItem->setBrush(QBrush(QColor(r, g, b)));  // brush fill with color vals
            fillRectItem->setPen(Qt::NoPen); // canvas examples have boarders, you can't have it for tests

            
            emit drawGraphic(fillRectItem);// emit to draw on canvas
        }
        else if (result.head.type == EllipseType) {
            // same as rectange getting the points 
            double x1 = result.head.value.ellipse_value.rect.point1.x;
            double y1 = result.head.value.ellipse_value.rect.point1.y;
            double x2 = result.head.value.ellipse_value.rect.point2.x;
            double y2 = result.head.value.ellipse_value.rect.point2.y;

            double x = std::min(x1, x2);
            double y = std::min(y1, y2);
            double width = std::fabs(x2 - x1);
            double height = std::fabs(y2 - y1);

            // Creating ellipse
            auto* ellipseItem = new QGraphicsEllipseItem(x, y, width, height);
            ellipseItem->setPen(QPen(Qt::black, 3)); // set color and thickness

            emit drawGraphic(ellipseItem);// emit to draw on canvas
        }

        emit info(QString::fromStdString(result.toString())); // for the postlisp commands
    }
    catch (const InterpreterSemanticError& err) { // catch any errors
        emit error(QString("Error: ") + err.what());
    }
}
