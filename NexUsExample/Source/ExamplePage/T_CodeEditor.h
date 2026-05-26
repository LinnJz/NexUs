#ifndef T_CODEEDITOR_H
#define T_CODEEDITOR_H

#include "T_BasePage.h"

class NXCodeEditor;

class T_CodeEditor : public T_BasePage
{
  Q_OBJECT

public:
  Q_INVOKABLE explicit T_CodeEditor(QWidget *parent = nullptr);
  ~T_CodeEditor();

private:
  NXCodeEditor *_codeEditor { nullptr };
};

#endif // T_CODEEDITOR_H
