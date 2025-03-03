#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "qt_interpreter.hpp"

#include <QWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QMessageBox>
#include <fstream>

class MainWindow: public QWidget{
  Q_OBJECT

public:

	// Default construct a MainWindow
    explicit MainWindow(QWidget* parent = nullptr);

	// Default construct a MainWidow, using filename as the script file to attempt to preload
    MainWindow(const std::string& filename, QWidget* parent = nullptr);
    


private:
    MessageWidget* messageWidget;
    CanvasWidget* canvasWidget;
    REPLWidget* replWidget;
    QtInterpreter interpreter;

    void executeScript(const QString& filename);

};


#endif
