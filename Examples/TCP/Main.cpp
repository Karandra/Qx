#include "MainWindow.h"
#include <QApplication>
#include <Qx/Core.hpp>

int main(int argc, char* argv[])
{
	QxApplication<QApplication> app(argc, argv);

	MainWindow mainWindow;
	mainWindow.show();

	return app.Run();
}
