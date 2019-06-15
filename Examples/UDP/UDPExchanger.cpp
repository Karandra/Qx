#include "UDPExchanger.h"
#include <QHostInfo>

void UDPExchanger::OnRecieveDatagram()
{
	while (m_Socket.hasPendingDatagrams())
	{
		QNetworkDatagram datagram = m_Socket.receiveDatagram();
		if (datagram.isValid())
		{
			QueueEvent(UDPEvent::EvtDatagram, std::move(datagram));
		}
	}
}

UDPExchanger::UDPExchanger(const QHostAddress& address, uint16_t port)
	:QxEvtHandler(this), m_Socket(this), m_Address(address), m_Port(port)
{
	Qx::EventSystem::ConnectSignal(&m_Socket, &QUdpSocket::readyRead, &UDPExchanger::OnRecieveDatagram, this);
}

void UDPExchanger::SendDatagram(const QNetworkDatagram& datagram)
{
	m_Socket.writeDatagram(datagram);
}
void UDPExchanger::SendDatagram(const QString& message)
{
	QByteArray messageBinary;
	QDataStream out(&messageBinary, QIODevice::WriteOnly);
	out << message;

	m_Socket.writeDatagram(messageBinary, m_Address, m_Port);
}

QString UDPExchanger::GetHostName() const
{
	return QHostInfo::localHostName();
}
