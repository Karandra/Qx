#pragma once
#include <QtCore>
class QxEvtHandler;

namespace Qx::EventSystem
{
	class QtEventFilter: public QObject
	{
		private:
			Q_OBJECT;
			QxEvtHandler* m_EvtHandler = nullptr;

		private:
			void OnDestroyed();
			void OnNameChanged(const QString& newName);

			bool InterceptEvent(QEvent& qEvent);

		public:
			QtEventFilter(QxEvtHandler* evtHandler);
			QtEventFilter(QtEventFilter&& other)
			{
				*this = std::move(other);
			}
			QtEventFilter(const QtEventFilter&) = delete;
			~QtEventFilter() = default;

		public:
			bool event(QEvent* qEvent) override;
			bool eventFilter(QObject* qWatched, QEvent* qEvent) override;

			void InstallEventFilter();
			void RemoveEventFilter();

		public:
			QtEventFilter& operator=(QtEventFilter&& other);
	};
}
