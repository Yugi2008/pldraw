#include "main_window.hpp"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), interpreter(this) {

    // widgets in the interface (calling the different files)
    auto* messageWidget = new MessageWidget(this);
    auto* canvasWidget = new CanvasWidget(this);
    auto* replWidget = new REPLWidget(this);

    // layout in interface
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(messageWidget);
    layout->addWidget(canvasWidget);
    layout->addWidget(replWidget);
    setLayout(layout);

    // below connect statements were assisted by AI
    // Connect REPL input to interpreter
    connect(replWidget, &REPLWidget::lineEntered, &interpreter, &QtInterpreter::parseAndEvaluate);

    // Connecting the interpreter's outputs to GUI components
    // connection for graphical objects
    connect(&interpreter, &QtInterpreter::drawGraphic, canvasWidget, &CanvasWidget::addGraphic);

    // connection allows informational messages from QtInterpreter to be shown to the user in MessageWidget
    connect(&interpreter, &QtInterpreter::info, messageWidget, &MessageWidget::info);

    // connection for error communication to the user through MessageWidget
    connect(&interpreter, &QtInterpreter::error, messageWidget, &MessageWidget::error);
}

// if used a file name
MainWindow::MainWindow(const std::string& filename, QWidget* parent) : MainWindow(parent) {
    executeScript(QString::fromStdString(filename));
}

// Slot to execute a script
void MainWindow::executeScript(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file: " + filename);
        return;
    }

    QString script = file.readAll();
    try {
        interpreter.parseAndEvaluate(script);
    }
    catch (const InterpreterSemanticError& err) {
        QMessageBox::warning(this, "Error", QString("Execution Error: ") + err.what());
    }
}
