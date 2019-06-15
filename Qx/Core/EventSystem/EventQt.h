#pragma once
#include "Common.h"
#include <QtCore/QEvent>
class QxEvent;

namespace Qx::EventSystem
{
	class QtEvent final: public QEvent
	{
		friend class QxEvent;

		private:
			QxEvent* m_Event = nullptr;

		private:
			QtEvent(QxEvent* event);
			QtEvent(const QtEvent&) = delete;
			QtEvent(QtEvent&& other);
			~QtEvent();

		public:
			const QxEvent* GetEvent() const
			{
				return m_Event;
			}
			QxEvent* GetEvent()
			{
				return m_Event;
			}

		public:
			QtEvent& operator=(const QtEvent&) = delete;
			QtEvent& operator=(QtEvent&& other);
	};
}
