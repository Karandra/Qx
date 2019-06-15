#pragma once
#include "Event.h"
class QxEvtHandler;

namespace Qx::EventSystem
{
	class EventBuilder
	{
		protected:
			QxEvtHandler* m_EvtHandler = nullptr;
			QxEvent* m_Event = nullptr;
			bool m_Async = false;
			bool m_Sent = false;
			bool m_IsSkipped = false;
			bool m_IsAllowed = true;

		private:
			EventBuilder() = default;

		public:
			EventBuilder(QxEvtHandler& evtHandler, std::unique_ptr<QxEvent> event)
				:m_EvtHandler(&evtHandler), m_Event(event.release()), m_Async(true)
			{
			}
			EventBuilder(QxEvtHandler& evtHandler, QxEvent& event)
				:m_EvtHandler(&evtHandler), m_Event(&event), m_Async(false)
			{
			}
			EventBuilder(EventBuilder&& other)
			{
				*this = std::move(other);
			}
			EventBuilder(const EventBuilder&) = delete;
			virtual ~EventBuilder();

		public:
			bool Do();

			bool IsAsync() const
			{
				return m_Async;
			}
			bool IsSkipped() const
			{
				return m_IsSkipped;
			}
			bool IsAllowed() const
			{
				return m_IsAllowed;
			}

		public:
			EventBuilder& operator=(EventBuilder&& other);
			EventBuilder& operator=(const EventBuilder&) = delete;
	};
}

template<class TEvent>
class QxEventBuilder: public Qx::EventSystem::EventBuilder
{
	public:
		QxEventBuilder(QxEvtHandler& evtHandler, std::unique_ptr<TEvent> event)
			:EventBuilder(evtHandler, std::move(event))
		{
		}
		QxEventBuilder(QxEvtHandler& evtHandler, TEvent& event)
			:EventBuilder(evtHandler, event)
		{
		}
		QxEventBuilder(QxEventBuilder&& other)
			:EventBuilder(other)
		{
		}

	public:
		template<class TFunctor> QxEventBuilder& On(TFunctor&& func)
		{
			std::invoke(func, static_cast<TEvent&>(*m_Event));
			return *this;
		}
		template<class TFunctor> QxEventBuilder& Then(TFunctor&& func)
		{
			m_Event->SetCallback([func = std::move(func)](QxEvtHandler& evtHandler, QxEvent& event)
			{
				std::invoke(func, static_cast<TEvent&>(*m_Event));
			});
			return *this;
		}

	public:
		QxEventBuilder& operator=(QxEventBuilder&& other)
		{
			static_cast<EventBuilder&>(*this) = std::move(other);
			return *this;
		}
};
