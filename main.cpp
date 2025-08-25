#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  // This MUST come first!

    app.setApplicationName("File Manager");
    app.setApplicationVersion("1.0");

    MainWindow window;  // Only create widgets AFTER QApplication
    window.show();

    return app.exec();
}
