#include "IPModifier.h"
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QGridLayout>


IPModifier::IPModifier(QWidget *parent)
	: QDialog(parent)
	, netCardNameLab_(new QLabel(tr("Net Card Name:"), this))
	, netCardNameCMB_(new QComboBox(this))
	, ipAddressLab_(new QLabel(tr("IP Address:"), this))
	, ipEdit_(new QtIPEdit(this))
	, okPB_(new QPushButton(tr("OK"), this))
{
	init();
}

void IPModifier::init()
{
	createLayout();
	createConnections();

	initNetCardNameCMB();
	setWindowIcon(QIcon(":/Image/IP.png"));
}

void IPModifier::createLayout()
{
	QGridLayout* mainLayout = new QGridLayout(this);
	mainLayout->addWidget(netCardNameLab_, 0, 0);
	mainLayout->addWidget(netCardNameCMB_, 0, 1);
	mainLayout->addWidget(ipAddressLab_, 1, 0);
	mainLayout->addWidget(ipEdit_, 1, 1);

	okPB_->setFixedSize(75, 25);
	mainLayout->addWidget(okPB_, 2, 1, Qt::AlignRight);

	resize(300, 100);
	setLayout(mainLayout);
}

void IPModifier::createConnections()
{
	bool okConnect = connect(netCardNameCMB_, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIP(int)));
	Q_ASSERT(okConnect);
	okConnect = connect(okPB_, &QPushButton::clicked, this, &IPModifier::modifyIP);
	Q_ASSERT(okConnect);
}

void IPModifier::initNetCardNameCMB()
{
	QString text;
	auto networkIFaces = QNetworkInterface::allInterfaces();

	for (int i = 0; i < networkIFaces.count(); i++)
	{
		text = networkIFaces.at(i).humanReadableName();
		netCardNameCMB_->addItem(text);
	}
}

void IPModifier::updateIP(int index)
{
	auto networkIFaces = QNetworkInterface::allInterfaces();

	if (!networkIFaces.at(index).addressEntries().empty())
	{
		ipEdit_->setIP(networkIFaces.at(index).addressEntries().at(0).ip().toString());
	}
	else
	{
		ipEdit_->setIP(QString());
	}
}

void IPModifier::modifyIP()
{
	int index = netCardNameCMB_->currentIndex();
	QString modifiedIp = ipEdit_->getIP();
	QString defaultGateway = modifiedIp.left(modifiedIp.lastIndexOf(".")) + ".1";
	auto networkIFaces = QNetworkInterface::allInterfaces();

	QString command = "netsh interface ipv4 set address name = "+ networkIFaces.at(index).humanReadableName()
		+ " source = static address = " + modifiedIp + " mask = " 
		+ networkIFaces.at(index).addressEntries().at(0).netmask().toString() + " "
		+ defaultGateway;

	QProcess cmd(this);
	cmd.start(command);
	cmd.waitForFinished();
	
	if (cmd.exitCode() == QProcess::CrashExit)
	{
		QMessageBox::warning(this, tr("Modify Failed")
			, tr("IP address modification failed, please confirm the IP address is valid!"));
	}
	else
	{
		QMessageBox::information(this, tr("Modify Success"), tr("IP has been set to: %1").arg(modifiedIp));
	}

	updateIP(index);
}
