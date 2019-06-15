#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <Qx/Core.hpp>

class TCPEvent: public QxNotifyEvent
{
	public:
		QxEVENT_DEFINE_SCM(Connected, TCPEvent);
		QxEVENT_DEFINE_SCM(Disconnected, TCPEvent);
		QxEVENT_DEFINE_SCM(Message, TCPEvent);

	private:
		QTime m_ConnectionTime;
		QTcpSocket* m_Socket = nullptr;

	private:
		void Init()
		{
			m_ConnectionTime = QTime::currentTime();
		}

	public:
		TCPEvent()
		{
			Init();
		}
		TCPEvent(QTcpSocket& socket)
			:m_Socket(&socket)
		{
			Init();
		}

	public:
		QTcpSocket& GetSocket() const
		{
			return *m_Socket;
		}
		QTime GetConnectionTime() const
		{
			return m_ConnectionTime;
		}
};
