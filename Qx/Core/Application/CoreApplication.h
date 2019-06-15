#pragma once
#include "Common.h"
#include "Qx/Core/EventSystem/EvtHandler.h"
#include "Qx/Core/EventSystem/EventFilter.h"
#include <QtCore>
#include <exception>
class QCoreApplication;
class QAbstractEventDispatcher;

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter

class QxCoreApplication: public QxEvtHandler, public QxEventFilter
{
	friend class QxEvtHandler;

	public:
		static QxCoreApplication* GetInstance();
		static QCoreApplication* GetQCoreApplicationInstance();
		static bool IsMainLoopRunning();

	private:
		QCoreApplication* m_QCoreApp = nullptr;
		std::exception_ptr m_CurrentException;

		mutable QReadWriteLock m_QueuedEventsLock;
		bool m_QueuedEventsProcessingSuspended = false;

	protected:
		void DoQueueEvent(QxEvtHandler& evtHandler, std::unique_ptr<QxEvent> event);
		void DoQueueEvent(std::unique_ptr<QxEvent> event)
		{
			QxEvtHandler::DoQueueEvent(std::move(event));
		}

		Result FilterEvent(QxEvent& event) override
		{
			return Result::Skip;
		}

	public:
		QxCoreApplication(QCoreApplication& qCoreApp);
		virtual ~QxCoreApplication();

	public:
		const QCoreApplication& GetQApp() const
		{
			return *m_QCoreApp;
		}
		QCoreApplication& GetQApp()
		{
			return *m_QCoreApp;
		}

	public:
		// Event-handling and main loop
		int Run();
		virtual int MainLoop();
		virtual void ExitMainLoop(int exitCode = 0);
		virtual bool UsesEventLoop() const;

		virtual QAbstractEventDispatcher* GetMainLoop();
		virtual void CallEventHandler(QxEvtHandler& evtHandler, QxEvent& event, QxEventCallWrapper& callWrapper);

		virtual void Yield();
		virtual void WakeUpIdle();

	public:
		// Event queue
		virtual void ProcessQueuedEvents();
		void DeleteQueuedEvents();
		bool HasPendingEvents() const;

		bool IsQueuedEventsProcessingSuspended() const;
		void SuspendQueuedEventsProcessing();
		void ResumeQueuedEventsProcessing();

		void ScheduleForDestruction(QObject& object);
		bool IsScheduledForDestruction(QObject& object) const;

	public:
		// Exception handling
		virtual bool OnExceptionInMainLoop();
		virtual void OnUnhandledException();
		virtual void OnFatalException();

		bool HasStoredException() const;
		bool StoreCurrentException();
		void RethrowStoredException();

	public:
		// Callbacks for application-wide "events"
		virtual bool OnInit();
		virtual int OnRun();
		virtual int OnExit();

		virtual void OnEnterEventLoop(QAbstractEventDispatcher& eventLoop);
		virtual void OnExitEventLoop(QAbstractEventDispatcher& eventLoop);

	public:
		// Application information
		QString GetAppName() const;
		void SetAppName(const QString& value);

		QString GetVendorName() const;
		void SetVendorName(const QString& value);

		QString GetAppVersion() const;
		void SetAppVersion(const QString& value);
};

// Restore suppressed warnings
#pragma warning(pop)
