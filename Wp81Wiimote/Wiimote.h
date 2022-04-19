#pragma once
#undef _ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE
#define _ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE 1
#undef WINAPI_FAMILY
#define WINAPI_FAMILY WINAPI_FAMILY_DESKTOP_APP
#define _WIN32_WINNT 0x0603	// Windows 8.1
#define LoadLibraryExW __LoadLibraryExW
#include <windows.h>
#include <bluetoothapis.h>
#include <bthdef.h>
#include <fileapi.h>
#include <rpcdce.h>
#include <string.h>
#include <hidsdi.h>

extern "C" {
	WINBASEAPI
		DWORD
		WINAPI
		QueryDosDeviceW(
			_In_opt_ LPCWSTR lpDeviceName,
			_Out_writes_to_opt_(ucchMax, return) LPWSTR lpTargetPath,
			_In_ DWORD ucchMax
		);

	RPCRTAPI
		_Must_inspect_result_
		RPC_STATUS
		RPC_ENTRY
		UuidFromStringW(
			_In_opt_ RPC_WSTR StringUuid,
			_Out_ UUID __RPC_FAR * Uuid
		);

	WINBASEAPI
	HANDLE
	WINAPI
	CreateFileA(
		_In_ LPCSTR lpFileName,
		_In_ DWORD dwDesiredAccess,
		_In_ DWORD dwShareMode,
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		_In_ DWORD dwCreationDisposition,
		_In_ DWORD dwFlagsAndAttributes,
		_In_opt_ HANDLE hTemplateFile
	);
}

namespace Wiimote
{
	void Debug(const char* format, ...);
	void DebugBtAddress(BLUETOOTH_ADDRESS address);
	void BtAddressString(BLUETOOTH_ADDRESS address, char*buffer, int bufferSize);
	void DebugSystemTime(SYSTEMTIME time);
	void Detect(BLUETOOTH_DEVICE_INFO * device_info);
	void RegisterPairingHandle(BLUETOOTH_DEVICE_INFO * device_info);
	bool BluetoothAuthCallback(LPVOID pvParam, PBLUETOOTH_AUTHENTICATION_CALLBACK_PARAMS  pAuthCallbackParams);
	HANDLE findDeviceHandle();
}