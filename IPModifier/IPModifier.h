#pragma once

#include <QtWidgets/QDialog>
#include <QNetworkInterface>
#include "QtIPEdit.h"
#include <QLabel>
#include <QComboBox>
#include <QPushButton>


class IPModifier 
	: public QDialog
{
	Q_OBJECT

public:
	IPModifier(QWidget *parent = Q_NULLPTR);

private:
	void init();

private:
	void createLayout();
	void createConnections();
	void initNetCardNameCMB();

private slots:
	void updateIP(int index);
	void modifyIP();

private:
	QLabel* netCardNameLab_;
	QComboBox* netCardNameCMB_;
	QLabel* ipAddressLab_;
	QtIPEdit* ipEdit_;
	QPushButton* okPB_;
};
