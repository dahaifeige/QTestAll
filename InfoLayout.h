#ifndef INFOLAYOUT_H
#define INFOLAYOUT_H
#pragma once
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <memory>
class InfoLayout :public QHBoxLayout
{
public:
	explicit InfoLayout();
	QLabel* GetFrameLabel() { return m_pFrameLabel.get(); }
	QLabel* GetImageNumLabel() { return m_pImageNumLabel.get(); }
	QLabel* GetLostCountLabel() { return m_pLostCountLabel.get(); }
	QLabel* GetResolutionLabel() { return m_pResolutionLabel.get(); }
	QLabel* GetPosLabel() { return m_pPosLabel.get(); }
	QLabel* GetRGBLabel() { return m_pRGBLabel.get(); }
	QLabel* GetScaleLabel() { return m_pScaleLabel.get(); }
	QLabel* GetTime() { return m_pTimeLabel.get(); }

	int GetWidth() { return m_pFrameLabel->width(); }
	int GetHeight() { return m_pFrameLabel->height(); }
	~InfoLayout();
private:
	std::unique_ptr<QLabel> m_pFrameLabel, m_pImageNumLabel, m_pLostCountLabel, m_pResolutionLabel, m_pPosLabel, m_pRGBLabel, m_pScaleLabel, m_pTimeLabel;
};
#endif