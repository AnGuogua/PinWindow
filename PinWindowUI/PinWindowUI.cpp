#include "PinWindowUI.h"
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <TlHelp32.h>
#include <sstream>
#include <iomanip>
#include <thread>
#include <strsafe.h> // 包含 StringCchCopy 和 StringCchCat 的头文件
#include <QFile>

PinWindowUI::PinWindowUI(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//初始化表格
	ui.tableWidget->setColumnCount(3);//设置列数
	ui.tableWidget->setRowCount(0);//设置行数
	QStringList headers;
	headers << tr("句柄") << tr("标题") << tr("进程名");
	ui.tableWidget->setHorizontalHeaderLabels(headers);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	updateData();

	bool ok;
	p = new MyPipe(L"\\\\.\\Pipe\\GuoguaPinWindow", ok);
	if (ok)
	{
		connect(p, &MyPipe::pushLog, this, &PinWindowUI::addLog);
		connect(p, &MyPipe::Eject, this, &PinWindowUI::Eject_slot);
	}
	// 在构造函数中连接信号和槽
	connect(ui.tableWidget, &QTableWidget::cellDoubleClicked, this, &PinWindowUI::onCellDoubleClicked);
	connect(ui.pushButton, &QPushButton::clicked, this, &PinWindowUI::setBand);
	connect(ui.refresh, &QPushButton::clicked, this, [this]() {
		this->updateData();
		});
	connect(this, &PinWindowUI::pushLog, this, &PinWindowUI::addLog);
	connect(this, &PinWindowUI::Eject, this, &PinWindowUI::Eject_slot);
	

	iconLabel = new QLabel(this);
	QPixmap pixmap(":/PinWindowUI/Normal.ico");
	iconLabel->setPixmap(pixmap);
	iconLabel->setAlignment(Qt::AlignCenter);
	// 设置对齐方式为居中
	iconLabel->setAlignment(Qt::AlignCenter);

	// 设置 QLabel 无外边距和内部内容边距
	iconLabel->setMargin(0);  // 外边距
	iconLabel->setContentsMargins(0, 0, 0, 0);  // 内部内容边距
	iconLabel->adjustSize();//适应大小

	// 设置 QLabel 大小策略为适应图标
	iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	iconLabel->move(260, 125);//调整坐标
	this->installEventFilter(this);//安装事件过滤器

	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
	DLL_PATH = new TCHAR[_tcslen(szFilePath) + _tcslen(L"DLL.dll") + 1];  // 分配内存，包含空字符

	if (DLL_PATH != nullptr) {
		StringCchCopy(DLL_PATH, _tcslen(szFilePath) + 1, szFilePath);  // 复制文件路径
		StringCchCat(DLL_PATH, _tcslen(szFilePath) + _tcslen(L"DLL.dll") + 1, L"DLL.dll");  // 拼接 DLL 名称
	}
	//在此处释放资源
	QFile file;
	file.copy(":/dll/DLL.dll",QString::fromWCharArray(DLL_PATH));
	emit pushLog(Info, tr("初始化成功 DLL路径为：") + QString::fromWCharArray(DLL_PATH));
}

PinWindowUI::~PinWindowUI()
{
	delete p;
	EjectDll(expID, DLL_PATH);
}

