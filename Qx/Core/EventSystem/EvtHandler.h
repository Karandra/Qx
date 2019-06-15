#pragma once
#include "Event.h"
#include "EventItem.h"
#include "EventBuilder.h"
#include "EventFilterQt.h"
#include "EventFilter.h"
#include "CallWrapper.h"
#include "Qx/Core/Utility.h"
#include "Qx/Core/ClientData.h"
#include "Events/QxIndirectCallEvent.h"
#include <QtCore/QNamespace.h>
#include <vector>
#include <variant>
class QObject;
class QxEventFilter;
class QxEventCallWrapper;

namespace Qx::EventSystem
{
	class QtEventFilter;
}

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter

namespace Qx::EventSystem
{
	template<class TSender, class TSelf, class TSignal, class TSlot>
	void ConnectSignal(const TSender* sender, TSignal signal, TSlot&& slot, TSelf self, Qt::ConnectionType type = Qt::ConnectionType::AutoConnection)
	{
		sender->connect(sender, signal, self, std::move(slot), type);
	}

	template<class TSender, class TSelf, class TSignal, class TSlot>
	void DisconnectSignal(const TSender* sender, TSignal signal, TSlot&& slot, TSelf self)
	{
		sender->disconnect(sender, signal, self, std::move(slot));
	}
}


class QxEvtHandler
{
	friend class Qx::EventSystem::QtEventFilter;

	public:
		enum class CientDataType
		{
			Untyped = 0,
			Object
		};

	protected:
		using EventItem = Qx::EventSystem::EventItem;

	private:
		static QxEventFilter* ms_EventFilters;

	public:
		static void AddEventFilter(QxEventFilter& eventFilter);
		static bool RemoveEventFilter(QxEventFilter& eventFilter);

	private:
		// Dynamic events table
		std::vector<EventItem> m_EventTable;

		// Events chain
		QxEvtHandler* m_CurrentHandler = this;
		QxEvtHandler* m_PrevHandler = nullptr;
		QxEvtHandler* m_NextHandler = nullptr;

		// Client data
		std::variant<void*, std::unique_ptr<QxClientData>> m_ClientData;

		// Watched Qt object and Qt event filter
		Qx::EventSystem::QtEventFilter m_QEventFilter;
		QObject* m_WatchedQObject = nullptr;

		// Enabled/disabled switch
		bool m_IsEnabled = true;

	private:
		bool DoBind(QxEventID eventID, std::unique_ptr<QxEventCallWrapper> eventHandler);
		bool DoUnbind(QxEventID eventID, QxEventCallWrapper& eventHandler);

		bool SearchEventTable(QxEvent& event);
		bool ExecuteEventHandler(QxEvent& event, EventItem& eventItem, QxEvtHandler& evtHandler);
		void ExecuteIndirectEvent(QxIndirectCallEvent& event);
		
		bool DoTryApp(QxEvent& event);
		bool DoTryChain(QxEvent& event);
		bool TryHereOnly(QxEvent& event);
		bool TryBeforeAndHere(QxEvent& event);
		bool ProcessEventLocally(QxEvent& event);

	protected:
		void DoQueueEvent(std::unique_ptr<QxEvent> event);
		bool DoProcessEvent(QxEvent& event);

		virtual bool OnDynamicBind(EventItem& eventItem)
		{
			return true;
		}
		virtual bool OnDynamicUnbind(const EventItem& eventItem)
		{
			return true;
		}

		virtual bool TryBefore(QxEvent& event);
		virtual bool TryAfter(QxEvent& event);

	public:
		QxEvtHandler(QObject* watchedObject = nullptr);
		QxEvtHandler(QxEvtHandler&& other);
		QxEvtHandler(const QxEvtHandler&) = delete;
		virtual ~QxEvtHandler();

	public:
		bool IsEvtHandlerEnabled() const
		{
			return m_IsEnabled;
		}
		void EnableEvtHandler(bool enabled = true)
		{
			m_IsEnabled = enabled;
		}

		const QObject* GetWatchedQObject() const
		{
			return m_WatchedQObject ? m_WatchedQObject : &m_QEventFilter;
		}
		QObject* GetWatchedQObject()
		{
			return m_WatchedQObject ? m_WatchedQObject : &m_QEventFilter;
		}
		void RemoveWatchedObject();
		void AssignWatchedObject(QObject& qObject);

