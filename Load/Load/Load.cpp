// Load.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "windows.h"
#include "string"
#include "iostream"
#include "fstream"

using namespace std;

int run() {
	HKEY hKEY;
	LPCTSTR data_Set = _T("Software\\Thunder Network\\BHOEnum\\");
	long ret0, ret1;
	DWORD dataType;
	DWORD dataSize;
	WCHAR data[100];


	ret0 = RegOpenKeyEx(HKEY_CURRENT_USER, data_Set, NULL, KEY_READ, &hKEY);
	if (ret0 != ERROR_SUCCESS)        //����޷���hKEY,����ֹ�����ִ��  
	{
		printf("Error #RegOpenKeyEx !!\n");
		system("pause");
		return 1;
	}

	ret1 = RegQueryValueExW(hKEY, _T("Thunder7"), NULL, &dataType, (LPBYTE)data, &dataSize);
	if (ret1 != ERROR_SUCCESS)
	{
		printf("Error #RegQueryValueEx !!\n");
		system("pause");
		return 1;
	}

	printf("Data Type:%d\n", dataType);
	printf("Data Size:%d\n", dataSize);
	printf("·��: ");

	PWCHAR temp = wcsstr(data, L"BHO");
	wcscpy(temp, L"Program\\");
	wprintf(data);
	printf("\n");

	wstring old_path = data;
	old_path += L"XLUserS.dll";
	wstring new_path = data;
	new_path += L"XLUserS1.dll";
	wprintf(L"%ws\n", old_path.c_str());

	DeleteFile(new_path.c_str());
	if (!MoveFile(old_path.c_str(), L"XLUserS1.dll")) {
		printf("û��Ȩ�޻����ļ���ռ�ã��볢���ù���ԱȨ�޻��߹ر�Ѹ��֮������");
		system("pause");
		return 0;
	}

	if (!CopyFile(L"XLUserS.dll", old_path.c_str(), 0)) {
		printf("û��Ȩ�޻����ļ���ռ�ã��볢���ù���ԱȨ�޻��߹ر�Ѹ��֮������");
		system("pause");
		return 0;
	}
	if (!MoveFile(L"XLUserS1.dll", new_path.c_str())) {
		printf("û��Ȩ�޻����ļ���ռ�ã��볢���ù���ԱȨ�޻��߹ر�Ѹ��֮������");
		system("pause");
		return 0;
	}

	printf("�����ɹ�\n");

	system("pause");
	return 0;
}

int restore() {
	HKEY hKEY;
	LPCTSTR data_Set = _T("Software\\Thunder Network\\BHOEnum\\");
	long ret0, ret1;
	DWORD dataType;
	DWORD dataSize;
	WCHAR data[100];


	ret0 = RegOpenKeyEx(HKEY_CURRENT_USER, data_Set, NULL, KEY_READ, &hKEY);
	if (ret0 != ERROR_SUCCESS)        //����޷���hKEY,����ֹ�����ִ��  
	{
		printf("Error #RegOpenKeyEx !!\n");
		system("pause");
		return 1;
	}

	ret1 = RegQueryValueExW(hKEY, _T("Thunder7"), NULL, &dataType, (LPBYTE)data, &dataSize);
	if (ret1 != ERROR_SUCCESS)
	{
		printf("Error #RegQueryValueEx !!\n");
		system("pause");
		return 1;
	}

	printf("Data Type:%d\n", dataType);
	printf("Data Size:%d\n", dataSize);
	printf("·��: ");

	PWCHAR temp = wcsstr(data, L"BHO");
	wcscpy(temp, L"Program\\");
	wprintf(data);
	printf("\n");

	wstring old_path = data;
	old_path += L"XLUserS.dll";
	wstring new_path = data;
	new_path += L"XLUserS1.dll";
	wprintf(L"%ws\n", old_path.c_str());

	wfstream _file;
	_file.open(new_path, ios::in);

	if (!_file) {
		printf("ж��ʧ��1������Ȩ�޻����ļ��Ƿ�ռ�ã������ļ�������\n");
		system("pause");
		return 0;
	}
	_file.close();

	if (!DeleteFile(old_path.c_str())) {
		printf("ж��ʧ��2������Ȩ�޻����ļ��Ƿ�ռ�ã������ļ�������\n");
		system("pause");
		return 0;
	}
	if (!MoveFileW(new_path.c_str(), L"XLUserS1.dll")) {
		printf("ж��ʧ��3������Ȩ�޻����ļ��Ƿ�ռ�ã������ļ�������\n");
		system("pause");
		return 0;
	}
	MoveFile(L"XLUserS1.dll", old_path.c_str());

	system("pause");
	return 0;
}

int main()
{

	printf("���������ֽ��в���\n1:��װ����\n2:ж�ز���\n");
	int n = 0;
	cin >> n;

	if (n == 1)
		run();
	else if (n == 2)
		restore();
	else
		printf("��������\n");
	
    return 0;
}

