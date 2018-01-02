#include "Lifter_server_mscv.h"
#include <QtWidgets/QApplication>
#include<QTextCodec>
#include"win_server.h"
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif
#define DUMP
#ifdef DUMP
#include <Windows.h>
#include <DbgHelp.h>
#include<QDebug>

#include <QDir>
#include <QMessageBox>
#pragma comment(lib, "Dbghelp.lib")

LONG WINAPI H2SMonitorServerUnhandledExceptionFilter(
	struct _EXCEPTION_POINTERS* ExceptionInfo
)
{
	//
	// 创建dmp目录
	QString strDmpDirName(QStringLiteral("./dmp"));
	QDir dmpDir(strDmpDirName);
	if (!dmpDir.exists()) {
		if (!dmpDir.mkdir(strDmpDirName)) {
			qDebug() << QString("创建dmp目录失败,error:%1")
				.arg(GetLastError());
			MessageBox(NULL, L"请手动在程序根目录创建一个 dmp 文件夹 ", L"", MB_OK);
			return EXCEPTION_CONTINUE_SEARCH;
		}
	}
	//MessageBox(NULL, L"发生错误,已生产dump文件,请及时把dmp文件夹打包发于技术人员!", L"错误提示", MB_OK);

	wchar_t dmpFileName[MAX_PATH] = { 0 };
	SYSTEMTIME st;
	GetLocalTime(&st);
	swprintf(dmpFileName, L"dmp/%04d%02d%02d_%02d%02d%02d_%03d.dmp",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	// 创建dmp文件
	HANDLE hDmpFile = CreateFile(dmpFileName, FILE_ALL_ACCESS,
		0, NULL, CREATE_ALWAYS, 0, NULL);
	if (INVALID_HANDLE_VALUE == hDmpFile) {
		return EXCEPTION_CONTINUE_SEARCH;
	}
	else {
		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId();
		eInfo.ExceptionPointers = ExceptionInfo;
		eInfo.ClientPointers = FALSE;

		BOOL ret = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hDmpFile, MiniDumpNormal, &eInfo, NULL, NULL);
		if (!ret) {
			qDebug() << QString("写dmp文件失败,error:%1")
				.arg(GetLastError());
		}
		else {
			qDebug() << QString("写dmp文件[%1]完成")
				.arg(QString().toWCharArray(dmpFileName));
		}
	}
	CloseHandle(hDmpFile);
	return EXCEPTION_EXECUTE_HANDLER;
}



#endif
int main(int argc, char *argv[])
{
	
#ifdef DUMP
	// 记录程序异常
	SetUnhandledExceptionFilter(H2SMonitorServerUnhandledExceptionFilter);
	//    for(int index = 0;index < 10; ++index)
	//        int i = 1/index; && "gui" == argv[1]
#endif
	
	QApplication a(argc, argv);
	if (argc >= 2 )
	{
		
		QString str = QString::fromLocal8Bit(argv[1]);
		if ("gui" == str)
		{
			Lifter_server_mscv w;
			w.show();
			return a.exec();
		}
		return 0;
	}
	else
	{
		
		SERVICE_TABLE_ENTRY ServiceTable[2];

		ServiceTable[0].lpServiceName = SERVICE_NAME;
		ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)service_main;

		ServiceTable[1].lpServiceName = NULL;
		ServiceTable[1].lpServiceProc = NULL;
		// 启动服务的控制分派机线程  
		StartServiceCtrlDispatcher(ServiceTable);	
		return 0;
	}

	
	
}
