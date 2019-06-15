#pragma once
#include "../Event.h"
#include "../CallWrapper.h"
#include "Utility.h"
#include <functional>
#include <type_traits>

class QxIndirectCallEvent: public QxEvent, private QxEventCallWrapper
{
	public:
		// Event for anonymous execution of a given callable (see QxEvtHandler::CallAfter)
		QxEVENT_DEFINE_SCM(IndirectCall, QxIndirectCallEvent);

	private:
		void Execute(QxEvtHandler& evtHandler, QxEvent& event) override = 0;
		bool IsSameAs(const QxEventCallWrapper& other) const override
		{
			return this == &other;
		}
		QxEvtHandler* GetEvtHandler() override
		{
			return GetSender();
		}

	public:
		QxIndirectCallEvent(QxEvtHandler& evtHandler)
			:QxEvent(EvtIndirectCall)
		{
			AssignSender(evtHandler);
		}

	public:
		QxEventCallWrapper& GetCallWrapper()
		{
			return *this;
		}
		void Execute()
		{
			Execute(*GetSender(), *this);
		}
};

namespace Qx::EventSystem
{
	// Wrapper for lambda function or a class which implements 'operator()'
	template<class TCallable, class... Args>
	class FunctorIndirectCall: public QxIndirectCallEvent
	{
		protected:
			TCallable m_Callable;
			std::tuple<Args...> m_Parameters;

		public:
			FunctorIndirectCall(QxEvtHandler& evtHandler, TCallable callable, Args&&... arg)
				:QxIndirectCallEvent(evtHandler), m_Callable(std::move(callable)), m_Parameters(std::forward<Args>(arg)...)
			{
			}
			
		public:
			void Execute(QxEvtHandler& evtHandler, QxEvent& event) override
			{
				std::apply(m_Callable, std::move(m_Parameters));
			}
	};

	// Wrapper for free or static function
	template<class TFunction, class... Args>
	class FunctionIndirectCall: public QxIndirectCallEvent
	{
		protected:
			std::tuple<Args...> m_Parameters;
			TFunction m_Function;

		public:
			FunctionIndirectCall(QxEvtHandler& evtHandler, TFunction func, Args&&... arg)
				:QxIndirectCallEvent(evtHandler), m_Function(func), m_Parameters(std::forward<Args>(arg)...)
			{
			}
			
		public:
			void Execute(QxEvtHandler& evtHandler, QxEvent& event) override
			{
				std::apply(m_Function, std::move(m_Parameters));
			}
	};

	// Wrapper for class member function
	template<class TMethod, class... Args>
	class MethodIndirectCall: public QxIndirectCallEvent
	{
		protected:
			std::tuple<Args...> m_Parameters;
			TMethod m_Method = nullptr;

		public:
			MethodIndirectCall(QxEvtHandler& evtHandler, TMethod func, Args&&... arg)
				:QxIndirectCallEvent(evtHandler), m_Method(func), m_Parameters(std::forward<Args>(arg)...)
			{
			}

		public:
			void Execute(QxEvtHandler& evtHandler, QxEvent& event) override
			{
				std::apply([this](auto&& ... arg)
				{
					using TClass = typename Utility::MethodTraits<TMethod>::TInstance;
					std::invoke(m_Method, static_cast<TClass*>(&evtHandler), std::forward<decltype(arg)>(arg)...);
				}, std::move(m_Parameters));
			}
	};
}
