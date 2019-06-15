#include "CoreApplication.h"
#include <QtCore>

namespace
{
	static Qx::Application::QxCoreApplication* g_AppInstance = nullptr;
}

QxCoreApplication* QxCoreApplication::GetInstance()
{
	return g_AppInstance;
}
QCoreApplication* QxCoreApplication::GetQCoreApplicationInstance()
{
	return QCoreApplication::instance();
}

QxCoreApplication::QxCoreApplication(QCoreApplication& qCoreApp)
	:QxEvtHandler(&qCoreApp), m_QCoreApp(&qCoreApp)
{
	g_AppInstance = this;
}
QxCoreApplication::~QxCoreApplication()
{
	g_AppInstance = nullptr;
}

bool QxCoreApplication::Yield()
{
	QCoreApplication::processEvents();
	return true;
}
int QxCoreApplication::MainLoop()
{
	return QCoreApplication::exec();
}
void QxCoreApplication::ExitMainLoop(int exitCode = 0)
{
	return QCoreApplication::exit(exitCode);
}
bool QxCoreApplication::UsesEventLoop() const
{
	return QCoreApplication::eventDispatcher() != nullptr;
}

QAbstractEventDispatcher* QxCoreApplication::GetMainLoop()
{
	return QCoreApplication::eventDispatcher();
}
void QxCoreApplication::CallEventHandler(QxEvtHandler& evtHandler, QxEvent& event, QxEventCallWrapper& callWrapper) const
{
	try
	{
		callWrapper.Execute(evtHandler, event);

		// If event wasn't skipped call callback for this event
		// and restore any possible changes in skip state
		if (const bool isSkipped = event.IsSkipped(); !isSkipped)
		{
			event.ExecuteCallback(evtHandler);
			event.Skip(isSkipped);
		}
	}
	catch (...)
	{
		event.m_ExceptionThrown = true;
		OnUnhandledException(event);
	}
}
