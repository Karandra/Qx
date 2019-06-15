#pragma once
#include <QUdpSocket>
#include <QNetworkDatagram>
#include "UDPEvent.h"

class UDPExchanger: public QObject, public QxEvtHandler
{
	protected:
		QUdpSocket m_Socket;
		QHostAddress m_Address;
		uint16_t m_Port = 0;

	private:
		void OnRecieveDatagram();

	public:
		UDPExchanger(const QHostAddress& address, uint16_t port);

	public:
		void SendDatagram(const QNetworkDatagram& datagram);
		void SendDatagram(const QString& message);

		QString GetHostName() const;
};
