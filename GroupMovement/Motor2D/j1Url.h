#ifndef __J1URL_H__
#define __J1URL_H__

#include <WinUser.h>
#include <shellapi.h>

void openUrl(const char* url)
{
	ShellExecute(GetActiveWindow(), "open", url, NULL, NULL, SW_SHOWNORMAL);
}

#endif // !__J1URL_H__
