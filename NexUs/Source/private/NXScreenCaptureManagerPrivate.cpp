#include "NXScreenCaptureManagerPrivate.h"
#ifdef Q_OS_MAC
NXScreenCaptureManagerPrivate::NXScreenCaptureManagerPrivate(QObject *parent)
    : QObject { parent }
{
}

NXScreenCaptureManagerPrivate::~NXScreenCaptureManagerPrivate() { }

NXScreenCaptureScreenPrivate::NXScreenCaptureScreenPrivate(QObject *parent)
    : QObject { parent }
{
}

NXScreenCaptureScreenPrivate::~NXScreenCaptureScreenPrivate() { }
#endif
