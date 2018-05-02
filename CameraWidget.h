#pragma once
#ifndef WIDGET_H
#define WIDGET_H

//#define ISP
#include <QLabel>
#include <QScrollBar>
#include <QMouseEvent>
#include <QPainter>
#include <QBuffer>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <memory>
#include <mutex>
#include <deque>
#include <windows.h>
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

class QTest;
class BitArray;
namespace gcp2 {
	class Image;
}
class CameraWidget : public QLabel
{
	Q_OBJECT
public:
	CameraWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	~CameraWidget();

	void CallBack(std::shared_ptr<gcp2::Image> image);
	void SaveImage(QImage image);
	//get status
	inline void SetStatus(BitArray* pBitArray) { m_pBitArray = pBitArray; }
	//image operation
	inline void SetZoomScale(float scale) { m_Scale = scale; } //设置图像缩放倍数
	// display window size of image
	inline int GetWnWidth() { return m_nWnWidth; }
	inline void SetWnWidth(int width) { m_nWnWidth = width; } //设置图像显示窗口宽度
	inline int GetWnHeight() { return m_nWnHeight; }
	inline void SetWnHeight(int height) { m_nWnHeight = height; } //设置图像显示窗口高度
	//image's size
	int GetImageWidth() { return m_nImageWidth; }
	int GetImageHeight() { return m_nImageHeight; }
	//scrollbar
	inline QScrollBar* GetVScrollBar() { return m_pVScrollBar.get(); }
	inline QScrollBar* GetHScrollBar() { return m_pHScrollBar.get(); }
	//display info
	inline int GetXPos() { return m_xPos; }
	inline int GetYPos() { return m_yPos; }
	inline double GetFps() { return m_fFps; }
	inline float GetSpeed() { return m_fSpeed; }
	inline float GetTotalFrames() { return m_fCount; }
	inline QColor GetRgb() { return m_Color; }
	inline uint Get12BitValue() { return m_12BitValue; }
	inline QString GetFormat() { return m_sFormat; }
	inline float GetScale() { return m_Scale; }

#ifdef  ISP
	inline std::shared_ptr<QVector<double>> GetXValue1() { return m_xValue1; }
	inline std::shared_ptr<QVector<double>> GetXValue2() { return m_xValue2; }
	inline std::shared_ptr<QVector<double>> GetYValue1() { return m_yValue1; }
	inline std::shared_ptr<QVector<double>> GetYValue2() { return m_yValue2; }
#endif
public slots:
	void GetVValue(int value);
	void GetHValue(int value);
	void Fps();
	void paintEvent(QPaintEvent * event);
signals:
	void ReadyDisplay(QPaintEvent*);
private:
	void mouseMoveEvent(QMouseEvent *event);
	//void mousePressEvent(QMouseEvent *event);
	//void mouseReleaseEvent(QMouseEvent *event);
private:
	int m_nImageWidth, m_nImageHeight;
	int m_nWnWidth, m_nWnHeight;
	std::shared_ptr<QTest> m_pAlgorithm;
	QColor m_Color;
	std::shared_ptr<gcp2::Image> m_pImage;
	QImage m_SrcImage;
	float m_Scale;
	std::shared_ptr<QScrollBar> m_pVScrollBar, m_pHScrollBar;
	int m_nXLeftTop, m_nYLeftTop;
	float m_nHSliderValue, m_nVSliderValue;
	std::unique_ptr<QTimer> m_pTimer;
	std::deque<float> m_queue;
	double m_fFrame, m_fCount, m_fPreCount, m_fFps;
	double m_fSpeed;
	int m_xPos, m_yPos;
	float m_xTempPos, m_yTempPos;
	QString m_sFormat;
	uint m_12BitValue;
	std::mutex m_Mutex;
	BitArray* m_pBitArray;
#ifdef ISP
	QTest* m_pTest;
	QVector<double> *tempXValue1, *tempXValue2;
	QVector<double> *tempYValue1, *tempYValue2;

	std::shared_ptr<QVector<double>> m_xValue1, m_xValue2;
	std::shared_ptr<QVector<double>> m_yValue1, m_yValue2;

#endif // ISP


};

#endif // WIDGET_H
