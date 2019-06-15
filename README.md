# Qx
Implementation of some wxWidgets concepts in Qt


# Description
Qt signal/slot system may be amazin and all, but I never liked it and never will. Event system in wxWidgets is more to my liking, so I tried to implement some parts of it in Qt (looking at wxWidgets code of course).


# What's implemented
Currentely there are three classes you can use: `QxEvtHandler`, `QxApplication` and `QxThread`. They are trying to model [`wxEvtHandler`](https://docs.wxwidgets.org/trunk/classwx_evt_handler.html), [`wxApp[Console]`](https://docs.wxwidgets.org/trunk/classwx_app.html) and [`wxThread`](https://docs.wxwidgets.org/trunk/classwx_thread.html) respectively. Also there are some basic event classe like `QxNotifyEvent`.


# Usage
Pretty much the same as in wxWidgets, here's an example.
```cpp
class MainWindow: public QMainWindow, public QxEvtHandler
{
	private:
		Q_OBJECT;
		std::unique_ptr<Ui::MainWindow> m_UI;
		
	private:
		void OnButton()
		{
			...
		}
		void OnDestroyed(QxNotifyEvent& event)
		{
			...
		}
		
	public:
		MainWindow(QWidget* parent = nullptr)
			:QMainWindow(parent), QxEvtHandler(this), m_UI(std::make_unique<Ui::MainWindow>())
		{
			// 1
			Qx::EventSystem::ConnectSignal(m_UI->Button, &QPushButton::clicked, &MainWindow::OnButton, this);
			
			// 2
			Bind(QxNotifyEvent::EvtObjectDestroyed, &MainWindow::OnDestroyed, this);
			
			// 3
			Bind(QxNotifyEvent::EvtObjectNameChanged, [](QxNotifyEvent& event)
			{
				QMessageBox(QMessageBox::Icon::Information, event.GetString(), "Object name changed");
			});
			
			// 4
			CallAfter([this]()
			{
				...
			});
		}
		~MainWindow() = default;
}
```
1. Alternate order of parameters to QObject::connect, nothing special.
2. Binds callable object to an event.
3. Same as [2] but with lambda function.
4. Queues execution of a given callable (lambda function is this case) to next event loop iteration.


# Known issues
- Event `QxNotifyEvent::EvtObjectDestroyed` doesn't work, I'm not sure why.
- For asynchronous even calling (`QxEvtHandler::QueueEvent` and `QxEvtHandler::CallAfter`) Qt `QCoreApplication::postEvent` is used. The mechanism of scheduling used by thins function may be different from wxWidgets one. Implementing Qx-own queue is planned for the future.
- There might be more undiscovered bugs.
