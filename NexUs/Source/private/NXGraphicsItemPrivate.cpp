#include "NXGraphicsItemPrivate.h"

NXGraphicsItemPrivate::NXGraphicsItemPrivate(QObject *parent)
    : QObject(parent)
{
}

NXGraphicsItemPrivate::~NXGraphicsItemPrivate() { }

QDataStream& operator<<(QDataStream& stream, const NXGraphicsItemPrivate *data)
{
  stream << data->_itemUID;
  stream << data->_pItemName;
  stream << data->_pMaxLinkPortCount;
  stream << data->_currentLinkPortState;
  stream << data->_pDataRoutes;
  stream << data->_pItemImage;
  stream << data->_pItemSelectedImage;
  return stream;
}

QDataStream& operator>>(QDataStream& stream, NXGraphicsItemPrivate *data)
{
  stream >> data->_itemUID;
  stream >> data->_pItemName;
  stream >> data->_pMaxLinkPortCount;
  stream >> data->_currentLinkPortState;
  stream >> data->_pDataRoutes;
  stream >> data->_pItemImage;
  stream >> data->_pItemSelectedImage;
  return stream;
}
