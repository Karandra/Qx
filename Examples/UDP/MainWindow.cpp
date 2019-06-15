#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>

void MainWindow::SetupUI()
{
	m_UI->setupUi(this);

	Qx::EventSystem::ConnectSignal(m_UI->SendToServerButton, &QPushButton::clicked, &MainWindow::OnSendToServerButton, this);
}
void MainWindow::SetupExchange()
{
	m_Client = std::make_unique<UDPClient>(QHostAddress::Broadcast, m_Port);
	m_Client->Bind(UDPEvent::EvtDatagram, &MainWindow::OnRecievedByClient, this);

	m_Server = std::make_unique<UDPServer>(QHostAddress::Any, m_Port);
	m_Server->Bind(UDPEvent::EvtDatagram, &MainWindow::OnRecievedByServer, this);
}

void MainWindow::OnSendToServerButton()
{
	QInputDialog dialog(this);
	dialog.setLabelText("Enter data to send to the UDP server");
	dialog.setTextValue("UDP Server");
	
	if (dialog.exec() == QDialog::Accepted)
	{
		QString text = dialog.textValue();
		m_Client->SendDatagram(text);
	}
}
void MainWindow::OnRecievedByServer(UDPEvent& event)
{
	const QNetworkDatagram& datagram = event.GetDatagram();
	QString text = event.GetTextMessage();

	if (text.contains("UDP Server", Qt::CaseInsensitive))
	{
		QHostAddress senderAddress = datagram.senderAddress();
		QHostAddress ip4Address = QHostAddress(senderAddress.toIPv4Address());
		m_UI->ServerTextBox->append(ip4Address.toString());

		QNetworkDatagram reply = event.MakeReply(m_Server->GetHostName());
		m_Server->SendDatagram(reply);
	}
	else
	{
		m_UI->ServerTextBox->append(text);
	}
}
void MainWindow::OnRecievedByClient(UDPEvent& event)
{
	m_UI->ClientTextBox->append(event.GetTextMessage());
}

MainWindow::MainWindow(QWidget* parent)
	:QMainWindow(parent), QxEvtHandler(this), m_UI(std::make_unique<Ui::MainWindow>())
{
	SetupUI();
	SetupExchange();
}
MainWindow::~MainWindow()
{
}
