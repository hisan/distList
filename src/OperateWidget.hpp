#ifndef __OPERATE_WIDGET_HPP__
#define __OPERATE_WIDGET_HPP__

#include <QPushButton>
#include <QHBoxLayout>
#include "dirInputWidget.hpp"

class OperateWidget: public QWidget
{
    Q_OBJECT
public:
    OperateWidget(QWidget* parent = nullptr);
public slots:
    void dealAfterInputDir(bool opened, const std::string& inputedDir);

signals:
    void selectNewDirSignal(const std::string selectedDir);

private:
    void addDirBySelection();
    void addDirByManual();

    std::string lastDir_ = "/home";

    QHBoxLayout* hLayout_;
    QPushButton *inputDirButton_;
    QPushButton *openDirButton_;
    DirInputWidget* dirInputWidget_;
};

#endif//__OPERATE_WIDGET_HPP__
