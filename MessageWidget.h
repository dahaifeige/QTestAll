#pragma once

#include <QDockWidget>
#include <QApplication>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <memory>
class MessageWidget : public QDockWidget
{
	Q_OBJECT

public:
	MessageWidget(QWidget *parent);
	~MessageWidget();
private:
	std::unique_ptr<QTabWidget> m_pTabWidget;
	std::unique_ptr<QTableWidget> m_pTableWidget;
	std::unique_ptr<QWidget> m_pFeatureWidget;
};
