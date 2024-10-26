#include <QApplication>
#include <QFileDialog>

#include "OperateWidget.hpp"

OperateWidget::OperateWidget(QWidget* parent)
    : dirInputWidget_(new DirInputWidget(nullptr))
    , QWidget(parent)
{
    openDirButton_ = new QPushButton(this);
    openDirButton_->setText("select directory");

    inputDirButton_ = new QPushButton(this);
    inputDirButton_->setText("input directory");

    connect(openDirButton_, &QPushButton::clicked, this, &OperateWidget::addDirBySelection);
    connect(inputDirButton_, &QPushButton::clicked, this, &OperateWidget::addDirByManual);
    connect(dirInputWidget_, &DirInputWidget::openDirRet, this, &OperateWidget::dealAfterInputDir);

    hLayout_ = new QHBoxLayout(this);
    hLayout_->addWidget(openDirButton_);
    hLayout_->addWidget(inputDirButton_);
}

void OperateWidget::dealAfterInputDir(bool opened, const std::string& inputedDir)
{
    std::string info("directory: ");
    info += inputedDir;

    if (!opened)
    {
        info += "do not exists,add failed!";
        QMessageBox::information(NULL, "error", info.c_str(), QMessageBox::Warning);
    }
    else
        emit selectNewDirSignal(inputedDir);
}

void OperateWidget::addDirBySelection()
{
    QString dir = QFileDialog::getExistingDirectory(this, "directory select", lastDir_.c_str(), 
        QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
      return;
    lastDir_ = dir.toStdString();
    emit selectNewDirSignal(lastDir_);
}

void OperateWidget::addDirByManual()
{
    int x = cursor().pos().x();
    int y = cursor().pos().y();
    dirInputWidget_->showInputDialog(x, y);
}