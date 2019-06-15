#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include "TCPEvent.h"

class TCPClient: public QObject, public QxEvtHandler
{
	private:
		Q_OBJECT;

	private:
		QTcpSocket m_Socket;
		QHostAddress m_HostAddress;
		uint16_t m_Port;
		bool m_IsConnected = false;

	private:
		void OnMessage();
		void OnConnected();
		void OnDisconnected();

	public:
		TCPClient(const QHostAddress& address, uint16_t port);
		~TCPClient();

	public:
		void Connect();
		void ResetConnection();
		void Close();

		bool IsConnedted() const
		{
			return m_IsConnected;
		}
};