//https://blog.csdn.net/setlee/article/details/140790825
void PinWindowUI::setInfo(HWND hWnd)
{
	if (hWnd != 0) {
		lastHwnd = hWnd;

		//----- 获取窗口标题
		TCHAR szTitle[MAX_PATH];
		GetWindowText(hWnd, szTitle, MAX_PATH);
		//----- 获取窗口类名
		TCHAR szClass[MAX_PATH];
		GetClassName(hWnd, szClass, MAX_PATH);
		//----- 获取窗口样式
		LONG style = GetWindowLong(hWnd, GWL_STYLE);
		//----- 获取窗口矩形
		RECT rect;
		GetWindowRect(hWnd, &rect);
		//----- 获取窗口进程ID
		DWORD processId = 0;
		GetWindowThreadProcessId(hWnd, &processId); // 获取进程 ID
		//----- 获取进程名
		WCHAR processPath[MAX_PATH] = { 0 };
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
		if (hProcess)
		{

			if (GetModuleFileNameEx(hProcess, NULL, processPath, MAX_PATH))
			{
				CloseHandle(hProcess);
			}
			else
			{
				CloseHandle(hProcess);
			}
		}
		QString windowsHwnd = QString("%1").arg((quintptr)hWnd, 8, 16, QLatin1Char('0')).toUpper();
		QString windowsTitle = QString::fromWCharArray(szTitle);
		QString windowsClass = QString::fromWCharArray(szClass);

		QString windowsStyle = QString("%1").arg((quint32)style, 8, 16, QChar('0')).toUpper();

		QString windowsRect = QString("(%1,%2)-(%3,%4) %5×%6")
			.arg(rect.left)
			.arg(rect.top)
			.arg(rect.right)
			.arg(rect.bottom)
			.arg(rect.right - rect.left)
			.arg(rect.bottom - rect.top);
		QString processID = QString::number(processId);
		QString processName = QString::fromWCharArray(processPath);

		const auto hpath = LoadLibrary(L"user32.dll");
		const auto pGetWindowBand = GetWindowBand(GetProcAddress(hpath, "GetWindowBand"));

		PDWORD p = new DWORD;
		QString ZBID;
		if (pGetWindowBand(hWnd, p))
		{
			int i = static_cast<int>(*p);
			switch (i)
			{
			case ZBID_DEFAULT:
				ZBID = "ZBID_DEFAULT";
				break;
			case ZBID_DESKTOP:
				ZBID = "ZBID_DESKTOP";
				break;
			case ZBID_UIACCESS:
				ZBID = "ZBID_UIACCESS";
				break;
			case ZBID_IMMERSIVE_IHM:
				ZBID = "ZBID_IMMERSIVE_IHM";
				break;
			case ZBID_IMMERSIVE_NOTIFICATION:
				ZBID = "ZBID_IMMERSIVE_NOTIFICATION";
				break;
			case ZBID_IMMERSIVE_APPCHROME:
				ZBID = "ZBID_IMMERSIVE_APPCHROME";
				break;
			case ZBID_IMMERSIVE_MOGO:
				ZBID = "ZBID_IMMERSIVE_MOGO";
				break;
			case ZBID_IMMERSIVE_EDGY:
				ZBID = "ZBID_IMMERSIVE_EDGY";
				break;
			case ZBID_IMMERSIVE_INACTIVEMOBODY:
				ZBID = "ZBID_IMMERSIVE_INACTIVEMOBODY";
				break;
			case ZBID_IMMERSIVE_INACTIVEDOCK:
				ZBID = "ZBID_IMMERSIVE_INACTIVEDOCK";
				break;
			case ZBID_IMMERSIVE_ACTIVEMOBODY:
				ZBID = "ZBID_IMMERSIVE_ACTIVEMOBODY";
				break;
			case ZBID_IMMERSIVE_ACTIVEDOCK:
				ZBID = "ZBID_IMMERSIVE_ACTIVEDOCK";
				break;
			case ZBID_IMMERSIVE_BACKGROUND:
				ZBID = "ZBID_IMMERSIVE_BACKGROUND";
				break;
			case ZBID_IMMERSIVE_SEARCH:
				ZBID = "ZBID_IMMERSIVE_SEARCH";
				break;
			case ZBID_GENUINE_WINDOWS:
				ZBID = "ZBID_GENUINE_WINDOWS";
				break;
			case ZBID_IMMERSIVE_RESTRICTED:
				ZBID = "ZBID_IMMERSIVE_RESTRICTED";
				break;
			case ZBID_SYSTEM_TOOLS:
				ZBID = "ZBID_SYSTEM_TOOLS";
				break;
			case ZBID_LOCK:
				ZBID = "ZBID_LOCK";
				break;
			case ZBID_ABOVELOCK_UX:
				ZBID = "ZBID_ABOVELOCK_UX";
				break;
			default:
				ZBID = QString::number(GetLastError());
				break;
			}
		}
		else
		{
			ZBID = QString::number(GetLastError());
		}



		ui.HWND->setText(windowsHwnd);
		ui.title->setText(windowsTitle);
		ui.wclass->setText(windowsClass);
		ui.style->setText(windowsStyle);
		ui.rect->setText(windowsRect);
		ui.pID->setText(processID);
		ui.pName->setText(processName);
		ui.zbid->setText(ZBID);
	}


}