	public:
		// EvtHandlers chain
		QxEvtHandler* GetNextHandler() const
		{
			return m_NextHandler;
		}
		QxEvtHandler* GetPreviousHandler() const
		{
			return m_PrevHandler;
		}
		void SetNextHandler(QxEvtHandler* evtHandler)
		{
			m_NextHandler = evtHandler;
		}
		void SetPrevHandler(QxEvtHandler* evtHandler)
		{
			m_PrevHandler = evtHandler;
		}

		void Unlink();
		bool IsUnlinked() const;

		// EvtHandlers stack
		QxEvtHandler* GetCurrentHandler() const
		{
			return m_CurrentHandler;
		}
		void SetCurrentHandler(QxEvtHandler& evtHandler)
		{
			m_CurrentHandler = &evtHandler;
		}
		void ResetCurrentHandler()
		{
			m_CurrentHandler = this;
		}

		bool PushEventHandler(QxEvtHandler& evtHandler);
		QxEvtHandler* PopEventHandler();

	public:
		// User-supplied data
		CientDataType GetClientDataType() const;
		void* GetClientData() const;
		QxClientData* GetClientObject() const;
		void SetClientData(void* clientData);
		void SetClientData(std::unique_ptr<QxClientData> clientData);

	public:
		// Bind free or static function
		template<class TEvent, class TEventArg>
		bool Bind(QxEventTag<TEvent> eventTag, void(*func)(TEventArg&))
		{
			return DoBind(eventTag, std::make_unique<Qx::EventSystem::FunctionWrapper<QxEventTag<TEvent>>>(func));
		}
		
		// Unbind free or static function
		template<class TEvent, class TEventArg>
		bool Unbind(QxEventTag<TEvent> eventTag, void(*func)(TEventArg&))
		{
			Qx::EventSystem::FunctionWrapper<TEvent> callWrapper(func);
			return DoUnbind(eventTag, callWrapper);
		}

		// Bind functor or a lambda function
		template<class TEvent, class TFunctor>
		bool Bind(QxEventTag<TEvent> eventTag, const TFunctor& func)
		{
			return DoBind(eventTag, std::make_unique<Qx::EventSystem::FunctorWrapper<TEvent, TFunctor>>(func));
		}

		// Unbind functor or lambda function
		template<class TEvent, class TFunctor>
		bool Unbind(QxEventTag<TEvent> eventTag, const TFunctor& func)
		{
			Qx::EventSystem::FunctorWrapper<TEvent, TFunctor> callWrapper(func);
			return DoUnbind(eventTag, callWrapper);
		}

		// Bind a member function
		template<class TEvent, class TClass, class TEventArg, class TEventHandler>
		bool Bind(QxEventTag<TEvent> eventTag, void(TClass::* method)(TEventArg&), TEventHandler* handler)
		{
			return DoBind(eventTag, std::make_unique<Qx::EventSystem::MethodWrapper<TEvent, TClass, TEventArg, TEventHandler>>(method, handler));
		}

		// Unbind a member function
		template<class TEvent, class TClass, class TEventArg, class TEventHandler>
		bool Unbind(QxEventTag<TEvent> eventTag, void(TClass::* method)(TEventArg&), TEventHandler* handler)
		{
			Qx::EventSystem::MethodWrapper<TEvent, TClass, TEventArg, TEventHandler> callWrapper(method, handler)
			return DoUnbind(eventTag, callWrapper);
		}

	public:
		// Bind Qt signal to functor
		template<class TEvent, class TFunctor>
		void BindSignal(TEvent eventTag, TFunctor func, Qt::ConnectionType type = Qt::ConnectionType::AutoConnection)
		{
			using namespace Qx::EventSystem::Utility;
			using TSender = typename MethodTraits<TEvent>::TInstance;

			Qx::EventSystem::ConnectSignal(static_cast<TSender*>(m_WatchedQObject), eventTag, std::move(func), m_WatchedQObject, type);
		}

		// Unbind Qt signal from functor
		template<class TEvent, class TFunctor>
		void UnbindSignal(TEvent event, TFunctor func)
		{
			using namespace Qx::EventSystem::Utility;
			using TSender = typename MethodTraits<TEvent>::TInstance;

			Qx::EventSystem::DisconnectSignal(static_cast<TSender*>(m_WatchedQObject), event, std::move(func), m_WatchedQObject);
		}

