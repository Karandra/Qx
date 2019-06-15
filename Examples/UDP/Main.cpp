#include "MainWindow.h"
#include <Qx/Core.hpp>
#include <QApplication>

int main(int argc, char* argv[])
{
	QxApplication<QApplication> app(argc, argv);

	MainWindow mainWindow;
	mainWindow.show();

	return app.Run();
}
