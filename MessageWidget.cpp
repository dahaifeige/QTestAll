#include "MessageWidget.h"
#include <QHeaderView>
MessageWidget::MessageWidget(QWidget *parent)
	: QDockWidget(parent),
m_pTabWidget(new QTabWidget),
m_pTableWidget(new QTableWidget),
m_pFeatureWidget(new QWidget)
{
	setWindowTitle(QApplication::translate("MainWindow", "Message Log", Q_NULLPTR));
	setAllowedAreas(Qt::BottomDockWidgetArea);
	m_pTableWidget->setColumnCount(4);
	QStringList header_log;
	header_log << "Level" << "Time" << "Source" << "Message";
	m_pTableWidget->setHorizontalHeaderLabels(header_log);
	m_pTableWidget->horizontalHeader()->setStretchLastSection(true);

	m_pTabWidget->addTab(m_pTableWidget.get(), "Message Log");
	m_pTabWidget->addTab(m_pFeatureWidget.get(), "Feature Documentation");
	m_pTabWidget->setTabPosition(QTabWidget::South);

	setWidget(m_pTabWidget.get());
}

MessageWidget::~MessageWidget()
{
}
