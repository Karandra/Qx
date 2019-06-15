#include "CoreApplication.h"
#include <QtCore>
#include <QException>
#include <QUnhandledException>
#include <QMessageBox>
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4101) // unreferenced local variable

namespace
{
	QxCoreApplication* g_AppInstance = nullptr;
}

QxCoreApplication* QxCoreApplication::GetInstance()
{
	return g_AppInstance;
}
QCoreApplication* QxCoreApplication::GetQCoreApplicationInstance()
{
	return QCoreApplication::instance();
}
bool QxCoreApplication::IsMainLoopRunning()
{
	return GetInstance() != nullptr && QCoreApplication::eventDispatcher() != nullptr;
}

void QxCoreApplication::DoQueueEvent(QxEvtHandler& evtHandler, std::unique_ptr<QxEvent> event)
{
	QReadLocker lock(&m_QueuedEventsLock);
	if (!m_QueuedEventsProcessingSuspended)
	{
		// QEvent-derived object inside QxEvent will delete it
		QCoreApplication::postEvent(evtHandler.GetWatchedQObject(), event.release()->ReleaseToQt());
	}
}
void QxCoreApplication::CallEventHandler(QxEvtHandler& evtHandler, QxEvent& event, QxEventCallWrapper& callWrapper)
{
	try
	{
		evtHandler.CallEventHandler(evtHandler, event, callWrapper);
	}
	catch (...)
	{
		event.m_ExceptionThrown = true;
		if (!OnExceptionInMainLoop())
		{
			ExitMainLoop(-1);
		}
		else
		{
			OnUnhandledException();
		}
	}
}

QxCoreApplication::QxCoreApplication(QCoreApplication& qCoreApp)
	:m_QCoreApp(&qCoreApp)
{
	g_AppInstance = this;
	AddEventFilter(*this);
}
QxCoreApplication::~QxCoreApplication()
{
	RemoveEventFilter(*this);
	g_AppInstance = nullptr;
}

int QxCoreApplication::Run()
{
	return OnRun();
}
int QxCoreApplication::MainLoop()
{
	return QCoreApplication::exec();
}
void QxCoreApplication::ExitMainLoop(int exitCode)
{
	return QCoreApplication::exit(exitCode);
}
bool QxCoreApplication::UsesEventLoop() const
{
	return QCoreApplication::eventDispatcher() != nullptr;
}

void QxCoreApplication::Yield()
{
	QCoreApplication::processEvents();
}
void QxCoreApplication::WakeUpIdle()
{
	if (QAbstractEventDispatcher* mainLoop = GetMainLoop())
	{
		mainLoop->wakeUp();
	}
}

QAbstractEventDispatcher* QxCoreApplication::GetMainLoop()
{
	return QCoreApplication::eventDispatcher();
}

void QxCoreApplication::ProcessQueuedEvents()
{
	QCoreApplication::sendPostedEvents(nullptr, QxEvent::EvtQEvent);
	QCoreApplication::processEvents();
}
void QxCoreApplication::DeleteQueuedEvents()
{
	QCoreApplication::removePostedEvents(nullptr, QxEvent::EvtQEvent);
}
bool QxCoreApplication::HasPendingEvents() const
{
	return QCoreApplication::hasPendingEvents();
}

bool QxCoreApplication::IsQueuedEventsProcessingSuspended() const
{
	QReadLocker lock(&m_QueuedEventsLock);
	return m_QueuedEventsProcessingSuspended;
}
void QxCoreApplication::SuspendQueuedEventsProcessing()
{
	QWriteLocker lock(&m_QueuedEventsLock);
	m_QueuedEventsProcessingSuspended = false;
}
void QxCoreApplication::ResumeQueuedEventsProcessing()
{
	QWriteLocker lock(&m_QueuedEventsLock);
	m_QueuedEventsProcessingSuspended = true;
}

