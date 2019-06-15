#pragma once
#include "Common.h"

class QxThread: public QThread, public QxEvtHandler
{
	protected:
		void run() override;

	private:
		void OnStarted();
		void OnFinished();

	public:
		QxThread(QObject* parent = nullptr);

	public:
		void Run(QThread::Priority priority = QThread::Priority::NormalPriority);
		void Exit(int exitCode = 0);
		void Terminate();
};
