#pragma once
#include <QDockWidget>
#include <QVBoxLayout>
#include <QComboBox>
class FeatureWidget:public QDockWidget
{
public:
	void SetView(QWidget* widget);
	QWidget* GetComboBox();
	FeatureWidget(QWidget* parent);
	~FeatureWidget();
private:
	QComboBox* m_pComboBox;
	QVBoxLayout* m_pVBoxLayout;
	QWidget* m_pWidget;

};

