#include <QApplication>

#include "src/gui/main_window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MainWindow mainWindow;

    mainWindow.showFullScreen();

    return app.exec();
}
