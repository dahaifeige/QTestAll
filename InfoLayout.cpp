#include "InfoLayout.h"

InfoLayout::InfoLayout():
m_pFrameLabel(new QLabel),
m_pImageNumLabel(new QLabel),
m_pLostCountLabel(new QLabel),
m_pResolutionLabel(new QLabel),
m_pPosLabel(new QLabel),
m_pRGBLabel(new QLabel),
m_pScaleLabel(new QLabel),
m_pTimeLabel(new QLabel)
{
	//pFrameLabel->setText("Information");

	m_pFrameLabel->setAlignment(Qt::AlignHCenter);
	m_pImageNumLabel->setAlignment(Qt::AlignHCenter);
	m_pLostCountLabel->setAlignment(Qt::AlignHCenter);
	m_pResolutionLabel->setAlignment(Qt::AlignHCenter);
	m_pPosLabel->setAlignment(Qt::AlignHCenter);
	m_pRGBLabel->setAlignment(Qt::AlignHCenter);
	m_pScaleLabel->setAlignment(Qt::AlignHCenter);
	m_pTimeLabel->setAlignment(Qt::AlignHCenter);

	addWidget(m_pFrameLabel.get());
	addWidget(m_pImageNumLabel.get());
	addWidget(m_pLostCountLabel.get());
	addWidget(m_pResolutionLabel.get());
	addWidget(m_pPosLabel.get());
	addWidget(m_pRGBLabel.get());
	addWidget(m_pScaleLabel.get());
	addWidget(m_pTimeLabel.get());
}

InfoLayout::~InfoLayout()
{
	qDebug() << "InfoLayout Destruct";
}