bool PinWindowUI::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		// 获取 QLabel 的区域（相对于 IconWidget）
		QRect labelRect = iconLabel->geometry();

		// 判断点击的位置是否在 QLabel 内部
		if (labelRect.contains(mouseEvent->pos())) {
			// 如果点击在 QLabel 上，执行相应操作
			// 例如，设置为拖动状态
			isDragging = true;
			startPos = mouseEvent->globalPosition();
			QCursor cursor(QPixmap(":/PinWindowUI/Drag.cur"));
			setCursor(cursor);
			QPixmap pixmap(":/PinWindowUI/Drag.ico");
			iconLabel->setPixmap(pixmap);
		}
	}

	if (event->type() == QEvent::MouseMove) {
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (isDragging) {
			POINT pt;
			GetCursorPos(&pt);
			HWND hwnd = WindowFromPoint(pt);
			hwnd = GetAncestor(hwnd, GA_ROOT);
			if (hwnd != lastHwnd) {
				emit setInfo(lastHwnd);
				lastHwnd = hwnd;
			}
			return true;
		}

	}
	if (event->type() == QEvent::MouseButtonRelease) {
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (isDragging) {
			isDragging = false;
			setCursor(Qt::ArrowCursor);
			QPixmap pixmap(":/PinWindowUI/Normal.ico");
			iconLabel->setPixmap(pixmap);
			if (lastHwnd) {
				emit setInfo(lastHwnd);
			}
			return true;
		}

	}
	return QWidget::eventFilter(watched, event);  // 将事件传递给父类


}

