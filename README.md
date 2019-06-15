# Qx
Implementation of some wxWidgets concepts in Qt

# Description
Qt signal/slot system may be amazin and all, but I never liked it and never will. Event system in wxWidgets is more to my liking, so I tried to implement some parts of it in Qt (looking at wxWidgets code of course).

# What's implemented
Currentely there are three classes you can use: `QxEvtHandler`, `QxApplication` and `QxThread`. They model [`wxEvtHandler`](https://docs.wxwidgets.org/trunk/classwx_evt_handler.html), [`wxApp[Console]`](https://docs.wxwidgets.org/trunk/classwx_app.html) and [`wxThread`](https://docs.wxwidgets.org/trunk/classwx_thread.html) respectively. Also there are some basic event classe like `QxNotifyEvent`.

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
