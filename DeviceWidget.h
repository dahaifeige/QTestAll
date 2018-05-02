#pragma once
#include <QApplication>
#include <QDockWidget>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QHeaderView>
#include <memory>
class DeviceWidget : public QDockWidget
{
	Q_OBJECT

public:
	DeviceWidget(QWidget *parent);
	~DeviceWidget();
	QSize DeviceWidget::sizeHint() const override;
	QTreeWidget* GetTreeWidget() { return m_pTreeWidget; }
	QCheckBox* GetCheckBox() { return m_pCheckBox; }
	QPushButton* GetPushButton() { return m_pPushButton; }
private:
	QTreeWidget* m_pTreeWidget;
	QLabel* m_pLabel;
	QCheckBox* m_pCheckBox;
	QPushButton* m_pPushButton;
	QHBoxLayout* m_pHBoxLayout;
	QVBoxLayout* m_pVBoxLayout;
	QWidget* m_pWidget;
	QHBoxLayout* m_pMainVBoxLayout;

};