void PinWindowUI::updateData()
{
	if (ui.tableWidget->rowCount() != 0)
	{
		ui.tableWidget->setRowCount(0);
	}
	//获取窗口列表，参考文章：https://blog.csdn.net/yp18792574062/article/details/109279577
	HWND desktop_HWND = GetDesktopWindow();//获取桌面句柄，因为所有窗口都是桌面的子窗口
	HWND window = GetWindow(desktop_HWND, GW_CHILD);
	std::vector<Window> v;
	do
	{
		Window w;
		w.hwnd = window;

		char windowTitle[128] = "";
		GetWindowTextA(window, windowTitle, 128);
		w.windowName = windowTitle;

		DWORD process_id;
		GetWindowThreadProcessId(window, &process_id);
		w.pID = process_id;

		RECT rect;
		GetWindowRect(window, &rect);
		w.rect = rect;

		char exename[128] = "";
		HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, process_id);
		GetProcessImageFileNameA(hProc, exename, 128);
		CloseHandle(hProc);
		std::string window_name = exename;
		auto pos = window_name.rfind("/");
		if (pos == std::string::npos) {
			pos = window_name.rfind("\\");
		}
		if (pos == std::string::npos) {
			w.exeName = "";
		}
		else
		{
			w.exeName = window_name.substr(pos + 1, window_name.length() - pos);
		}
		v.push_back(w);
		window = GetNextWindow(window, GW_HWNDNEXT);
	} while (window != NULL);

	//输出--正常窗口
	for (int i = 0; i < v.size(); i++)
	{
		if ((v[i].rect.bottom == 0 || v[i].rect.right == 0)) {
			continue;
		}
		DWORD ex_styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_EXSTYLE);
		if (ex_styles & WS_EX_TOOLWINDOW) {
			continue;
		}
		DWORD styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_STYLE);
		if (styles & WS_CHILD) {
			continue;
		}
		if (!IsWindowVisible(v[i].hwnd)) {
			continue;
		}
		int r = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(ui.tableWidget->rowCount());
		//句柄
		TCHAR szBuffer[256];
		wsprintf(szBuffer, L"0x%08p", v[i].hwnd);
		QString strHWND = QString::fromWCharArray(szBuffer);
		ui.tableWidget->setItem(r, 0, new QTableWidgetItem(strHWND));
		//标题
		ui.tableWidget->setItem(r, 1, new QTableWidgetItem(QString::fromLocal8Bit(v[i].windowName)));
		//进程名
		ui.tableWidget->setItem(r, 2, new QTableWidgetItem(QString::fromLocal8Bit(v[i].exeName)));
	}
	//输出：大小为0的窗口
	for (int i = 0; i < v.size(); i++)
	{
		if ((v[i].rect.bottom == 0 || v[i].rect.right == 0)) {
			int r = ui.tableWidget->rowCount();
			ui.tableWidget->insertRow(ui.tableWidget->rowCount());
			//句柄
			TCHAR szBuffer[256];
			wsprintf(szBuffer, L"0x%08p", v[i].hwnd);
			QString strHWND = QString::fromWCharArray(szBuffer);
			ui.tableWidget->setItem(r, 0, new QTableWidgetItem(strHWND));
			//标题
			ui.tableWidget->setItem(r, 1, new QTableWidgetItem(QString::fromLocal8Bit(v[i].windowName)));
			//进程名
			ui.tableWidget->setItem(r, 2, new QTableWidgetItem(QString::fromLocal8Bit(v[i].exeName)));
		}
		DWORD ex_styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_EXSTYLE);
		if (ex_styles & WS_EX_TOOLWINDOW) {
			continue;
		}
		DWORD styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_STYLE);
		if (styles & WS_CHILD) {
			continue;
		}
		if (!IsWindowVisible(v[i].hwnd)) {
			continue;
		}
	}

	//输出：工具窗口
	for (int i = 0; i < v.size(); i++)
	{
		if ((v[i].rect.bottom == 0 || v[i].rect.right == 0)) {
			continue;
		}
		DWORD ex_styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_EXSTYLE);
		if (ex_styles & WS_EX_TOOLWINDOW) {
			int r = ui.tableWidget->rowCount();
			ui.tableWidget->insertRow(ui.tableWidget->rowCount());
			//句柄
			TCHAR szBuffer[256];
			wsprintf(szBuffer, L"0x%08p", v[i].hwnd);
			QString strHWND = QString::fromWCharArray(szBuffer);
			ui.tableWidget->setItem(r, 0, new QTableWidgetItem(strHWND));
			//标题
			ui.tableWidget->setItem(r, 1, new QTableWidgetItem(QString::fromLocal8Bit(v[i].windowName)));
			//进程名
			ui.tableWidget->setItem(r, 2, new QTableWidgetItem(QString::fromLocal8Bit(v[i].exeName)));
		}
		DWORD styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_STYLE);
		if (styles & WS_CHILD) {
			continue;
		}
		if (!IsWindowVisible(v[i].hwnd)) {
			continue;
		}
	}

	//输出：子窗口，不能应用于弹出式窗口风格
	for (int i = 0; i < v.size(); i++)
	{
		if ((v[i].rect.bottom == 0 || v[i].rect.right == 0)) {
			continue;
		}
		DWORD ex_styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_EXSTYLE);
		if (ex_styles & WS_EX_TOOLWINDOW) {
			continue;
		}
		DWORD styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_STYLE);
		if (styles & WS_CHILD) {
			int r = ui.tableWidget->rowCount();
			ui.tableWidget->insertRow(ui.tableWidget->rowCount());
			//句柄
			TCHAR szBuffer[256];
			wsprintf(szBuffer, L"0x%08p", v[i].hwnd);
			QString strHWND = QString::fromWCharArray(szBuffer);
			ui.tableWidget->setItem(r, 0, new QTableWidgetItem(strHWND));
			//标题
			ui.tableWidget->setItem(r, 1, new QTableWidgetItem(QString::fromLocal8Bit(v[i].windowName)));
			//进程名
			ui.tableWidget->setItem(r, 2, new QTableWidgetItem(QString::fromLocal8Bit(v[i].exeName)));
		}
		if (!IsWindowVisible(v[i].hwnd)) {
			continue;
		}
	}

	//输出：不可见窗口
	for (int i = 0; i < v.size(); i++)
	{
		if ((v[i].rect.bottom == 0 || v[i].rect.right == 0)) {
			continue;
		}
		DWORD ex_styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_EXSTYLE);
		if (ex_styles & WS_EX_TOOLWINDOW) {
			continue;
		}
		DWORD styles = (DWORD)GetWindowLongPtr(v[i].hwnd, GWL_STYLE);
		if (styles & WS_CHILD) {
			continue;
		}
		if (!IsWindowVisible(v[i].hwnd)) {
			int r = ui.tableWidget->rowCount();
			ui.tableWidget->insertRow(ui.tableWidget->rowCount());
			//句柄
			TCHAR szBuffer[256];
			wsprintf(szBuffer, L"0x%08p", v[i].hwnd);
			QString strHWND = QString::fromWCharArray(szBuffer);
			ui.tableWidget->setItem(r, 0, new QTableWidgetItem(strHWND));
			//标题
			ui.tableWidget->setItem(r, 1, new QTableWidgetItem(QString::fromLocal8Bit(v[i].windowName)));
			//进程名
			ui.tableWidget->setItem(r, 2, new QTableWidgetItem(QString::fromLocal8Bit(v[i].exeName)));
		}

	}
	return;
}

