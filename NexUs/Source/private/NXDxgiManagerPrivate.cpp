#include "NXDxgiManagerPrivate.h"
#ifdef Q_OS_WIN
NXDxgiManagerPrivate::NXDxgiManagerPrivate(QObject *parent)
    : QObject { parent }
{
}

NXDxgiManagerPrivate::~NXDxgiManagerPrivate() { }

NXDxgiScreenPrivate::NXDxgiScreenPrivate(QObject *parent)
    : QObject { parent }
{
}

NXDxgiScreenPrivate::~NXDxgiScreenPrivate() { }
#endif
