#include "DeviceWidget.h"
DeviceWidget::DeviceWidget(QWidget *parent)
	: QDockWidget(parent),
m_pWidget(new QWidget),
m_pTreeWidget(new QTreeWidget),
m_pLabel(new QLabel),
m_pCheckBox(new QCheckBox),
m_pPushButton(new QPushButton),
m_pHBoxLayout(new QHBoxLayout),
m_pVBoxLayout(new QVBoxLayout)
{
	setWindowTitle(QApplication::translate("MainWindow", "Devices", Q_NULLPTR));
	setAllowedAreas(Qt::LeftDockWidgetArea);
	setMinimumSize(QSize(400, 200));


	m_pTreeWidget->setMaximumHeight(200);
	m_pTreeWidget->header()->setVisible(false);
	m_pLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Tip<span style=\" font-style:italic;\">:Double-click a camera to open it</span></p></body></html>", Q_NULLPTR));
	m_pCheckBox->setText("Auto-Scan");
	m_pPushButton->setText("flush");

	m_pHBoxLayout->addWidget(m_pLabel);
	m_pHBoxLayout->addWidget(m_pCheckBox);
	m_pHBoxLayout->addWidget(m_pPushButton);	
	m_pHBoxLayout->setMargin(0);

	m_pVBoxLayout->addWidget(m_pTreeWidget);
	m_pVBoxLayout->addLayout(m_pHBoxLayout);
	m_pVBoxLayout->setMargin(0);

	m_pWidget->setLayout(m_pVBoxLayout);

	//m_pMainVBoxLayout = new QHBoxLayout;
	//m_pMainVBoxLayout->addWidget(m_pWidget);
	//m_pWidget->setMinimumSize(QSize(500, 200));
	//m_pWidget->resize(QSize(500, 200));
	setWidget(m_pWidget);
	//setLayout(m_pMainVBoxLayout);
}

DeviceWidget::~DeviceWidget()
{
	if (m_pTreeWidget)
		delete m_pTreeWidget;
	if (m_pLabel)
		delete m_pLabel;
	if (m_pCheckBox)
		delete m_pCheckBox;
	if (m_pPushButton)
		delete m_pPushButton;
	if (m_pHBoxLayout)
		delete m_pHBoxLayout;
	if (m_pVBoxLayout)
		delete m_pVBoxLayout;
	if (m_pWidget)
		delete m_pWidget;
}

QSize DeviceWidget::sizeHint() const
{
	//Don't work
	return QSize(0, 0);
}