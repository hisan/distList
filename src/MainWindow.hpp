#ifndef OPEN_DIR_WIDGET_HPP_
#define OPEN_DIR_WIDGET_HPP_

#include <QMainWindow>
#include <QFrame>
#include <QVBoxLayout>

#include "OperateWidget.hpp"
#include "myDirListWidget.hpp"

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    void initUi();

private:
    int baseWidth = 330;
    int baseHeight = 80;

    QWidget *pCentralWidget = nullptr;
    QVBoxLayout *vLayout_;

    OperateWidget* operateWidget_;
    QFrame *line;
    MyDirListWidget* dirListWidget_;
};

#endif //OPEN_DIR_WIDGET_HPP_