void QxCoreApplication::ScheduleForDestruction(QObject& object)
{
	object.deleteLater();
}
bool QxCoreApplication::IsScheduledForDestruction(QObject& object) const
{
	// No way to implement that without hacking Qt event queue, so returning false.
	return false;
}

bool QxCoreApplication::OnExceptionInMainLoop()
{
	QMessageBox dialog(QMessageBox::Icon::Critical,
					   "Unhandled exception",
					   "An unhandled exception occurred. Press \"Abort\" to terminate the program,\n\
					   \"Retry\" to exit the program normally and \"Ignore\" to try to continue.",
					   QMessageBox::StandardButton::Abort|QMessageBox::StandardButton::Retry|QMessageBox::StandardButton::Ignore
	);
	switch (dialog.exec())
	{
		case QMessageBox::StandardButton::Abort:
		{
			throw;
			return false;
		}
		case QMessageBox::StandardButton::Ignore:
		case QMessageBox::StandardButton::Retry:
		{
			return false;
		}
	};
	return false;
}
void QxCoreApplication::OnUnhandledException()
{
	// We're called from an exception handler so we can re-throw the exception to recover its type
	QString what;
	try
	{
		throw;
	}
	catch (const QUnhandledException& e)
	{
		what = QString("exception of type \"QUnhandledException\"");
	}
	catch (const QException& e)
	{
		what = QString("exception of type \"QException\"");
	}
	catch (const std::exception& e)
	{
		what = QString("standard exception of type \"%1\" with message \"%2\"").arg(typeid(e).name()).arg(e.what());
	}
	catch (...)
	{
		what = "unknown exception";
	}

	// Determine exception context
	QString context;
	if (m_QCoreApp->thread() == QThread::currentThread())
	{
		context = "the application";
	}
	else
	{
		context = "the thread in which it happened";
	}

	qCritical() << QString("Unhandled %s. Terminating %s.").arg(what).arg(context);
}
void QxCoreApplication::OnFatalException()
{
	qFatal("%s", "Fatal exception occurred. Terminating the application");
}

bool QxCoreApplication::HasStoredException() const
{
	return (bool)m_CurrentException;
}
bool QxCoreApplication::StoreCurrentException()
{
	if (!m_CurrentException)
	{
		m_CurrentException = std::current_exception();
		return true;
	}
	return false;
}
void QxCoreApplication::RethrowStoredException()
{
	if (m_CurrentException)
	{
		std::exception_ptr storedException;
		std::swap(storedException, m_CurrentException);
		std::rethrow_exception(storedException);
	}
}

bool QxCoreApplication::OnInit()
{
	return true;
}
int QxCoreApplication::OnRun()
{
	if (OnInit())
	{
		const int exitCode = MainLoop();
		OnExit();
		return exitCode;
	}
	return std::numeric_limits<int>::min();
}
int QxCoreApplication::OnExit()
{
	ProcessQueuedEvents();
	return std::numeric_limits<int>::min();
}

void QxCoreApplication::OnEnterEventLoop(QAbstractEventDispatcher& eventLoop)
{
	// Not implemented
}
void QxCoreApplication::OnExitEventLoop(QAbstractEventDispatcher& eventLoop)
{
	// Not implemented
}

QString QxCoreApplication::GetAppName() const
{
	return QCoreApplication::applicationName();
}
void QxCoreApplication::SetAppName(const QString& value)
{
	QCoreApplication::setApplicationName(value);
}

QString QxCoreApplication::GetVendorName() const
{
	return QCoreApplication::organizationName();
}
void QxCoreApplication::SetVendorName(const QString& value)
{
	QCoreApplication::setOrganizationName(value);
}

QString QxCoreApplication::GetAppVersion() const
{
	return QCoreApplication::applicationVersion();
}
void QxCoreApplication::SetAppVersion(const QString& value)
{
	QCoreApplication::setApplicationVersion(value);
}
