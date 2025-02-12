# PinWindow
这是一个利用Windows系统的z序段机制实现的窗口置顶器
# 描述：
该工具用于通过调用SetWindowBand来操作窗口的Z序段，允许用户将窗口移至不同的Z序段（例如，将窗口置于任务管理器之上或开始菜单下方等）。此工具依赖于Windows 8及更高版本的窗口段机制，通过DLL注入的方式来绕过一些权限限制。

# 使用说明：
**请您在确保完全理解本软件的运行原理，且仔细了解过本软件实现的目的后再使用。**
**请您在使用的时候暂时关闭您的杀毒软件或者将此工具加入白名单。此工具涉及的一些操作（如注入，Hook函数等）可能会引起误报。我们承诺，本软件完全安全无毒，您可以自行阅读源码并自行编译。**
#### 操作系统要求：
Windows8及以上的操作系统

#### 环境要求：
explorer正在正常运行

#### 如何使用：
双击PinWindowUI.exe，在窗口中拖动准星一样的小指针，直至您要操作的窗口上（和SPY++一样）。（您也可以在下方列表中查找）检查上方显示的与您想选择的是否是一个窗口。在这之后，您可以在右上方的下拉框中选择您想要的Band和Z序，最后点击“设置窗口段”，等待日志窗口输出灰色的“设置成功”即可。

# 运行原理
参考https://blog.adeltax.com/window-z-order-in-windows-10/
https://blog.csdn.net/weixin_43820461/article/details/125018556

利用这两个函数
```cpp
typedef BOOL(WINAPI* NtUserEnableIAMAccess)(
    IN ULONG64 key, 
    IN BOOL enable);
 
typedef BOOL(WINAPI* SetWindowBand)(
    IN HWND hWnd, 
    IN HWND hwndInsertAfter, 
    IN DWORD dwBand);
```
其中，NtUserEnableIAMAccess用来获取调用SetWindowBand的许可，这一切都在注入到explorer后实现。

**（太粗略了，等之后有时间再补充）**

# 编译环境
Visual Studio2022+QT6.72