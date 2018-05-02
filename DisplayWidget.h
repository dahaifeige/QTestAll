#ifndef DockCameraWidget_H
#define DockCameraWidget_H
#pragma once
#include <QApplication>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QTime>
#include <QScrollArea>
#include "CameraCard.h"
class CameraWidget;
class InfoLayout;
class DisplayWidget :public QDockWidget
{
	Q_OBJECT
public:
	DisplayWidget();
	~DisplayWidget();
	CameraWidget* GetCameraWidget() { return m_pCameraWidget; }
	QSize GetScrollAreaSize() { return m_pScrollArea->size(); }
	void StartTimer();
	void StopTimer();
	void StartTimerPerS();
	void StopTimerPerS();
	std::shared_ptr<gcp2::CameraCard>  GetCameraCard() { return m_pCameraCard; }
	void SetCameraCard(std::shared_ptr<gcp2::CameraCard> pCameraCard) { m_pCameraCard = pCameraCard; }
	std::shared_ptr<gcp2::ICamera>  GetCamera() { return m_pCamera; }
	void SetCamera(std::shared_ptr<gcp2::ICamera> pCamera) { m_pCamera = pCamera; }
	QSize sizeHint() const override;
private slots :
	void FreshInfo();
	void FreshInfoPerS();
private:
	std::shared_ptr<gcp2::CameraCard> m_pCameraCard;
	std::shared_ptr<gcp2::ICamera> m_pCamera;
	
	std::unique_ptr<QWidget> m_pWidget;
	CameraWidget* m_pCameraWidget;
	std::unique_ptr<QScrollArea> m_pScrollArea;
	InfoLayout* m_pInfoLayout;
	QVBoxLayout* m_pVLayout;
	std::unique_ptr<QTimer>m_pTimer, m_pTimerPerS;
	std::unique_ptr<QTime> m_pTime;

};

#endif
