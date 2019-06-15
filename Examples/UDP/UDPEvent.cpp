#include "UDPEvent.h"

QString UDPEvent::GetTextMessage() const
{
	QByteArray messageBinary = m_Datagram.data();
	QDataStream stream(&messageBinary, QIODevice::ReadOnly);

	QString message;
	stream >> message;
	return message;
}
QNetworkDatagram UDPEvent::MakeReply(const QString& value)
{
	QByteArray messageBinary;
	QDataStream out(&messageBinary, QIODevice::WriteOnly);
	out << value;

	return m_Datagram.makeReply(messageBinary);
}
