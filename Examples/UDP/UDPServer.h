#pragma once
#include "UDPExchanger.h"

class UDPServer: public UDPExchanger
{
	public:
		UDPServer(const QHostAddress& address, uint16_t port)
			:UDPExchanger(address, port)
		{
			// Начинаем слушать события по указанному адресу и порту
			m_Socket.bind(address, port);
		}
};
