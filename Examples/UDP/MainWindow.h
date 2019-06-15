#pragma once
#include <QMainWindow>
#include <QMessageBox>
#include <Qx/Core.hpp>

#include "UDPClient.h"
#include "UDPServer.h"

namespace Ui
{
	class MainWindow;
}

class MainWindow: public QMainWindow, public QxEvtHandler
{
	private:
		Q_OBJECT;
		std::unique_ptr<Ui::MainWindow> m_UI;

	private:
		std::unique_ptr<UDPClient> m_Client;
		std::unique_ptr<UDPServer> m_Server;
		uint16_t m_Port = 4001;

	private:
		void SetupUI();
		void SetupExchange();

		void OnSendToServerButton();
		void OnRecievedByServer(UDPEvent& event);
		void OnRecievedByClient(UDPEvent& event);

	public:
		MainWindow(QWidget* parent = nullptr);
		~MainWindow();
};
