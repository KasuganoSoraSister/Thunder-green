// Load.cpp : 定义控制台应用程序的入口点。
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
	if (ret0 != ERROR_SUCCESS)        //如果无法打开hKEY,则中止程序的执行  
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
	printf("路径: ");

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
		printf("没有权限或者文件被占用，请尝试用管理员权限或者关闭迅雷之后重试");
		system("pause");
		return 0;
	}

	if (!CopyFile(L"XLUserS.dll", old_path.c_str(), 0)) {
		printf("没有权限或者文件被占用，请尝试用管理员权限或者关闭迅雷之后重试");
		system("pause");
		return 0;
	}
	if (!MoveFile(L"XLUserS1.dll", new_path.c_str())) {
		printf("没有权限或者文件被占用，请尝试用管理员权限或者关闭迅雷之后重试");
		system("pause");
		return 0;
	}

	printf("操作成功\n");

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
	if (ret0 != ERROR_SUCCESS)        //如果无法打开hKEY,则中止程序的执行  
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
	printf("路径: ");

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
		printf("卸载失败1，请检查权限或者文件是否被占用，或者文件不存在\n");
		system("pause");
		return 0;
	}
	_file.close();

	if (!DeleteFile(old_path.c_str())) {
		printf("卸载失败2，请检查权限或者文件是否被占用，或者文件不存在\n");
		system("pause");
		return 0;
	}
	if (!MoveFileW(new_path.c_str(), L"XLUserS1.dll")) {
		printf("卸载失败3，请检查权限或者文件是否被占用，或者文件不存在\n");
		system("pause");
		return 0;
	}
	MoveFile(L"XLUserS1.dll", old_path.c_str());

	system("pause");
	return 0;
}

int main()
{

	printf("请输入数字进行操作\n1:安装补丁\n2:卸载补丁\n");
	int n = 0;
	cin >> n;

	if (n == 1)
		run();
	else if (n == 2)
		restore();
	else
		printf("输入有误\n");
	
    return 0;
}