// 槽函数实现
void PinWindowUI::onCellDoubleClicked(int row, int column)
{
	// 获取双击的单元格中的数据
	QString cellData = ui.tableWidget->item(row, 0)->text();
	HWND h = nullptr;
	sscanf_s(cellData.toStdString().c_str(), "%x", &h);
	setInfo(h);
}

int PinWindowUI::setBand()
{
	//获取z序和层级
	int zorder, band;
	switch (ui.comboBox_2->currentIndex())
	{
	case 0:
		zorder = 1;
		break;
	case 1:
		zorder = 0;
		break;
	case 2:
		zorder = -2;
		break;
	case 3:
		zorder = -1;
		break;
	}
	switch (ui.comboBox->currentIndex())
	{
	case 0:
		band = 1;
		break;
	case 1:
		band = 12;
		break;
	case 2:
		band = 5;
		break;
	case 3:
		band = 6;
		break;
	case 4:
		band = 8;
		break;
	case 5:
		band = 4;
		break;
	case 6:
		band = 7;
		break;
	case 7:
		band = 16;
		break;
	case 8:
		band = 17;
		break;
	case 9:
		band = 18;
		break;
	case 10:
		band = 3;
		break;
	case 11:
		band = 14;
		break;
	case 12:
		band = 2;
	}
	emit pushLog(Null,"\n--------------------------------------\n");
	expID = GetExplorerPID();
	if (!InjectDll(expID, DLL_PATH))
	{
		return 1;
	}
	//等待连接
	if (!p->connectToClient())
	{
		return 1;
	}
	//发送
	std::stringstream ss;
	ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << reinterpret_cast<uintptr_t>(lastHwnd);
	std::string send = "PinWindow." + ss.str() + '.' + std::to_string(zorder) +'.'+ std::to_string(band);
	p->writeData(send);
	std::thread t([=]() {
		p->readLoop();
		});
	t.detach();
	return 0;

}

BOOL PinWindowUI::InjectDll(DWORD dwProcessId, LPTSTR lpDllPath)
{
	HANDLE hProcess = NULL;
	LPTSTR lpDllPathRemote = NULL;
	HANDLE hThread = NULL;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD |
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessId);
	if (!hProcess)
	{
		emit pushLog(Error, tr("[注入]打开进程失败 错误代码：") + QString::number(GetLastError()));
		return FALSE;
	}


	// (1)	调用VirtualAllocEx函数在远程进程的地址空间中分配一块内存；
	int cbDllPath = (_tcslen(lpDllPath) + 1) * sizeof(TCHAR);
	lpDllPathRemote = (LPTSTR)VirtualAllocEx(hProcess, NULL, cbDllPath,
		MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!lpDllPathRemote)
	{
		emit pushLog(Error, tr("[注入]分配远程内存失败 错误代码：") + QString::number(GetLastError()));
		return FALSE;
	}


	// (2)	调用WriteProcessMemory函数把要注入的dll的路径复制到第1步分配的内存中；
	if (!WriteProcessMemory(hProcess, lpDllPathRemote, lpDllPath, cbDllPath, NULL))
	{
		emit pushLog(Error, tr("[注入]写入内存失败 错误代码：") + QString::number(GetLastError()));
		return FALSE;
	}

	// (3)	调用GetProcAddress函数得到LoadLibraryA / LoadLibraryW函数(Kernel32.dll)的实际地址；
	PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	if (!pfnThreadRtn)
	{
		emit pushLog(Error, tr("[注入]LoadLibrary地址获取失败 错误代码：") + QString::number(GetLastError()));
		return FALSE;
	}


	// (4)	调用CreateRemoteThread函数在远程进程中创建一个线程
	hThread = CreateRemoteThread(hProcess, NULL, 0, pfnThreadRtn, lpDllPathRemote, 0, NULL);
	if (!hThread)
	{
		emit pushLog(Error, tr("[注入]创建远程线程失败 错误代码：") + QString::number(GetLastError()));
		return FALSE;
	}


	WaitForSingleObject(hThread, INFINITE);
	// (5)	调用VirtualFreeEx函数释放第1步分配的内存；
	if (!lpDllPathRemote)
		VirtualFreeEx(hProcess, lpDllPathRemote, 0, MEM_RELEASE);
	if (hThread)
		CloseHandle(hThread);
	if (hProcess)
		CloseHandle(hProcess);
	emit pushLog(Info, tr("[注入]注入成功"));
	return TRUE;
}

