#ifndef MESSAGE_WINDOW_HPP
#define MESSAGE_WINDOW_HPP

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class QLineEdit;

class MessageWidget : public QWidget {

public:
  // Default construct a MessageWidget displaying no text
    MessageWidget(QWidget* parent = nullptr);

  // a public slot accepting an informational message to display, clearing any error formatting
  void info(QString message);


  // a public slot accepting an error message to display as selected text highlighted with a red background.
  void error(QString message);

  void clear();

private:
    QLineEdit* messageEdit = nullptr;
};

#endif
