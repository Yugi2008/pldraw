#include <QDebug>
#include <QtTest/QtTest>
#include <QtWidgets>

#include "canvas_widget.hpp"
#include "main_window.hpp"
#include "message_widget.hpp"
#include "repl_widget.hpp"

class unittests_gui : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void testPoint();
    void testLine();
    void testArc();
    void testRect();
    void testFillRect();
    void testEllipse();
    void testValidPoint();
    void testInvalidCommands();
    void testIncompleteCommand();
    void testTooManyArguments();
    void testUnsupportedType();
    void testMissingArguments();
    void testArithmeticErrors();
    void testRedefinitionErrors();
    void cleanupTestCase();
    void testMalformedGraphicsCommands();
    void testPointOutsideCanvas();
    void testHistoryNavigation();
    void testColorValidation();
    void testValidFillRectColors();
    void testCanvasClearing();


private:
    MainWindow mainWindow;
    REPLWidget* repl;
    QLineEdit* replEdit;
    CanvasWidget* canvas;
    QGraphicsScene* scene;
    MessageWidget* message;
    QLineEdit* messageEdit;

};


void unittests_gui::testPoint() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QTest::keyClicks(replEdit, "((0 0 point) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(scene->itemAt(QPointF(0, 0), QTransform()) != nullptr,
        "Expected a point in the scene. Not found.");
}

void unittests_gui::testLine() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QTest::keyClicks(replEdit, "(((10 0 point) (0 10 point) line) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(scene->itemAt(QPointF(10, 0), QTransform()) != nullptr,
        "Expected a line in the scene. Not found.");
    QVERIFY2(scene->itemAt(QPointF(0, 10), QTransform()) != nullptr,
        "Expected a line in the scene. Not found.");
}

void unittests_gui::testArc() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QTest::keyClicks(replEdit, "(((0 0 point) (100 0 point) pi arc) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(scene->itemAt(QPointF(100, 0), QTransform()) != nullptr,
        "Expected a point on the arc in the scene. Not found.");
}

void unittests_gui::testRect() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QTest::keyClicks(replEdit, "(((0 0 point) (10 10 point) rect) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(scene->itemAt(QPointF(0, 0), QTransform()) != nullptr,
        "Expected a rectangle in the scene. Not found.");
}

void unittests_gui::testFillRect() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QTest::keyClicks(replEdit, "((((-10 -10 point) (10 10 point) rect) 255 0 0 fill_rect) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(scene->itemAt(QPointF(0, 0), QTransform()) != nullptr,
        "Expected a filled rectangle in the scene. Not found.");
}

void unittests_gui::testEllipse() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QTest::keyClicks(replEdit, "((((0 0 point) (20 20 point) rect) ellipse) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(scene->itemAt(QPointF(10, 10), QTransform()) != nullptr,
        "Expected an ellipse in the scene. Not found.");
}

void unittests_gui::initTestCase() {
    repl = mainWindow.findChild<REPLWidget*>();
    QVERIFY2(repl, "Could not find REPLWidget instance in MainWindow instance.");

    replEdit = repl->findChild<QLineEdit*>();
    QVERIFY2(replEdit, "Could not find QLineEdit instance in REPLWidget instance.");

    message = mainWindow.findChild<MessageWidget*>();
    QVERIFY2(message, "Could not find MessageWidget instance in MainWindow instance.");

    messageEdit = message->findChild<QLineEdit*>();
    QVERIFY2(messageEdit, "Could not find QLineEdit in MessageWidget instance.");

    canvas = mainWindow.findChild<CanvasWidget*>();
    QVERIFY2(canvas, "Could not find CanvasWidget instance in MainWindow instance.");

    scene = canvas->findChild<QGraphicsScene*>();
    QVERIFY2(scene, "Could not find QGraphicsScene instance in CanvasWidget instance.");

    mainWindow.setMinimumSize(800, 600);
    mainWindow.show();
}

void unittests_gui::testValidPoint() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "((0 0 point) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(scene->itemAt(QPointF(0, 0), QTransform()) != nullptr,
        "Expected a point in the scene. Not found.");
}

void unittests_gui::testInvalidCommands() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "(unknown_command)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message for unknown command.");
}

void unittests_gui::testIncompleteCommand() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "((0 0 point");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message for incomplete command.");
}

void unittests_gui::testTooManyArguments() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "((0 0 point) (10 10 point) (20 20 point) line)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message for too many arguments.");
}


void unittests_gui::testMissingArguments() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "((point))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message for missing arguments.");
}

void unittests_gui::testArithmeticErrors() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "(1 0 /)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message for division by zero.");
}

void unittests_gui::testRedefinitionErrors() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "((x 10 define) (x 20 define))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message for redefinition.");
}

void unittests_gui::cleanupTestCase() {
    mainWindow.close();
}




void unittests_gui::testUnsupportedType() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "((unsupported_type))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"),
        "Expected error message for unsupported type.");
}



void unittests_gui::testColorValidation() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "(((0 0 point) (10 10 point) rect) 256 0 0 fill_rect) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"),
        "Expected error message for invalid color values.");
}

void unittests_gui::testValidFillRectColors() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QTest::keyClicks(replEdit, "((((-10 -10 point) (10 10 point) rect) 0 255 0 fill_rect) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(scene->itemAt(QPointF(0, 0), QTransform()) != nullptr,
        "Expected a green filled rectangle in the scene. Not found.");
}

void unittests_gui::testCanvasClearing() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QTest::keyClicks(replEdit, "clear");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(scene->items().isEmpty(),
        "Expected the canvas to be cleared, but it still has items.");
}

void unittests_gui::testPointOutsideCanvas() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QTest::keyClicks(replEdit, "((10000 10000 point) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"),
        "Expected error message for point outside canvas boundaries.");
}

void unittests_gui::testHistoryNavigation() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "((0 0 point) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);
    QTest::keyClicks(replEdit, "((10 10 point) draw)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QTest::keyClick(replEdit, Qt::Key_Up, Qt::NoModifier);
    QCOMPARE(replEdit->text(), "((10 10 point) draw)");

    QTest::keyClick(replEdit, Qt::Key_Up, Qt::NoModifier);
    QCOMPARE(replEdit->text(), "((0 0 point) draw)");

    QTest::keyClick(replEdit, Qt::Key_Down, Qt::NoModifier);
    QCOMPARE(replEdit->text(), "((10 10 point) draw)");
}

void unittests_gui::testMalformedGraphicsCommands() {
    QVERIFY(repl && replEdit);

    QTest::keyClicks(replEdit, "((10 10 point) (20 20 point) line)) draw");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QVERIFY2(messageEdit->text().startsWith("Error"),
        "Expected error message for malformed graphics command.");
}




















QTEST_MAIN(unittests_gui)
#include "unittests_gui.moc"