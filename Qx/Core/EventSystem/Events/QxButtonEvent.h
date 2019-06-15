#pragma once
#include "QxNotifyEvent.h"

class QxButtonEvent: public QxNotifyEvent
{
	public:
		QxEVENT_DEFINE_SCM(Clicked, QxButtonEvent);
		QxEVENT_DEFINE_SCM(Pressed, QxButtonEvent);
		QxEVENT_DEFINE_SCM(Released, QxButtonEvent);
		QxEVENT_DEFINE_SCM(Toggled, QxButtonEvent);

	public:
		QxButtonEvent() = default;
};
