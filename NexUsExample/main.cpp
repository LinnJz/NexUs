#include <QApplication>
#include <QScreen>

#include "NXApplication.h"
#include "NXLog.h"
#include "mainwindow.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#endif
#include <QVBoxLayout>
#include "NXText.h"
#include "NXTabWidget.h"
int main(int argc, char* argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#else
  //根据实际屏幕缩放比例更改
  qputenv("QT_SCALE_FACTOR", "1.5");
#endif
#endif
  QApplication a(argc, argv);
  nxApp->init();

  //NXLog::getInstance()->initMessageLog(true);
  //MainWindow w;
  //w.show();
  QWidget w;
  w.setMaximumSize(1080, 720);
  w.setMinimumSize(400, 720);
  w.resize(1080, 720);
  QVBoxLayout* layout = new QVBoxLayout(&w);
  NXTabWidget* _tabWidget = new NXTabWidget(&w);
  _tabWidget->setFixedHeight(600);
  _tabWidget->setIsTabTransparent(true);
  _tabWidget->setTabBarStyle(NXTabBarType::Google);
  NXText *page1 = new NXText("新标签页", &w);
  page1->setTextPixelSize(32);
  page1->setAlignment(Qt::AlignCenter);
  _tabWidget->addTab(page1, QIcon(":/Resource/Image/Cirno.jpg"), "新标签页");
  for (int i = 0; i < 5; i++)
  {
    NXText *page = new NXText(QString("新标签页%1").arg(i), &w);
    page->setTextPixelSize(32);
    page->setAlignment(Qt::AlignCenter);
    _tabWidget->addTab(page, QString("新标签页%1").arg(i));
  }
  layout->addWidget(_tabWidget);
  w.show();
#ifdef Q_OS_WIN
  //    HWND handle = FindWindowA(NULL, "NXWidgetTool");
  //    if (handle != NULL)
  //    {
  //        SetWindowDisplayAffinity(handle, WDA_EXCLUDEFROMCAPTURE);
  //    }
#endif
  return a.exec();
}