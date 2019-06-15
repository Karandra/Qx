#pragma once
#include <QNetworkDatagram>
#include <Qx/Core.hpp>

class UDPEvent: public QxNotifyEvent
{
	public:
		QxEVENT_DEFINE_SCM(Datagram, UDPEvent);

	private:
		QNetworkDatagram m_Datagram;

	public:
		UDPEvent(QNetworkDatagram datagram)
			:m_Datagram(std::move(datagram))
		{
		}

	public:
		const QNetworkDatagram& GetDatagram() const
		{
			return m_Datagram;
		}
		QNetworkDatagram& GetDatagram()
		{
			return m_Datagram;
		}

		QString GetTextMessage() const;
		QNetworkDatagram MakeReply(const QString& value);
};
