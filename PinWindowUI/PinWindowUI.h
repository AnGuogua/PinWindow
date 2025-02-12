#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PinWindowUI.h"
#include "Pipe.h"
#include <QMouseEvent>
#include <Windows.h>
#include <tchar.h>

enum ZBID
{
	ZBID_DEFAULT = 0,
	ZBID_DESKTOP = 1,
	ZBID_UIACCESS = 2,
	ZBID_IMMERSIVE_IHM = 3,
	ZBID_IMMERSIVE_NOTIFICATION = 4,
	ZBID_IMMERSIVE_APPCHROME = 5,
	ZBID_IMMERSIVE_MOGO = 6,
	ZBID_IMMERSIVE_EDGY = 7,
	ZBID_IMMERSIVE_INACTIVEMOBODY = 8,
	ZBID_IMMERSIVE_INACTIVEDOCK = 9,
	ZBID_IMMERSIVE_ACTIVEMOBODY = 10,
	ZBID_IMMERSIVE_ACTIVEDOCK = 11,
	ZBID_IMMERSIVE_BACKGROUND = 12,
	ZBID_IMMERSIVE_SEARCH = 13,
	ZBID_GENUINE_WINDOWS = 14,
	ZBID_IMMERSIVE_RESTRICTED = 15,
	ZBID_SYSTEM_TOOLS = 16,
	ZBID_LOCK = 17,
	ZBID_ABOVELOCK_UX = 18,
};
typedef BOOL(WINAPI* GetWindowBand)(
	HWND hWnd,
	PDWORD pdwBand
	);

class PinWindowUI : public QMainWindow
{
    Q_OBJECT

public:
    PinWindowUI(QWidget *parent = nullptr);
    ~PinWindowUI();
    TCHAR* DLL_PATH;
signals:
    void pushLog(int t, QString context);
    bool Eject();
private slots:
    void onCellDoubleClicked(int row, int column);
    void setInfo(HWND h);
    void addLog(int t,QString context);
    bool Eject_slot();
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
private:
    enum logType
    {
        Null = 0,
        Info = 1,
        Error = 2,
    };
    Ui::PinWindowUIClass ui;
    void updateData();
    int setBand();
    struct Window
    {
        HWND hwnd;
        std::string windowName;
        std::string exeName;
        std::string pID;
        RECT rect;
    };
    QLabel* iconLabel = nullptr;
    bool isDragging = false;
    QPointF startPos;
    HWND lastHwnd = nullptr;
    DWORD expID = 0;
    BOOL InjectDll(DWORD dwProcessId, LPTSTR lpDllPath);
    BOOL EjectDll(DWORD dwProcessId, LPTSTR lpDllPath);
    DWORD GetExplorerPID();
    MyPipe* p;
};
