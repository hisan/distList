#include "MainWindow.hpp"

MainWindow::MainWindow()
{
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowIcon(QIcon(std::string(":/images/dirIcon.png").c_str()));
    pCentralWidget = new QWidget(this);
    this->setCentralWidget(pCentralWidget);
    pCentralWidget->setMinimumSize(330, 500);
    pCentralWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    pCentralWidget->setWindowTitle("Dir Cache Tools");

    initUi();
}

void MainWindow::initUi()
{
    operateWidget_ = new OperateWidget(pCentralWidget);

    line = new QFrame(pCentralWidget);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    dirListWidget_ = new MyDirListWidget(pCentralWidget);

    vLayout_ = new QVBoxLayout(pCentralWidget);

    vLayout_->addWidget(operateWidget_);
    vLayout_->addWidget(line);
    vLayout_->addWidget(dirListWidget_);

    connect(operateWidget_, &OperateWidget::selectNewDirSignal, dirListWidget_, &MyDirListWidget::addNewDir);
}