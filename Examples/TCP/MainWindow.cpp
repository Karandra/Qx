#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>

namespace
{
	QString AddressToString(const QHostAddress& address)
	{
		return QHostAddress(address.toIPv4Address()).toString();
	}
}

std::unique_ptr<TCPServer> MainWindow::CreateServer()
{
	auto server = std::make_unique<TCPServer>(QHostAddress::Any, m_Port);
	server->Bind(TCPEvent::EvtConnected, &MainWindow::OnServerConnection, this);

	return server;
}
void MainWindow::OnServerSendButton()
{
	if (!m_Server)
	{
		m_Server = CreateServer();
	}

	if (!m_Server->IsListening())
	{
		qDebug() << "Server error";
		m_Server.reset();
	}
}

void MainWindow::OnServerConnection(TCPEvent& event)
{
	QTcpSocket& socket = event.GetSocket();

	QString address = AddressToString(socket.peerAddress());
	QString time = event.GetConnectionTime().toString();

	QByteArray streamData;
	QDataStream stream(&streamData, QIODevice::WriteOnly);

	stream << uint16_t(0);
	stream << time;

	stream.device()->seek(0);
	stream << uint16_t(streamData.size() - sizeof(uint16_t));
	socket.write(streamData);

	m_UI->ServerLog->append(QString("New connection, time %1, IP-address: %2").arg(time).arg(address));
}

std::unique_ptr<TCPClient> MainWindow::CreateClient()
{
	auto client = std::make_unique<TCPClient>(QHostAddress::LocalHost, m_Port);

	client->Bind(TCPEvent::EvtConnected, &MainWindow::OnClientConnected, this);
	client->Bind(TCPEvent::EvtDisconnected, &MainWindow::OnClientDisconnected, this);
	client->Bind(TCPEvent::EvtMessage, &MainWindow::OnClientMessage, this);

	return client;
}
void MainWindow::OnClientConnectButton()
{
	if (!m_Client)
	{
		m_Client = CreateClient();
	}
	m_Client->Connect();
}

void MainWindow::OnClientConnected(TCPEvent& event)
{
	QTcpSocket& socket = event.GetSocket();
	QString address = AddressToString(socket.peerAddress());

	m_UI->ClientLog->append(QString("Connected to %1").arg(address));
}
void MainWindow::OnClientDisconnected(TCPEvent& event)
{
	QTcpSocket& socket = event.GetSocket();
	QString address = AddressToString(socket.peerAddress());

	m_UI->ClientLog->append(QString("Disconnected from %1").arg(address));
}
void MainWindow::OnClientMessage(TCPEvent& event)
{
	QTcpSocket& socket = event.GetSocket();
	QDataStream stream(&socket);

	if (socket.bytesAvailable() > sizeof(uint16_t))
	{
		uint16_t length = 0;
		stream >> length;

		if (length != 0)
		{
			QString message;
			stream >> message;

			m_UI->ClientLog->append(QString("Connection time: %1").arg(message));
		}
	}

	m_Client->ResetConnection();
}

//////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget* parent)
	:QMainWindow(parent), QxEvtHandler(this), m_UI(std::make_unique<Ui::MainWindow>())
{
	m_UI->setupUi(this);

	Qx::EventSystem::ConnectSignal(m_UI->ServerSendButton, &QPushButton::clicked, &MainWindow::OnServerSendButton, this);
	Qx::EventSystem::ConnectSignal(m_UI->ClientConnectButton, &QPushButton::clicked, &MainWindow::OnClientConnectButton, this);
}
MainWindow::~MainWindow()
{
}
