#ifndef NXMARKDOWNVIEWERPRIVATE_H
#define NXMARKDOWNVIEWERPRIVATE_H

#include <QObject>

#include "NXDef.h"

class QTextBrowser;
class NXMarkdownViewer;
class NXMarkdownViewerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXMarkdownViewer)
    Q_PROPERTY_CREATE_D(QString, Markdown)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit NXMarkdownViewerPrivate(QObject* parent = nullptr);
    ~NXMarkdownViewerPrivate() override;

private:
    NXThemeType::ThemeMode _themeMode;
    QTextBrowser* _textBrowser{nullptr};
    void _applyThemeStyle();
};

#endif // NXMARKDOWNVIEWERPRIVATE_H
