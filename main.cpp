#include <QApplication>
#include "mainwindow.h"
#include "core/Logger.h"
#ifdef _WIN32
#include <windows.h>
#include <iostream>
#include <fcntl.h>
#endif

int main(int argc, char *argv[])
{
    // 初始化日志系统，设置级别为 DEBUG（与控制台版本一致）
    Logger::initialize(Logger::Level::DEBUG);
    #ifdef _WIN32
        AllocConsole();
        FILE* fDummy;
        freopen_s(&fDummy, "CONOUT$", "w", stdout);
        freopen_s(&fDummy, "CONOUT$", "w", stderr);
        std::cout.clear();
        std::cerr.clear();
    #endif
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}