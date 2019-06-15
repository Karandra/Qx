#include "TCPClient.h"

void TCPClient::OnMessage()
{
	QueueEvent(TCPEvent::EvtMessage, m_Socket);
}
void TCPClient::OnConnected()
{
	m_IsConnected = true;
	ProcessEvent(TCPEvent::EvtConnected, m_Socket);
}
void TCPClient::OnDisconnected()
{
	m_IsConnected = false;
	ProcessEvent(TCPEvent::EvtDisconnected, m_Socket);
}

TCPClient::TCPClient(const QHostAddress& address, uint16_t port)
	:QxEvtHandler(this), m_HostAddress(address), m_Port(port)
{
	Qx::EventSystem::ConnectSignal(&m_Socket, &QTcpSocket::connected, &TCPClient::OnConnected, this);
	Qx::EventSystem::ConnectSignal(&m_Socket, &QTcpSocket::disconnected, &TCPClient::OnDisconnected, this);
	Qx::EventSystem::ConnectSignal(&m_Socket, &QTcpSocket::readyRead, &TCPClient::OnMessage, this);
}
TCPClient::~TCPClient()
{
	Close();
}

void TCPClient::Connect()
{
	m_Socket.connectToHost(m_HostAddress, m_Port);
}
void TCPClient::ResetConnection()
{
	m_Socket.disconnectFromHost();
}
void TCPClient::Close()
{
	m_Socket.close();
}
