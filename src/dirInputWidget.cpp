#include <QApplication>
#include "dirInputWidget.hpp"

DirInputWidget::DirInputWidget(QWidget* parent)
    : QWidget(parent)
{
    this->resize(327, 42);
    this->setWindowTitle(QApplication::translate("Form", "directory input dialog", nullptr));

    horizontalLayout = new QHBoxLayout(this);

    pushButton = new QPushButton(this);
    pushButton->setText("ok");
    lineEdit = new QLineEdit(this);
    
    horizontalLayout->addWidget(lineEdit);
    horizontalLayout->addWidget(pushButton);

    connect(pushButton, &QPushButton::clicked, this, &DirInputWidget::addDir);

    hide();
}

void DirInputWidget::addDir()
{
    std::string inputDir(lineEdit->text().toUtf8().data(), lineEdit->text().toUtf8().size());
    DIR* curdir = NULL;
    curdir = opendir(inputDir.c_str());
    bool ret = curdir == NULL?false : true;
    emit openDirRet(ret, inputDir);
    closedir(curdir);
    this->hide();
    lineEdit->clear();
}

void DirInputWidget::showInputDialog(int x, int y)
{
    QRect rect(x, y, this->width(), this->height());
    this->setGeometry(rect);
    this->update();
    this->show();
}