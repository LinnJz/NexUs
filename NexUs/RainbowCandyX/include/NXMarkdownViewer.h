#ifndef NXMARKDOWNVIEWER_H
#define NXMARKDOWNVIEWER_H

#include <QWidget>

#include "NXProperty.h"

class NXMarkdownViewerPrivate;
class NX_EXPORT NXMarkdownViewer : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXMarkdownViewer)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Markdown)

  public:
    explicit NXMarkdownViewer(QWidget* parent = nullptr);
    ~NXMarkdownViewer() override;
};

#endif // NXMARKDOWNVIEWER_H
