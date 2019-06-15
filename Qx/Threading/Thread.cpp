#include "Thread.h"
#include "Events/QxThreadEvent.h"

void QxThread::run()
{
	ProcessEvent(QxThreadEvent::EvtExecute);
}
void QxThread::OnStarted()
{
	ProcessEvent(QxThreadEvent::EvtStarted);
}
void QxThread::OnFinished()
{
	ProcessEvent(QxThreadEvent::EvtFinished);
}

QxThread::QxThread(QObject* parent)
	:QThread(parent), QxEvtHandler(this)
{
	BindSignal(&QThread::started, &QxThread::OnStarted, this);
	BindSignal(&QThread::finished, &QxThread::OnFinished, this);
}

void QxThread::Run(QThread::Priority priority)
{
	QThread::start(priority);
}
void QxThread::Exit(int exitCode)
{
	QThread::exit(exitCode);
}
void QxThread::Terminate()
{
	QThread::terminate();
}
