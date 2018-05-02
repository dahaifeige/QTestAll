#include "DisplayWidget.h"
#include "InfoLayout.h"
#include "CameraWidget.h"
#include "Camera.h"
DisplayWidget::DisplayWidget():
m_pCameraCard(std::shared_ptr<gcp2::CameraCard> (NULL)),
m_pCamera(std::shared_ptr<gcp2::Camera> (NULL)),
m_pWidget(new QWidget),
m_pCameraWidget(new CameraWidget),
m_pScrollArea(new QScrollArea),
m_pInfoLayout(new InfoLayout),
m_pVLayout(new QVBoxLayout),
m_pTimer(new QTimer),
m_pTimerPerS(new QTimer),
m_pTime(new QTime(0,0))
{
	setWindowTitle(QApplication::translate("MainWindow", "Camera Display", Q_NULLPTR));
	setAllowedAreas(Qt::RightDockWidgetArea);

	
	m_pScrollArea->setWidget(m_pCameraWidget);
	m_pScrollArea->setHorizontalScrollBar(m_pCameraWidget->GetHScrollBar());
	m_pScrollArea->setVerticalScrollBar(m_pCameraWidget->GetVScrollBar());

	m_pVLayout->addWidget(m_pScrollArea.get());
	m_pVLayout->addLayout(m_pInfoLayout);
	m_pVLayout->setMargin(0);

	m_pWidget->setLayout(m_pVLayout);
	setWidget(m_pWidget.get());
	
	setMinimumSize(1024, 768);

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setFloating(true);

	
	connect(m_pTimer.get(), SIGNAL(timeout()), this, SLOT(FreshInfo()));//定时刷新信息
	connect(m_pTimerPerS.get(), SIGNAL(timeout()), this, SLOT(FreshInfoPerS()));//定时刷新信息

}

DisplayWidget::~DisplayWidget()
{
	if (m_pCameraWidget)
		delete m_pCameraWidget;
	if (m_pInfoLayout)
		delete m_pInfoLayout;
	if (m_pVLayout)
		delete m_pVLayout;
}

void DisplayWidget::FreshInfo()
{
	m_pCameraWidget->SetWnWidth(m_pScrollArea->width() - 20 + 1);
	m_pCameraWidget->SetWnHeight(m_pScrollArea->height() - 20 + 1);
	gcp2::Statistics  m_statistic = m_pCamera->GetStatistics(0);
	m_pInfoLayout->GetImageNumLabel()->setText(QString("<font size='+1'>ImagesCount: %1; ImagesErrCount: %2</font>").arg(m_statistic.imagesCount).arg(m_statistic.imagesErrCount));
	m_pInfoLayout->GetLostCountLabel()->setText(QString(" <font size='+1'>ImagesLostCount: %1; SegmentsLostCount: %2</font>").arg(m_statistic.imagesLostCount).arg(m_statistic.segmentsLostCount));
	m_pInfoLayout->GetResolutionLabel()->setText(QString("<font size='+1'>%1 x %2</font>").arg(m_pCameraWidget->GetImageWidth()).arg(m_pCameraWidget->GetImageHeight()));
	m_pInfoLayout->GetPosLabel()->setText(QString("<font size='+1'>X:%1, Y:%2</font>").arg(m_pCameraWidget->GetXPos()).arg(m_pCameraWidget->GetYPos()));
	if (m_pCameraWidget->GetFormat() == "MONO12")
		m_pInfoLayout->GetRGBLabel()->setText(QString("<font size='+1'>%1: GrayValue:%2</font>").arg(m_pCameraWidget->GetFormat()).arg(m_pCameraWidget->Get12BitValue()));
	else
		m_pInfoLayout->GetRGBLabel()->setText(QString("<font size='+1'> %1: RGB:<font color=red>%2</font> <font color=green>%3</font> <font color=blue>%4</font> </font>").arg(m_pCameraWidget->GetFormat()).arg(m_pCameraWidget->GetRgb().red()).arg(m_pCameraWidget->GetRgb().green()).arg(m_pCameraWidget->GetRgb().blue()));
	m_pInfoLayout->GetScaleLabel()->setText(QString("<font size='+1'>%1%</font>").arg(m_pCameraWidget->GetScale()));//<i>Hello</i><font color=red>Qt!</font></h2>
}

QSize DisplayWidget::sizeHint() const
{
	//Doesn't work
	return QSize(1280, 840);
}

void DisplayWidget::FreshInfoPerS()
{
	*m_pTime = m_pTime.get()->addSecs(1);
	m_pInfoLayout->GetTime()->setText(QString("<font size='+1'>Time:%1h %2m %3s</font>").arg(m_pTime.get()->hour()).arg(m_pTime.get()->minute()).arg(m_pTime.get()->second()));
	m_pInfoLayout->GetFrameLabel()->setText(QString("<font size='+1'>%1 fps (%2Mb/s)</font>").arg(m_pCameraWidget->GetFps(), 0, 'f', 1).arg(m_pCameraWidget->GetSpeed(), 0, 'f', 2));
}

void DisplayWidget::StartTimer()
{
	m_pTimer->start(0);
}

void DisplayWidget::StopTimer()
{
	m_pTimer->stop();
}

void DisplayWidget::StartTimerPerS()
{
	m_pTimerPerS->start(1000);
}

void DisplayWidget::StopTimerPerS()
{
	m_pTimerPerS->stop();
}

