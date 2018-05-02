#include "CameraItem.h"
#include "BitArray.h"
#include "DisplayWidget.h"
#include "CameraWidget.h"
CameraItem::CameraItem():
	m_pBitArray(new BitArray),
	m_pDisplayWidget(new DisplayWidget)
{

}

CameraItem::CameraItem(QTreeWidgetItem * parent, int type) : QTreeWidgetItem(parent),
	m_pBitArray(new BitArray),
	m_pDisplayWidget(new DisplayWidget)
{
	m_pDisplayWidget->GetCameraWidget()->SetStatus(m_pBitArray.get());
}

CameraItem::~CameraItem()
{
}

std::shared_ptr<BitArray> CameraItem::GetStatus()
{
	return m_pBitArray;
}

std::shared_ptr<DisplayWidget> CameraItem::GetDisplayWidget()
{
	return m_pDisplayWidget;
}
