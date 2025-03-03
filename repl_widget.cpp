#include "repl_widget.hpp"


// base stucture was assisted by AI (like the other files regarding widgets same format heavliy changed
// probably nothing even lasted from using the AI mainly the history)
REPLWidget::REPLWidget(QWidget* parent) : QWidget(parent), historyIndex(-1) {

    // the user input
    replEdit = new QLineEdit(this);

    // layout of the input
    auto* layout = new QHBoxLayout(this);
    QLabel* promptLabel = new QLabel("slisp> ", this);
    layout->addWidget(promptLabel);  // add slisp>  label
    layout->addWidget(replEdit);     // add input 
    setLayout(layout);

    // connecting the returnPressed signal to the already standing one
    connect(replEdit, &QLineEdit::returnPressed, this, &REPLWidget::changed);

}

void REPLWidget::changed() {
    QString text = replEdit->text();
    if (!text.isEmpty()) {
        emit lineEntered(text);       // emit the entered text
        history.prepend(text);        // add to history of the entered text
        historyIndex = -1;            // reset the history
        replEdit->clear();            // clear input text
    }
}




// below was assisted with AI (chatgpt)
void REPLWidget::navigateHistory(int direction) {
    if (history.isEmpty()) return;

    // history based on direction
    historyIndex += direction;
    if (historyIndex < 0) { // can only be positive (history cant be negative)
        historyIndex = 0;
    }
    else if (historyIndex >= history.size()) {// can't go past the end
        historyIndex = history.size() - 1;
    }
    
    replEdit->setText(history[historyIndex]);// update input with the past command
}

void REPLWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Up) {
        navigateHistory(1);  //  up in history
    }
    else if (event->key() == Qt::Key_Down) {
        navigateHistory(-1); // Navigate down in history
    }
    else {
        QWidget::keyPressEvent(event);  // handling for other keys as normal
    }
}