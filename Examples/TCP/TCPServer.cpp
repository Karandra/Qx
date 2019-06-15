#include "TCPServer.h"

void TCPServer::OnConnection()
{
	while (m_TCPServer.hasPendingConnections())
	{
		if (QTcpSocket* socket = m_TCPServer.nextPendingConnection())
		{
			QueueEvent(TCPEvent::EvtConnected, *socket);
		}
	}
}

TCPServer::TCPServer(const QHostAddress& address, uint16_t port)
	:QxEvtHandler(this)
{
	m_TCPServer.listen(address, port);
	Qx::EventSystem::ConnectSignal(&m_TCPServer, &QTcpServer::newConnection, &TCPServer::OnConnection, this);
}
TCPServer::~TCPServer()
{
	Close();
}
