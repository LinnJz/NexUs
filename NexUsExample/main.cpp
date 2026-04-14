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
#include "NXPivot.h"

QList<QPixmap> splitSpriteHorizontally(const QString& filePath)
{
  QPixmap original(filePath);
  if (original.isNull())
  {
    // 加载失败处理
    return QList<QPixmap>();
  }

  int totalWidth = original.width();
  int height     = original.height();
  int partWidth  = totalWidth / 3; // 三等分宽度

  QList<QPixmap> parts;
  for (int i = 0; i < 3; ++i)
  {
    QPixmap part = original.copy(i * partWidth, 0, partWidth, height);
    parts.append(part);
  }
  return parts;
}

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
  MainWindow w;
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
