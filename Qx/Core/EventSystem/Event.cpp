#include "Event.h"
#include <atomic>
#include <QtCore>

QxEVENT_DEFINE_ESCM_AS(QEvent, QxEvent, QEvent::registerEventType());

//////////////////////////////////////////////////////////////////////////
QxEventID QxEvent::NewEventID()
{
	static std::atomic<QxEventID> g_EventCounter(QEvent::MaxUser);
	return ++g_EventCounter;
}

QEvent* QxEvent::ReleaseToQt()
{
	if (!m_QEvent)
	{
		m_QEvent = new Qx::EventSystem::QtEvent(this);
	}
	return m_QEvent;
}
void QxEvent::ExecuteCallback(QxEvtHandler& evtHandler)
{
	if (m_Callback)
	{
		m_Callback(evtHandler, *this);
	}
}

QxEvent& QxEvent::operator=(const QxEvent& other)
{
	m_QEvent = nullptr;
	m_Callback = other.m_Callback;
	m_EventSender = other.m_EventSender;
	m_EventID = other.m_EventID;
	m_ClientData = other.m_ClientData;
	m_IsSkipped = other.m_IsSkipped;
	m_IsAllowed = other.m_IsAllowed;
	m_CanDisallow = other.m_CanDisallow;
	m_ExceptionThrown = other.m_ExceptionThrown;

	return *this;
}
QxEvent& QxEvent::operator=(QxEvent&& other)
{
	m_Callback = std::move(other.m_Callback);

	using Qx::Utility::ExchangeAndReset;
	ExchangeAndReset(m_QEvent, other.m_QEvent, nullptr);
	ExchangeAndReset(m_EventSender, other.m_EventSender, nullptr);
	ExchangeAndReset(m_EventID, other.m_EventID, QxEvent::EvtNull);
	ExchangeAndReset(m_ClientData, other.m_ClientData, nullptr);
	ExchangeAndReset(m_IsSkipped, other.m_IsSkipped, false);
	ExchangeAndReset(m_IsAllowed, other.m_IsAllowed, true);
	ExchangeAndReset(m_CanDisallow, other.m_CanDisallow, false);
	ExchangeAndReset(m_ExceptionThrown, other.m_ExceptionThrown, false);

	return *this;
}
