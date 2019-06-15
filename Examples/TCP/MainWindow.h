#pragma once
#include <QMainWindow>
#include <QMessageBox>
#include <Qx/Core.hpp>
#include "TCPServer.h"
#include "TCPClient.h"
#include "TCPEvent.h"

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
		std::unique_ptr<TCPServer> m_Server;
		std::unique_ptr<TCPClient> m_Client;
		uint16_t m_Port = 5824;

	private:
		std::unique_ptr<TCPServer> CreateServer();
		void OnServerSendButton();
		void OnServerConnection(TCPEvent& event);

	private:
		std::unique_ptr<TCPClient> CreateClient();
		void OnClientConnectButton();

		void OnClientConnected(TCPEvent& event);
		void OnClientDisconnected(TCPEvent& event);
		void OnClientMessage(TCPEvent& event);

	public:
		MainWindow(QWidget* parent = nullptr);
		~MainWindow();
};
