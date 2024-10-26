#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    auto dirMange = new MainWindow();
    dirMange->show();
    app.exec();
    return 0;
}