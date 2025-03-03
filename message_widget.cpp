#include "message_widget.hpp"

#include <QVBoxLayout> // for some reason it has to be in the same file not the .hpp :/

MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent) {
    messageEdit = new QLineEdit(this);
    messageEdit->setReadOnly(true); // set the box to read only so you can't edit it

    // layout was with help from AI for general idea
    auto* layout = new QHBoxLayout(this);
    QLabel* textM = new QLabel("Message:", this); // text for the message output box 
    layout->addWidget(textM);       // add the text label
    layout->addWidget(messageEdit);  // add the message display box
    setLayout(layout);
}

// a public slot accepting an informational message to display, clearing any error formatting
void MessageWidget::info(QString message) {
    messageEdit->setText(message);
    messageEdit->setStyleSheet("color: black;"); // color is standard black
}

// a public slot accepting an error message to display as selected text HIGHLIGHTED with a red background.
void MessageWidget::error(QString message) {
    messageEdit->setText("Error: " + message);


    QPalette p = messageEdit->palette();
    p.setColor(QPalette::Highlight, Qt::red); // Red background for highlight
    p.setColor(QPalette::HighlightedText, Qt::white);
    messageEdit->setPalette(p);

    // Force the text box to use the highlight background
    messageEdit->selectAll();
}

// clearign and going to reset the highlight
void MessageWidget::clear() {
    messageEdit->clear();
    messageEdit->setStyleSheet("QLabel { color: black; background-color: none; }");
}