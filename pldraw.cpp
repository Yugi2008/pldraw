#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


#include <QApplication>
#include "main_window.hpp"

#include "interpreter.hpp"
#include "interpreter_semantic_error.hpp"


// used outline from postlisp
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Interpreter interpreter;
    if (argc == 1) { // REPL 
        MainWindow window;
        window.setMinimumSize(800, 600); // set min size window requirement
        window.show();
        return app.exec();
    }
    else if (argc == 2) { // execute file
        std::string filename = argv[1];
        QFile file(QString::fromStdString(filename));
        if (!file.exists()) { // if file doesnr exist 
            std::cerr << "Error: File does not exist: " << filename << "\n";
            return EXIT_FAILURE;
        }

        MainWindow window(filename);
        window.setMinimumSize(800, 600); // set min size window requirement
        window.show();
        return app.exec();
    }
    else if (argc == 3 && std::string(argv[1]) == "-e") {// -e  from command line
        std::string expression = argv[2];
        Interpreter interpreter;

        std::istringstream iss(expression);
        if (!interpreter.parse(iss)) { // if parse fails
            std::cerr << "Error: Failed to parse expression\n";
            return EXIT_FAILURE;
        }

        try {
            Expression result = interpreter.eval();
            std::cout << result << std::endl;
        }
        catch (const InterpreterSemanticError& e) { 
            std::cerr << "Error: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
    else { // same format from last time
        std::cerr << "Error: Invalid arguments\n";
        std::cerr << "Usage:\n";
        std::cerr << "  pldraw                 Start GUI mode\n";
        std::cerr << "  pldraw <filename>      Open file in GUI\n";
        std::cerr << "  pldraw -e \"<expr>\"     Execute expression from command line\n";
        return EXIT_FAILURE;
    }
}

   