BOOL PinWindowUI::EjectDll(DWORD dwProcessId, LPTSTR lpDllPath)
{
	HANDLE hSnapshot;
	MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
	BOOL bRet;
	BOOL bFound = FALSE;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		emit pushLog(Error, tr("[释放]创建快照失败 错误代码：") + QString::number(GetLastError()));
		return FALSE;
	}
		

	bRet = Module32First(hSnapshot, &me);
	while (bRet)
	{
		if (_tcsicmp(TEXT("DLL.dll"), me.szModule) == 0 ||
			_tcsicmp(lpDllPath, me.szExePath) == 0)
		{
			bFound = TRUE;
			break;
		}

		bRet = Module32Next(hSnapshot, &me);
	}
	if (!bFound)
	{
		emit pushLog(Error, tr("[释放]未找到DLL"));
		return FALSE;
	}
		

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD |
		PROCESS_VM_OPERATION, FALSE, dwProcessId);
	if (!hProcess)
	{
		emit pushLog(Error, tr("[释放]打开进程失败 错误代码：") + QString::number(GetLastError()));
		return FALSE;
	}
		

	// (6)	调用GetProcAddress得到FreeLibrary函数(Kernel32.dll)的实际地址；
	PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");
	if (!pfnThreadRtn)
	{
		emit pushLog(Error, tr("[释放]获取FreeLibrary地址失败 错误代码：") + QString::number(GetLastError()));
		return FALSE;
	}
		

	// (7)	调用CreateRemoteThread函数在远程进程中创建一个新线程，
	// 让该线程调用FreeLibrary函数并在参数中传入已注入dll的模块地址以卸载该dll
	hThread = CreateRemoteThread(hProcess, NULL, 0, pfnThreadRtn, me.modBaseAddr, 0, NULL);
	if (!hThread)
	{
		emit pushLog(Error, tr("[释放]创建远程线程失败 错误代码：") + QString::number(GetLastError()));
		return FALSE;
	}
		

	WaitForSingleObject(hThread, INFINITE);
	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);
	if (hThread)
		CloseHandle(hThread);
	if (hProcess)
		CloseHandle(hProcess);
	emit pushLog(Info, tr("[释放]卸载DLL成功"));
	return TRUE;
}

DWORD PinWindowUI::GetExplorerPID() {
	// 创建快照
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		emit pushLog(Error, tr("[注入]进程快照创建失败  错误代码：") + QString::number(GetLastError()));
		return 0; // 错误处理
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	DWORD pID = 0;
	// 获取第一个进程
	if (Process32First(hSnapshot, &pe32)) {
		do {
			// 查找 explorer 进程
			if (_wcsicmp(pe32.szExeFile, L"explorer.exe") == 0) {
				if (pID == 0)
				{
					pID = pe32.th32ProcessID;
				}
				else
				{
					emit pushLog(Error, tr("[注入]找到多个explorer实例 PID为：") + QString::number(pe32.th32ProcessID)+"和"+QString::number(pID));
					CloseHandle(hSnapshot); // 记得关闭句柄
					return 0;
				}//如果有两个及以上explorer实例，可能会注入失败，造成explorer无法响应
			}
		} while (Process32Next(hSnapshot, &pe32));
	}
	if (pID)
	{
		emit pushLog(Info, tr("[注入]找到Explorer进程 PID为：") + QString::number(pe32.th32ProcessID));
	}
	else
	{
		emit pushLog(Error, tr("[注入]未找到Explorer进程，请尝试启动explorer后重试"));
	}
	
	CloseHandle(hSnapshot); // 关闭句柄
	return pID; 
}

//更新日志
void PinWindowUI::addLog(int t, QString context)
{
	QString type;
	switch (t)
	{
	case Info:
		type = "[Info] ";
		ui.log->setTextColor(QColor("black"));
		ui.log->append(type + context);
		break;
	case Error:
		type = "[Error] ";
		ui.log->setTextColor(QColor("red"));
		ui.log->append(type + context);
		break;
	case Null:
		type = "";
		ui.log->setTextColor(QColor("gray"));
		ui.log->append(type + context);
		break;
	}
	ui.log->moveCursor(QTextCursor::End);
}

bool PinWindowUI::Eject_slot()
{
	delete p;
	bool ok;
	p = new MyPipe(L"\\\\.\\Pipe\\GuoguaPinWindow", ok);
	if (ok)
	{
		connect(p, &MyPipe::pushLog, this, &PinWindowUI::addLog);
		connect(p, &MyPipe::Eject, this, &PinWindowUI::Eject_slot);
	}
	return EjectDll(expID, DLL_PATH);
}
