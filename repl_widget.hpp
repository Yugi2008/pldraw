#ifndef REPL_WIDGET_HPP
#define REPL_WIDGET_HPP

#include <QLineEdit>
#include <QString>
#include <QHBoxLayout>
#include <QLabel>
#include <QKeyEvent>

class REPLWidget : public QWidget {
    Q_OBJECT

public:
    // Default construct a REPLWidget
    REPLWidget(QWidget* parent = nullptr);

signals:
    // A signal that sends the current edited text as a QString when the return key is pressed.
    void lineEntered(QString entry);
    

private:
    QLineEdit* replEdit; // Input field for commands
    QStringList history; // History of entered commands
    int historyIndex;  // Index for history navigation

    // Navigate up or down through history
    void navigateHistory(int direction);
    void keyPressEvent(QKeyEvent* event) override;

    void changed();
};

#endif