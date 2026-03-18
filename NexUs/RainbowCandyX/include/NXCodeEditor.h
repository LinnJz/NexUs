#ifndef NXCODEEDITOR_H
#define NXCODEEDITOR_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXCodeEditorPrivate;
class NX_EXPORT NXCodeEditor : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXCodeEditor)
    Q_PROPERTY_CREATE_H(bool, IsReadOnly)
    Q_PROPERTY_CREATE_H(int, TabSize)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Code)

  public:
    enum Language
    {
        CPP = 0,
        C,
        CSharp,
        Python,
        JavaScript,
        Lua,
        Rust,
        PHP
    };
    Q_ENUM(Language)

    explicit NXCodeEditor(QWidget* parent = nullptr);
    ~NXCodeEditor() override;

    void setLanguage(Language lang) noexcept;
    Language getLanguage() const noexcept;
};

#endif // NXCODEEDITOR_H
