#ifndef CAMITEM_H
#define CAMITEM_H
#pragma once
#include <QTreeWidgetItem>
#include <memory>
class BitArray;
class DisplayWidget;
class CamItemImp;
class CameraItem :public QTreeWidgetItem
{
public:
	CameraItem();
	CameraItem(QTreeWidgetItem *parent, int type = Type);
	~CameraItem();
	std::shared_ptr<BitArray> GetStatus();
	std::shared_ptr<DisplayWidget> GetDisplayWidget();
private:
	std::shared_ptr<BitArray> m_pBitArray;
	std::shared_ptr<DisplayWidget> m_pDisplayWidget;
};
#endif