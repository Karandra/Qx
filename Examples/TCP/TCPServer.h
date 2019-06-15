#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include "TCPEvent.h"

class TCPServer: public QObject, public QxEvtHandler
{
	private:
		Q_OBJECT;
		QTcpServer m_TCPServer;

	private:
		void OnConnection();

	public:
		TCPServer(const QHostAddress& address, uint16_t port);
		~TCPServer();

	public:
		bool IsListening() const
		{
			return m_TCPServer.isListening();
		}
		void Close()
		{
			m_TCPServer.close();
		}
};
