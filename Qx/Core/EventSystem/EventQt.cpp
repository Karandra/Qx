#include "EventQt.h"
#include "Event.h"
#include "Utility.h"

namespace Qx::EventSystem
{
	QtEvent::QtEvent(QxEvent* event)
		:QEvent(static_cast<QEvent::Type>(QxEvent::EvtQEvent.GetID())), m_Event(event)
	{
	}
	QtEvent::QtEvent(QtEvent&& other):
		QEvent(std::move(other))
	{
		Utility::ExchangeAndReset(m_Event, other.m_Event, nullptr);
	}
	QtEvent::~QtEvent()
	{
		delete m_Event;
	}

	QtEvent& QtEvent::operator=(QtEvent&& other)
	{
		static_cast<QEvent&>(*this) = std::move(other);
		Utility::ExchangeAndReset(m_Event, other.m_Event, nullptr);

		return *this;
	}
}
