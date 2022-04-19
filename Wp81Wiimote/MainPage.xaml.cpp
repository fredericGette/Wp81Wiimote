//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include <ppltasks.h>
#include "MainPage.xaml.h"
#include "Wiimote.h"

using namespace Wp81Wiimote;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;
using namespace std;
using namespace Windows::UI::Core;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter

	// TODO: Prepare page for display here.

	// TODO: If your application contains multiple pages, ensure that you are
	// handling the hardware Back button by registering for the
	// Windows::Phone::UI::Input::HardwareButtons.BackPressed event.
	// If you are using the NavigationHelper provided by some templates,
	// this event is handled for you.

	Refresh();
}

void MainPage::AppBarButton_Click(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{
	Button^ b = (Button^)sender;
	if (b->Tag->ToString() == "Refresh")
	{
		// Refresh
		Refresh();
	}
}

void MainPage::Refresh()
{
	TextBoxWiimoteFound->Text = L"Looking for Wiimote...\n";

	// Allocate memory with the new keyword
	// don't use the stack because we want to use the BLUETOOTH_DEVICE_INFO outside lambdas
	BLUETOOTH_DEVICE_INFO* deviceInfo = new BLUETOOTH_DEVICE_INFO;
	ZeroMemory(deviceInfo, sizeof(BLUETOOTH_DEVICE_INFO));
	deviceInfo->dwSize = sizeof(BLUETOOTH_DEVICE_INFO);

	create_task([this, deviceInfo]()
	{
		try
		{
			Wiimote::Detect(deviceInfo);

			Dispatcher->RunAsync(
				CoreDispatcherPriority::Normal,
				ref new DispatchedHandler([this, deviceInfo]()
			{
				TextBoxWiimoteFound->Text += L"\n";

				// Allocate memory with the ref new keyword
				TextBoxWiimoteFound->Text += ref new Platform::String(deviceInfo->szName);

				// Allocate memory in the stack, will be free at the end of this lambda
				char address[18]; // length 18 = xx:xx:xx:xx:xx:xx[0] 
				ZeroMemory(address, sizeof(address));
				Wiimote::BtAddressString(deviceInfo->Address, address, sizeof(address));
				char16 address16[18];
				// copy an array of char into an array of char16
				transform(address, address + 18, address16, [](char c) { return (char16)c; });

				TextBoxWiimoteFound->Text += L" (";
				TextBoxWiimoteFound->Text += ref new Platform::String(address16);
				TextBoxWiimoteFound->Text += L")";

				if (deviceInfo->fAuthenticated != FALSE)
				{
					// Wiimote already paired
					TextBoxWiimoteFound->Text += L"Wiimote is paired.\n";

					if (deviceInfo->fConnected != FALSE)
					{
						TextBoxWiimoteFound->Text += L"Wiimote is connected.\n";
						create_task([deviceInfo]()
						{
							HANDLE hDevice = INVALID_HANDLE_VALUE;
							hDevice = Wiimote::findDeviceHandle();

							WCHAR ProductName[255];
							ZeroMemory(ProductName, sizeof(ProductName));
							if (HidD_GetProductString(hDevice, ProductName, 255))
							{
								OutputDebugString(L"HID Name :"); OutputDebugString(ProductName); OutputDebugString(L"\n");
							}
						});
					}
					else
					{
						TextBoxWiimoteFound->Text += L"Wiimote is not connected.\n";
						TextBoxWiimoteFound->Text += L"Press buttons 1+2 of the Wiimote.\n";
						TextBoxWiimoteFound->Text += L"If Wiimote refuses to connect: Delete the Wiimote in the Bluetooth settings and restart the procedure.\n";
					}
				}
				else
				{
					// Wiimote not paired
					TextBoxWiimoteFound->Text += L"Wiimote is not paired.\n";
					create_task([this, deviceInfo]()
					{
						Wiimote::RegisterPairingHandle(deviceInfo);

						Dispatcher->RunAsync(
							CoreDispatcherPriority::Normal,
							ref new DispatchedHandler([this]()
						{
							// Do stuff on the UI Thread

							TextBoxWiimoteFound->Text += L"Pairing handle registred.\n";
							TextBoxWiimoteFound->Text += L"Please go to the Bluetooth settings to pair the Wiimote:\n";
							TextBoxWiimoteFound->Text += L"1a-If the Wiimote line is not visible: Press buttons 1+2 of the Wiimote.\n";
							TextBoxWiimoteFound->Text += L"1b-If the status is 'pairing'; Tap Wiimote line to switch from 'pairing' to 'tap to pair'.\n";
							TextBoxWiimoteFound->Text += L"2-Tap Wiimote line to open pairing window.\n";
							TextBoxWiimoteFound->Text += L"3-Tap 'cancel' to close the pairing window.\n";
						}));
					});
				}
			}));
		}
		catch (char* reason)
		{
			Dispatcher->RunAsync(
				CoreDispatcherPriority::Normal,
				ref new DispatchedHandler([this, reason]()
			{
				// Do stuff on the UI Thread
				TextBoxWiimoteFound->Text += L"\n";
				char16 reason16[100];
				ZeroMemory(reason16, sizeof(reason16));
				transform(reason, reason + strlen(reason), reason16, [](char c) { return (char16)c; });
				TextBoxWiimoteFound->Text += ref new Platform::String(reason16);
			}));
		}
	});
}
