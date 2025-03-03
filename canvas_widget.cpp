

#include "canvas_widget.hpp"

// Framework assisted with ai(chatgpt) primarly the use of scene(new QGraphicsScene(this)), view(nullptr) aparameters
CanvasWidget::CanvasWidget(QWidget* parent) : QWidget(parent), scene(new QGraphicsScene(this)), view(nullptr) {
    view = new QGraphicsView(scene, this);

    // uses the QVBoxLayout library, it automatically resizes to fill the widget’s available space.
    auto layout = new QVBoxLayout(this); //  creates new vertical layout
    layout->addWidget(view); // adds the view widget to the layout.
    setLayout(layout); // sets layout as the main layout manager for CanvasWidget
}

// adding item so scene
void CanvasWidget::addGraphic(QGraphicsItem* item) { 
    scene->addItem(item);
}
