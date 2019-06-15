#pragma once
#include <QtCore>
#include "CoreApplication.h"

template<class TQApp>
class QxApplication: public QxCoreApplication
{
	public:
		using TApplication = TQApp;

	public:
		static QxApplication* GetInstance()
		{
			return static_cast<QxApplication*>(QxCoreApplication::GetInstance());
		}
		static TApplication* GetQApplicationInstance()
		{
			return static_cast<TApplication*>(QxCoreApplication::GetQCoreApplicationInstance());
		}

	private:
		TApplication m_Application;

	public:
		template<class... Args> QxApplication(Args&&... arg)
			:QxCoreApplication(m_Application), m_Application(std::forward<Args>(arg)...)
		{
			AssignWatchedObject(m_Application);
		}
		~QxApplication()
		{
			RemoveWatchedObject();
		}

	public:
		const TApplication& GetQApp() const
		{
			return m_Application;
		}
		TApplication& GetQApp()
		{
			return m_Application;
		}
};
