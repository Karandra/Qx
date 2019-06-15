#pragma once
#include "QxNotifyEvent.h"

class QxIOEvent: public QxNotifyEvent
{
	public:
		QxEVENT_DEFINE_SCM(Closing, QxIOEvent);

		QxEVENT_DEFINE_SCM(BytesWritten, QxIOEvent);
		QxEVENT_DEFINE_SCM(BytesRead, QxIOEvent);

		QxEVENT_DEFINE_SCM(ChannelBytesWritten, QxIOEvent);
		QxEVENT_DEFINE_SCM(ChannelBytesRead, QxIOEvent);
		QxEVENT_DEFINE_SCM(ChannelReadFinished, QxIOEvent);

	private:
		int64_t m_Bytes = 0;
		int m_Channel = 0;

	public:
		QxIOEvent() = default;

	public:
		int64_t GetBytes() const
		{
			return m_Bytes;
		}
		void SetBytes(int64_t value)
		{
			m_Bytes = value;
		}

		int GetChannel() const
		{
			return m_Channel;
		}
		void SetChannel(int value)
		{
			m_Channel = value;
		}
};
