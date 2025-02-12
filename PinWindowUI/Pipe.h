//此文件是关于命名管道通信的，用于GuoguaPinwindow
#pragma once
#include <Windows.h>
#include <QObject>

class MyPipe:public QObject
{
	Q_OBJECT

public:
	MyPipe(LPCWSTR pipeName,bool &ok);
	~MyPipe();
	HANDLE hPipe;
	OVERLAPPED overlapped{};
	
	bool connectToClient();
	bool writeData(const std::string& data);
	bool readData(std::string& outData);
	void readLoop();

private:
	enum logType
	{
		Null = 0,
		Info = 1,
		Error = 2,
	};
signals:
	void pushLog(int t, QString context);
	bool Eject();
};