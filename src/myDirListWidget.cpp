#include <set>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <dirent.h>
#include <unordered_map>
#include <condition_variable>

#include <QDir>
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>
#include <QProcess>

#include "myDirListWidget.hpp"

std::condition_variable cond_;
std::mutex mtx_;
bool dirCacheChanged_ = false;
bool stop_ = false;

MyDirListWidget::MyDirListWidget(QWidget* parent)
    : QListWidget(parent)
{
    tid_ = std::thread([this]() {
        this->syncDirList();
    });

    std::string binDirStr(QApplication::applicationDirPath().toStdString());
    dirListFile_ = (std::filesystem::path(binDirStr) / fileName_).string();
    std::cout << "dirListFile: " << dirListFile_ << "\n";

    connect(this, &MyDirListWidget::rightDoubleClicked, this, &MyDirListWidget::deleteDir);
    connect(this, &MyDirListWidget::leftDoubleClicked, this, &MyDirListWidget::openDir);

    initDirWithList();
}

MyDirListWidget::~MyDirListWidget()
{
    {
        std::unique_lock<std::mutex> lock(mtx_);
        stop_ = true;
        cond_.notify_one();
    }

    if (tid_.joinable())
        tid_.join();
    std::cout << "destruct \n";
}

void MyDirListWidget::addDirList()
{
    std::sort(dirWithFrence_.begin(), dirWithFrence_.end(), [](auto& tuple1, auto& tuple2) {
        const auto& [_0, _1, frence] = tuple1;
        const auto& [_2, _3, frence2] = tuple2;
        return frence > frence2;
    });

    auto accessIter  = dirWithFrence_.begin();
    while (accessIter != dirWithFrence_.end())
    {
        auto& [path, item, frence]  = *accessIter;
        if (frence == 0)
        {
            accessIter = dirWithFrence_.erase(accessIter);
            if (item)
            {
                this->takeItem(this->row(item));
                delete item;
            }
        }
        else 
            accessIter++;
    }

    int count = this->count();
    for (int i = count - 1; i >= 0; --i)
        this->takeItem(i);

    std::for_each(dirWithFrence_.begin(), dirWithFrence_.end(), [&](auto& itemPair)
    {
        auto& [_1, item, _2] = itemPair;
        this->addItem(item);
    });

    if (currentItem_)
        this->setCurrentItem(currentItem_);

    {
        std::unique_lock<std::mutex> lock(mtx_);
        dirCacheChanged_ = true;
        cond_.notify_one();
    }
}

void MyDirListWidget::initDirWithList()
{
    std::fstream ifs(dirListFile_, std::ios::in);
    if (not ifs.is_open())
        std::cerr << "Failed to create the file." << std::endl;
    ifs.seekg(0);

    std::string dirLine;
    while(std::getline(ifs, dirLine))
    {
        auto firstSharpPos = dirLine.find_first_of("#");
        auto secondSharpPos = dirLine.find_last_of("#");
        if (std::set({firstSharpPos, secondSharpPos}).count(std::string::npos))
            continue;

        std::string pathName = dirLine.substr(firstSharpPos + 1, secondSharpPos - firstSharpPos - 1);
        auto item = new QListWidgetItem(pathName.c_str(), this);
        item->setToolTip(pathName.c_str());
        uint32_t frequence = std::atoi(dirLine.substr(secondSharpPos + 1).c_str());

        dirWithFrence_.push_back(std::make_tuple(pathName.c_str(), item, std::max(frequence, (uint32_t)1)));
    }
    ifs.close();
    addDirList();
}

void MyDirListWidget::openDir(QListWidgetItem* clickedItem)
{
    int curPathNum = (int)dirWithFrence_.size();
    const auto clickedPath = clickedItem->text().toStdString();

    for (int i = 0; i < curPathNum; ++i)
    {
        auto& [path, item, frence] = dirWithFrence_[i];
        if (item == clickedItem)
        {
            currentItem_ = item;
            ++frence;

            QString program;

            #ifdef Q_OS_LINUX
            program = "xdg-open";
            #elif defined(Q_OS_WIND)
            program = "explorer";
            #else
                return;
            #endif

            std::cout << "cmd: " << program.toStdString() << " " << path <<"\n";

            QProcess *exePro = new QProcess(this);
            connect(exePro, &QProcess::errorOccurred, this, [exePro](QProcess::ProcessError error) {
                std::cerr << "QProcess error: " << error << ", state: " << exePro->state() << "\n";
            });

            connect(exePro, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), exePro, &QProcess::deleteLater);

            exePro->start("xdg-open", {path.string().c_str()});

            if (!exePro->waitForStarted()) {
                std::cerr << "Failed to start process\n";
                delete exePro;
            }
            break;
        }
    }
    addDirList();
}

void MyDirListWidget::deleteDir(QListWidgetItem* clickedItem)
{
    int curPathNum = (int)dirWithFrence_.size();
    const auto clickedPath = clickedItem->text().toStdString();

    for (int i = 0; i < curPathNum; ++i)
    {
        auto& [path, item, frence] = dirWithFrence_[i];
        if (item == clickedItem)
        {
            if (currentItem_ == item)
                currentItem_ = nullptr;
            frence = 0;
            break;
        }
    }
    addDirList();
}

void MyDirListWidget::saveDirList()
{
    std::ofstream ofs(dirListFile_);
    auto dirCache = dirWithFrence_;
    for (auto& [path, _, frence] : dirCache)
    {
        std::string tmpLine = "#";
        tmpLine += path;
        tmpLine += "#" + std::to_string(frence) + "\n";
        ofs.write(tmpLine.c_str(), tmpLine.size());
    }
    ofs.close();
}

void MyDirListWidget::syncDirList() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx_);
        cond_.wait(lock, [&](){
            return stop_ || dirCacheChanged_;
        });

        if (stop_) {
            return;
        }

        std::cout << "still ongoing...\n";

        dirCacheChanged_ = false;
        saveDirList();
    }
}

bool MyDirListWidget::addNewDir(const std::string inputDir)
{
    for (auto& [path, _, frence] : dirWithFrence_)
    {
        if (path == inputDir)
        {
            QMessageBox::information(NULL, "ok", "direcotry already exists!", QMessageBox::Ok);
            return true;
        }
    }

    auto item = new QListWidgetItem(inputDir.c_str(), this);
    item->setToolTip(inputDir.c_str());
    dirWithFrence_.push_back(std::make_tuple(inputDir, item, 1));
    addDirList();
    return true;
}

void MyDirListWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    auto item = this->itemAt(event->pos());
    if (item) {
        if(event->button() == Qt::RightButton) {
            emit rightDoubleClicked(item);
        }
        else if(event->button() == Qt::LeftButton) {
            emit leftDoubleClicked(item);
        }
    }
    QListWidget::mouseDoubleClickEvent(event);
}
