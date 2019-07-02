# Qx
Implementation of some wxWidgets concepts in Qt


# Description
Qt signal/slot system may be amazing and all, but I never liked it and never will. Event system in wxWidgets is more to my liking, so I tried to implement some parts of it in Qt (looking at wxWidgets code of course).


# What's implemented
Currentely there are three classes you can use: `QxEvtHandler`, `QxApplication` and `QxThread`. They are trying to model [`wxEvtHandler`](https://docs.wxwidgets.org/trunk/classwx_evt_handler.html), [`wxApp[Console]`](https://docs.wxwidgets.org/trunk/classwx_app.html) and [`wxThread`](https://docs.wxwidgets.org/trunk/classwx_thread.html) respectively. Also there are some basic event classes like `QxNotifyEvent`.


# Usage
Pretty much the same as in wxWidgets, here's an example.
```cpp
#include <Qx/Core.hpp>

class MainWindow: public QMainWindow, public QxEvtHandler
{
	private:
		std::unique_ptr<Ui::MainWindow> m_UI;
		
	private:
		void OnButton(int x = 42)
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
			// -------------- Binding to an event
			// [1] Alternate order of parameters to QObject::connect, nothing special
			Qx::EventSystem::ConnectSignal(m_UI->Button, &QPushButton::clicked, &MainWindow::OnButton, this);
			
			// [2] Binds callable object to an event
			Bind(QxNotifyEvent::EvtObjectDestroyed, &MainWindow::OnDestroyed, this);
			
			// [3] Same as [2] but with lambda function
			Bind(QxNotifyEvent::EvtObjectNameChanged, [](QxNotifyEvent& event)
			{
				QMessageBox(QMessageBox::Icon::Information, event.GetString(), "Object name changed");
			});
			
			// -------------- Sending an event
			// [4] Queues given event to next event loop iteration. Queues event with "null" type. 
			QueueEvent(std::make_unique<QxNotifyEvent>("New name"));
			
			// [4.1] Shorthand that forwards parameters to event constructor without explicit call of 'std::make_unique'.
			QueueEvent<QxNotifyEvent>("New name");
			
			// [4.2] Variant of above with deducing event type from event ID. Forwards additional parameters to event constructor.
			// Sets this ID as event ID.
			QueueEvent(QxNotifyEvent::EvtObjectNameChanged, "New name");
			
			// [5] Queues given event to next event loop iteration and allows to configure it
			QueueEventEx(QxNotifyEvent::EvtObjectNameChanged).On([](QxNotifyEvent& event)
			{
				// Configure it here before queuing
				event.SetString("New name");
			}).Then([](QxNotifyEvent& event)
			{
				// Callback to be called after this event has been processed by an event handler.
				// Will be called in each event handler that processed and not skipped this event.
			}).Do();
			// Do() actually queues the event. It will be called in EventBuilder object destructor if it wasn't called manually.
			
			// [6] Queues execution of a given callable (lambda function is this case) to next event loop iteration
			CallAfter([this]()
			{
				...
			});
			
			// [6.1] Or this way
			CallAfter(&MainWindow::OnButton);
			
			// [6.2] Or even this
			CallAfter(&MainWindow::OnButton, 100);
			
			// There are also variants of 'ProcessEvent[Ex]' with the same signatures to initiate event processing right here,
			// without waiting for next loop iteration.
			
			// -------------- Everything else
			// Setup UI after binding to 'QxNotifyEvent::EvtObjectNameChanged' to get this event
			m_UI->setupUi(this);
		}
		~MainWindow() = default;
}
```


# Known issues
- Event `QxNotifyEvent::EvtObjectDestroyed` doesn't work, I'm not sure why.
- For asynchronous even calling (`QxEvtHandler::QueueEvent` and `QxEvtHandler::CallAfter`) Qt `QCoreApplication::postEvent` is used. The mechanism of scheduling used by thins function may be different from wxWidgets one. Implementing Qx-own queue is planned for the future.
- There might be more undiscovered bugs.
