#ifndef __MY_DIR_LIST_WIDGET_HPP__
#define __MY_DIR_LIST_WIDGET_HPP__

#include <vector>
#include <string>
#include <tuple>
#include <memory>
#include <iostream>
#include <thread>
#include <filesystem>
#include <unordered_map>

#include <QApplication>
#include <QMouseEvent>
#include <QListWidget>

class MyDirListWidget: public QListWidget
{
    Q_OBJECT
public:
    MyDirListWidget(QWidget* parent);
    ~MyDirListWidget();
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void initDirWithList();
    void addDirList();
    void saveDirList();
    void syncDirList();

signals:
    void rightDoubleClicked(QListWidgetItem*);
    void leftDoubleClicked(QListWidgetItem*);

public slots:
    bool addNewDir(const std::string inputDir);
    void openDir(QListWidgetItem*);
    void deleteDir(QListWidgetItem*);

private:
    QListWidgetItem* currentItem_ = nullptr;
    std::thread tid_;
    std::string dirListFile_;
    const std::string fileName_ = "dirList.txt";
    std::vector<std::tuple<std::filesystem::path, QListWidgetItem*, int>> dirWithFrence_;
};

#endif//__MY_DIR_LIST_WIDGET_HPP__