		// Bind Qt signal to class member function
		template<class TEvent, class TMethod, class TEventHandler>
		void BindSignal(TEvent eventTag, TMethod method, TEventHandler* handler, Qt::ConnectionType type = Qt::ConnectionType::AutoConnection)
		{
			using namespace Qx::Utility;
			using TSender = typename MethodTraits<TEvent>::TInstance;

			Qx::EventSystem::ConnectSignal(static_cast<TSender*>(m_WatchedQObject), eventTag, std::move(method), handler, type);
		}

		// Unbind Qt signal from class member function
		template<class TEvent, class TMethod, class TEventHandler>
		void UnbindSignal(TEvent eventTag, TMethod method, TEventHandler* handler)
		{
			using namespace Qx::EventSystem::Utility;
			using TSender = typename MethodTraits<TEvent>::TInstance;

			Qx::EventSystem::DisconnectSignal(static_cast<TSender*>(m_WatchedQObject), eventTag, std::move(method), handler);
		}

	public:
		// Regular event sending functions
		bool ProcessEvent(QxEvent& event)
		{
			event.AssignSender(*m_CurrentHandler);
			return DoProcessEvent(event);
		}
		template<class TEvent, class... Args> bool ProcessEvent(Args&&... arg)
		{
			TEvent event(std::forward<Args>(arg)...);
			event.AssignSender(*m_CurrentHandler);

			return DoProcessEvent(event);
		}
		template<class TEvent, class... Args> bool ProcessEvent(QxEventTag<TEvent> eventTag, Args&&... arg)
		{
			TEvent event(std::forward<Args>(arg)...);
			event.SetEventID(eventTag);
			event.AssignSender(*m_CurrentHandler);

			return DoProcessEvent(event);
		}

		void QueueEvent(std::unique_ptr<QxEvent> event)
		{
			DoQueueEvent(std::move(event));
		}
		template<class TEvent, class... Args> void QueueEvent(Args&&... arg)
		{
			DoQueueEvent(std::make_unique<TEvent>(std::forward<Args>(arg)...));
		}
		template<class TEvent, class... Args> void QueueEvent(QxEventTag<TEvent> eventTag, Args&&... arg)
		{
			auto event = std::make_unique<TEvent>(std::forward<Args>(arg)...);
			event->SetEventID(eventTag);

			DoQueueEvent(std::move(event));
		}

		// Construct and send the event using event builder
		template<class TEvent, class... Args>
		QxEventBuilder<TEvent> ProcessEventEx(QxEventTag<TEvent> eventTag, Args&&... arg)
		{
			TEvent event(std::forward<Args>(arg)...);
			event.SetEventID(eventTag);
			event.AssignSender(*m_CurrentHandler);

			return QxEventBuilder<TEvent>(*this, event);
		}

		template<class TEvent, class... Args>
		QxEventBuilder<TEvent> QueueEventEx(QxEventTag<TEvent> eventTag, Args&&... arg)
		{
			auto event = std::make_unique<TEvent>(std::forward<Args>(arg)...);
			event->SetEventID(eventTag);

			return QxEventBuilder<TEvent>(*this, event);
		}

		// Queue execution of a class member, free, static, lambda function or a class which implements 'operator()'
		template<class TCallable, class... Args>
		void CallAfter(TCallable callable, Args&&... arg)
		{
			using namespace Qx::EventSystem;
			using TCallableTraits = typename Qx::Utility::CallableTraits<TCallable, Args...>;

			if constexpr(TCallableTraits::IsMemberFunction)
			{
				DoQueueEvent(std::make_unique<MethodIndirectCall<TCallable, Args...>>(*this, callable, std::forward<Args>(arg)...));
			}
			else if constexpr(TCallableTraits::IsFreeFunction)
			{
				DoQueueEvent(std::make_unique<FunctionIndirectCall<TCallable, Args...>>(*this, callable, std::forward<Args>(arg)...));
			}
			else if constexpr(TCallableTraits::IsFunctor)
			{
				DoQueueEvent(std::make_unique<FunctorIndirectCall<TCallable, Args...>>(*this, std::move(callable), std::forward<Args>(arg)...));
			}
			else
			{
				static_assert(false, "Unsupported callable type or the type is not callable");
			}
		}

	public:
		QxEvtHandler& operator=(QxEvtHandler&& other);
};

// Restore suppressed warnings
#pragma warning(pop)
