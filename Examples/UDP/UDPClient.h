#pragma once
#include "UDPExchanger.h"

class UDPClient: public UDPExchanger
{
	public:
		UDPClient(const QHostAddress& address, uint16_t port)
			:UDPExchanger(address, port)
		{
		}
};
