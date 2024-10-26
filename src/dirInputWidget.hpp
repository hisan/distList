#ifndef DIR_INPUT_HPP
#define DIR_INPUT_HPP

#include <dirent.h>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QErrorMessage>
#include <QMessageBox>

class DirInputWidget: public QWidget
{
    Q_OBJECT
public:
    DirInputWidget(QWidget* parent = nullptr);
    void showInputDialog(int x, int y);
    void addDir();
signals:
    void openDirRet(bool opened, const std::string& openedDir);

private:
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
};

#endif //DIR_INPUT_HPP