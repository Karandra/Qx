#include "EventFilterQt.h"
#include "EvtHandler.h"
#include "Event.h"
#include "Events/QxNotifyEvent.h"
#include <QMessageBox>

namespace Qx::EventSystem
{
	void QtEventFilter::OnObjectDestroyed()
	{
		m_EvtHandler->ProcessEvent(QxNotifyEvent::EvtObjectDestroyed);
	}
	void QtEventFilter::OnObjectNameChanged(const QString& newName)
	{
		m_EvtHandler->ProcessEvent(QxNotifyEvent::EvtObjectNameChanged, newName);
	}

	bool QtEventFilter::InterceptEvent(QEvent& qEvent)
	{
		if (m_EvtHandler && qEvent.type() == QxEvent::EvtQEvent)
		{
			if (QxEvent* event = static_cast<QtEvent&>(qEvent).GetEvent())
			{
				m_EvtHandler->DoProcessEvent(*event);
			}
			return true;
		}
		return false;
	}

	QtEventFilter::QtEventFilter(QxEvtHandler* evtHandler)
		:m_EvtHandler(evtHandler)
	{
	}

	bool QtEventFilter::event(QEvent* qEvent)
	{
		if (qEvent && InterceptEvent(*qEvent))
		{
			return true;
		}
		return QObject::event(qEvent);
	}
	bool QtEventFilter::eventFilter(QObject* qWatched, QEvent* qEvent)
	{
		if (qEvent && InterceptEvent(*qEvent))
		{
			return false;
		}
		return QObject::eventFilter(qWatched, qEvent);
	}

	void QtEventFilter::InstallEventFilter()
	{
		if (m_EvtHandler)
		{
			if (QObject* watchedObject = m_EvtHandler->GetWatchedQObject())
			{
				ConnectSignal(watchedObject, &QObject::destroyed, &QtEventFilter::OnObjectDestroyed, this);
				ConnectSignal(watchedObject, &QObject::objectNameChanged, &QtEventFilter::OnObjectNameChanged, this);

				watchedObject->installEventFilter(this);
			}
		}
	}
	void QtEventFilter::RemoveEventFilter()
	{
		if (m_EvtHandler)
		{
			if (QObject* watchedObject = m_EvtHandler->GetWatchedQObject())
			{
				DisconnectSignal(watchedObject, &QObject::destroyed, &QtEventFilter::OnObjectDestroyed, this);
				DisconnectSignal(watchedObject, &QObject::objectNameChanged, &QtEventFilter::OnObjectNameChanged, this);

				watchedObject->removeEventFilter(this);
			}
		}
	}

	QtEventFilter& QtEventFilter::operator=(QtEventFilter&& other)
	{
		m_EvtHandler = other.m_EvtHandler;
		other.m_EvtHandler = nullptr;

		return *this;
	}
}
