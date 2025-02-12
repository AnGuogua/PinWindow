#include"Pipe.h"
MyPipe::MyPipe(LPCWSTR pipeName,bool& ok)
{
	hPipe = CreateNamedPipeW(
		pipeName,												//管道名
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,				// 双向管道
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,	// 消息模式、读取模式和等待模式
		1,														// 最大实例数
		1024,													// 输出缓冲区大小
		1024,													// 输入缓冲区大小
		0,														// 默认超时
		NULL
	);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		//创建失败
		emit pushLog(Error, tr("[管道]命名管道创建失败 错误代码：") + QString::number(GetLastError()));
        ok = false;
        return;
	}
    overlapped.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (overlapped.hEvent == nullptr) {
        emit pushLog(Error, tr("[管道]命名管道事件创建失败 错误代码：") + QString::number(GetLastError()));
        ok = false;
        return;
    }
    ok = true;
    emit pushLog(Info, "[管道]命名管道已创建");
    return;
}

MyPipe::~MyPipe()
{
    CloseHandle(hPipe);
}
bool MyPipe::connectToClient() {
    BOOL result = ConnectNamedPipe(hPipe, nullptr) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
    if (!result) {
        emit pushLog(Error, tr("[管道]连接DLL失败 错误代码：") + QString::number(GetLastError()));
        return false;
    }
    return true;
    emit pushLog(Info, tr("[管道]已连接到DLL"));
}

bool MyPipe::writeData(const std::string& data) {
    DWORD bytesWritten = 0;
    BOOL result = WriteFile(hPipe, data.c_str(), (DWORD)data.size(), &bytesWritten, &overlapped);

    if (!result && GetLastError() == ERROR_IO_PENDING) {
        // 等待异步操作完成
        if (WaitForSingleObject(overlapped.hEvent, INFINITE) != WAIT_OBJECT_0) {
            emit pushLog(Error, tr("[管道]向DLL发送信息失败 错误代码：") + QString::number(GetLastError()));
            return false;
        }
    }
    //emit pushLog(Info, tr("[管道]向DLL发送信息成功"));
    return true;
}

bool MyPipe::readData(std::string& outData) {
    char buffer[512];
    DWORD bytesRead = 0;

    BOOL result = ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, &overlapped);
    if (!result && GetLastError() == ERROR_IO_PENDING) {
        // 等待异步操作完成
        if (WaitForSingleObject(overlapped.hEvent, INFINITE) != WAIT_OBJECT_0) {
            emit pushLog(Error, tr("[管道]读取DLL信息失败 错误代码：") + QString::number(GetLastError()));
            return false;
        }
    }
    //emit pushLog(Info, tr("[管道]已读取到来自DLL的信息"));
    outData.assign(buffer, bytesRead);
    return true;
}

void MyPipe::readLoop()
{
    while(1)
    {
        Sleep(100);
        std::string s;
        readData(s);
        if (s[0] == 'E')
        {
            switch (s[1])
            {
            case 'A':
                //数据包错误
                emit pushLog(Error, tr("[DLL]数据包错误"));
                break;
            case 'B':
                //调用错误
                emit pushLog(Error, tr("[DLL]调用SetWindowBand失败 错误代码：")+QString::fromStdString(s.substr(2,s.size())));
                break;
            case 'C':
                //IAM失败
                emit pushLog(Error, tr("[DLL]设置线程为IAM线程失败 错误代码：") + QString::fromStdString(s.substr(2, s.size())));
                break;
            case 'D':
                //未获取key
                emit pushLog(Error, tr("[DLL]已成功拦截NtUserEnableIAMAccess，但没有获取到Key"));
            }
                
            emit Eject();
            return;
        }
        else if (s == "finish")//成功
        {
            emit pushLog(Null, tr("设置成功"));
            emit Eject();
            return;
        }
        else if (s[0] == 'L')//log
        {
            switch (s[1])
            {
            case 'A':
                //连接成功
                emit pushLog(Info, tr("[DLL]连接成功"));
                break;
            case 'B':
                //设置跳转
                emit pushLog(Info, tr("[DLL]函数NtUserEnableIAMAccess已被Hook"));
                break;
            case 'C':
                //迫使调用
                emit pushLog(Info, tr("[DLL]正在尝试迫使explorer调用该函数"));
                break;
            case 'D':
                //取消跳转
                emit pushLog(Info, tr("[DLL]函数NtUserEnableIAMAccess的Hook已解除"));
                break;
            case 'E':
                //获取到key
                emit pushLog(Info, tr("[DLL]获取到Key：") + QString::fromStdString(s.substr(2, s.size())));
                break;
            }
        }
        else
        {
            emit pushLog(Null ,tr("[DLL]未知消息：") + QString::fromStdString(s));
        }
    }
   
}
