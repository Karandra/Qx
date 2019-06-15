#pragma once
#include "EventSystem/Event.h"

class QxIdleEvent: public QxEvent
{
	public:
		QxEVENT_DEFINE_SCM(Idle, QxIdleEvent);

	public:
		QxIdleEvent() = default;
};
