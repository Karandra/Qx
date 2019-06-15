#pragma once
#include "Common.h"
#include "EventQt.h"
#include "Utility.h"
#include <functional>
class QEvent;
class QxEvtHandler;
class QxCoreApplication;
class QxIndirectCallEvent;

// Event declaration macros
#define QxEVENT_DECLARE(name, type)						extern const QxEventTag<type> QxEVT_##name
#define QxEVENT_DEFINE(name, type)						const QxEventTag<type> QxEVT_##name QxEvent::NewEventID()

#define QxEVENT_DECLARE_SCM(name, type)					static const QxEventTag<type> Evt##name

// Basic event class
class QxEvent
{
	friend class QxEvtHandler;
	friend class QxCoreApplication;
	friend class QxIndirectCallEvent;

	public:
		using TCallback = std::function<void(QxEvtHandler&, QxEvent&)>;

		// The predefined constants for the number of times we propagate
		// an event upwards window child-parent chain
		enum class PropagationLevel: uint32_t
		{
			// Don't propagate it at all
			None = 0,

			// Propagate it until it is processed
			Max = std::numeric_limits<uint32_t>::max()
		};

	public:
		inline static constexpr QxEventTag<QxEvent> EvtNull = {};

		// EvtQEvent is a special event type to distinguish usual QEvent objects from
		// QxEventSystem produced ones inside 'QObject::event/notify/eventFilter/etc' functions.
		QxEVENT_DECLARE_SCM(QEvent, QxEvent);

	public:
		static QxEventID NewEventID();

	private:
		TCallback m_Callback;
		QxEvtHandler* m_EventSender = nullptr;
		QxEvtHandler* m_TargetHandler = nullptr;
		void* m_ClientData = nullptr;

		// Qt event object. Created on demand and used to pass QxEvent objects to Qt event loop.
		// Once created, this objects owns this QxEvent and will delete it in its destructor.
		Qx::EventSystem::QtEvent* m_QEvent = nullptr;

		// The propagation level: while it is positive, we propagate the event to the parent window (if any)
		PropagationLevel m_PropagationLevel = PropagationLevel::None;

		// ID of this event
		QxEventID m_EventID = EvtNull;

		// State
		bool m_IsSkipped = false;
		bool m_IsAllowed = true;
		bool m_CanDisallow = false;
		bool m_ExceptionThrown = false;

		// Initially false but becomes true as soon as WasProcessed() is called for
		// the first time, as this is done only by ProcessEvent() it explains the
		// variable name: it becomes true after ProcessEvent() was called at least
		// once for this event
		bool m_WasProcessed = false;

		// This one is initially false too, but can be set to true to indicate that
		// the event will be passed to another handler if it's not processed in
		// this one.
		bool m_WillBeProcessedAgain = false;

	private:
		QEvent* ReleaseToQt();
		void ExecuteCallback(QxEvtHandler& evtHandler);

		// Processing state
		bool WasProcessed()
		{
			if (m_WasProcessed)
			{
				return true;
			}

			m_WasProcessed = true;
			return false;
		}
		void SetWillBeProcessedAgain()
		{
			m_WillBeProcessedAgain = true;
		}
		bool WillBeProcessedAgain()
		{
			if (m_WillBeProcessedAgain)
			{
				m_WillBeProcessedAgain = false;
				return true;
			}
			return false;
		}

		bool ShouldProcessOnlyIn(QxEvtHandler& evtHandler) const
		{
			return &evtHandler == m_TargetHandler;
		}
		void AssignTarget(QxEvtHandler& evtHandler)
		{
			m_TargetHandler = &evtHandler;
		}

	public:
		QxEvent(QxEventID eventID = EvtNull)
			:m_EventID(eventID)
		{
		}
		QxEvent(const QxEvent& other)
			:QxEvent()
		{
			*this = other;
		}
		QxEvent(QxEvent&& other)
			:QxEvent()
		{
			*this = std::move(other);
		}
		virtual ~QxEvent() = default;

	public:
		QxEventID GetEventID() const
		{
			return m_EventID;
		}
		void SetEventID(QxEventID id)
		{
			m_EventID = id;
		}

		QxEvtHandler* GetSender() const
		{
			return m_EventSender;
		}
		void AssignSender(QxEvtHandler& evtHandler)
		{
			m_EventSender = &evtHandler;
		}
		void ResetSender()
		{
			m_EventSender = nullptr;
		}

		const TCallback& GetCallback() const
		{
			return m_Callback;
		}
		TCallback& GetCallback()
		{
			return m_Callback;
		}
		void SetCallback(TCallback func)
		{
			m_Callback = std::move(func);
		}

		void* GetClientData() const
		{
			return m_ClientData;
		}
		void SetClientData(void* clientData)
		{
			m_ClientData = clientData;
		}

		bool IsSkipped() const
		{
			return m_IsSkipped;
		}
		void Skip(bool skip = true)
		{
			m_IsSkipped = skip;
		}

		bool IsAllowed() const
		{
			return m_IsAllowed;
		}
		void Allow(bool allow = true)
		{
			if (allow || m_CanDisallow)
			{
				m_IsAllowed = allow;
			}
		}
		void Disallow()
		{
			Allow(false);
		}
		bool CanDisallow() const
		{
			return m_CanDisallow;
		}
		void SetCanDisallow(bool canDisallow = true)
		{
			m_CanDisallow = canDisallow;
		}

		bool OwnedByQt() const
		{
			return m_QEvent != nullptr;
		}
		bool ExceptionThrown() const
		{
			return m_ExceptionThrown;
		}
		bool IsProcessed() const
		{
			return m_WasProcessed;
		}

		// Propagation
		bool ShouldPropagate() const
		{
			return m_PropagationLevel != PropagationLevel::None;
		}
		template<class T = PropagationLevel> T StopPropagation()
		{
			using Qx::EventSystem::Utility::ExchangeResetAndReturn;

			static_assert(std::is_integral_v<T> || std::is_enum_v<T>);
			return static_cast<T>(ExchangeResetAndReturn(m_PropagationLevel, PropagationLevel::None));
		}
		template<class T = PropagationLevel> void ResumePropagation(T propagationLevel)
		{
			static_assert(std::is_integral_v<T> || std::is_enum_v<T>);
			m_PropagationLevel = static_cast<PropagationLevel>(propagationLevel);
		}

	public:
		QxEvent& operator=(const QxEvent& other);
		QxEvent& operator=(QxEvent&& other);
};

// Rest of the event declaration and definition macros. These require QxEvent to be already defined
#define QxEVENT_DEFINE_ESCM(name, type)					const QxEventTag<type> type::Evt##name = QxEvent::NewEventID()
#define QxEVENT_DEFINE_ESCM_AS(name, type, other)		const QxEventTag<type> type::Evt##name = (other)

#define QxEVENT_DEFINE_SCM(name, type)					inline static const QxEventTag<type> Evt##name = QxEvent::NewEventID()
#define QxEVENT_DEFINE_SCM_AS(name, type, other)		inline static const QxEventTag<type> Evt##name = (other)
