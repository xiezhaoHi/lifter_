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
	// ����dmpĿ¼
	QString strDmpDirName(QStringLiteral("./dmp"));
	QDir dmpDir(strDmpDirName);
	if (!dmpDir.exists()) {
		if (!dmpDir.mkdir(strDmpDirName)) {
			qDebug() << QString("����dmpĿ¼ʧ��,error:%1")
				.arg(GetLastError());
			MessageBox(NULL, L"���ֶ��ڳ����Ŀ¼����һ�� dmp �ļ��� ", L"", MB_OK);
			return EXCEPTION_CONTINUE_SEARCH;
		}
	}
	//MessageBox(NULL, L"��������,������dump�ļ�,�뼰ʱ��dmp�ļ��д�����ڼ�����Ա!", L"������ʾ", MB_OK);

	wchar_t dmpFileName[MAX_PATH] = { 0 };
	SYSTEMTIME st;
	GetLocalTime(&st);
	swprintf(dmpFileName, L"dmp/%04d%02d%02d_%02d%02d%02d_%03d.dmp",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	// ����dmp�ļ�
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
			qDebug() << QString("дdmp�ļ�ʧ��,error:%1")
				.arg(GetLastError());
		}
		else {
			qDebug() << QString("дdmp�ļ�[%1]���")
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
	// ��¼�����쳣
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
		// ��������Ŀ��Ʒ��ɻ��߳�  
		StartServiceCtrlDispatcher(ServiceTable);	
		return 0;
	}

	
	
}
