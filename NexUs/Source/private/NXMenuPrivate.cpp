#include "NXMenuPrivate.h"

#include <QAction>
#include <QLinearGradient>
#include <QModelIndex>
#include <QPropertyAnimation>

#include "NXMenu.h"

NXMenuPrivate::NXMenuPrivate(QObject *parent)
    : QObject { parent }
{
}

NXMenuPrivate::~NXMenuPrivate() { }
