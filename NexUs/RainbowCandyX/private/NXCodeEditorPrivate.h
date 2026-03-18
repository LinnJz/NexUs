#ifndef NXCODEEDITORPRIVATE_H
#define NXCODEEDITORPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXCodePlainTextEdit;
class NXCodeEditor;
class NXCodeEditorPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXCodeEditor)
    Q_PROPERTY_CREATE_D(QString, Code)
    Q_PROPERTY_CREATE_D(bool, IsReadOnly)
    Q_PROPERTY_CREATE_D(int, TabSize)
public:
    explicit NXCodeEditorPrivate(QObject* parent = nullptr);
    ~NXCodeEditorPrivate() override;
    Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
    friend class NXCodeEditorLineArea;
    friend class NXCodeHighlighter;
    NXThemeType::ThemeMode _themeMode;
    int _language{0};
    NXCodePlainTextEdit* _editor{nullptr};
    QWidget* _lineArea{nullptr};
    void* _highlighter{nullptr};
};

#endif // NXCODEEDITORPRIVATE_H
