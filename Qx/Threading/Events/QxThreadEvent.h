#pragma once
#include "Threading/Common.h"

class QxThreadEvent: public QxEvent
{
	public:
		QxEVENT_DEFINE_SCM(Execute, QxThreadEvent);
		QxEVENT_DEFINE_SCM(Started, QxThreadEvent);
		QxEVENT_DEFINE_SCM(Finished, QxThreadEvent);

	public:
		QxThreadEvent() = default;
};
