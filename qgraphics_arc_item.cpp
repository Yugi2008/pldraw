#include "qgraphics_arc_item.hpp"


// AI was used for the general formating for this file

// constuctor initializing x, y, width, height, and optional parent item
QGraphicsArcItem::QGraphicsArcItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent)
    : QGraphicsEllipseItem(x, y, width, height, parent) {

}

// overrides the paint method to draw a arc without lines from the center to endpoints
void QGraphicsArcItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QGraphicsEllipseItem::paint(painter, option, widget); // full ellipse boundary
    painter->save(); // saves painter state

    
    QPen pen = painter->pen(); // painter's pen for arc 
    pen.setCapStyle(Qt::FlatCap);  // arc endpoints are flat
    painter->setPen(pen); // pen settings for the painter

    
    painter->drawArc(rect(), startAngle(), spanAngle());// draws arc portion from start and span angles

    painter->restore(); // get the painter state back to original 
}