#pragma once
#include "QxNotifyEvent.h"

class QxTextEvent: public QxNotifyEvent
{
	public:
		QxEVENT_DEFINE_SCM_AS(TextChanged, QxTextEvent, QxNotifyEvent::EvtText);

	public:
		QxTextEvent() = default;
};
