#include "FeatureWidget.h"
FeatureWidget::FeatureWidget(QWidget *parent) : QDockWidget(parent), 
m_pComboBox(new QComboBox()),
m_pVBoxLayout(new QVBoxLayout),
m_pWidget(new QWidget)
{
	setWindowTitle("Feature");
	setAllowedAreas(Qt::LeftDockWidgetArea);
	setMinimumSize(QSize(400, 600));
	m_pComboBox->addItems({ "Beginner","Expert","Guru","Invisible" });
	m_pComboBox->setCurrentIndex(0);

	m_pVBoxLayout->setMargin(0);
	m_pWidget->setLayout(m_pVBoxLayout);
	setWidget(m_pWidget);
}
void FeatureWidget::SetView(QWidget* widget)
{
	m_pVBoxLayout->addWidget(m_pComboBox);
	m_pVBoxLayout->addWidget(widget);
}

QWidget* FeatureWidget::GetComboBox()
{
	return m_pComboBox;
}

FeatureWidget::~FeatureWidget()
{
	if (m_pComboBox)
		delete m_pComboBox;
	if (m_pVBoxLayout)
		delete m_pVBoxLayout;
	if (m_pWidget)
		delete m_pWidget;
}
