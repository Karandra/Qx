#pragma once
#include "EventSystem/Event.h"

class QxNotifyEvent: public QxEvent
{
	public:
		// Event for 'QObject::destroyed' signal
		QxEVENT_DEFINE_SCM(Destroyed, QxNotifyEvent);

		// Event for 'QObject::objectNameChanged' signal
		QxEVENT_DEFINE_SCM(ObjectNameChanged, QxNotifyEvent);

		QxEVENT_DEFINE_SCM(Text, QxNotifyEvent);
		QxEVENT_DEFINE_SCM(Number, QxNotifyEvent);

	private:
		QString m_String;
		int64_t m_Int = 0;

	public:
		QxNotifyEvent() = default;
		QxNotifyEvent(const QString& text)
			:m_String(text)
		{
		}
		QxNotifyEvent(const QString& text, int64_t number)
			:m_String(text), m_Int(number)
		{
		}
		QxNotifyEvent(int64_t number)
			:m_Int(number)
		{
		}

	public:
		virtual QString GetString() const
		{
			return m_String;
		}
		virtual void SetString(const QString& value)
		{
			m_String = value;
		}

		virtual int64_t GetInt() const
		{
			return m_Int;
		}
		virtual void SetInt(int64_t value)
		{
			m_Int = value;
		}
};
