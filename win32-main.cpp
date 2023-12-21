#ifdef _WIN32

#include <Windows.h>
#include <shobjidl.h>

#include <string>

#include "convert.h"

// Part of this code comes from Microsoft documentation available at:
// https://learn.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;
        IFileSaveDialog *pFileSave;

        PWSTR f_in_name, f_out_name;
        BOOL f_in_name_allocd = false;
        BOOL f_out_name_allocd = false;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &f_in_name);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        f_in_name_allocd = true;
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }

        // Create the FileSaveDialog object.
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileSave->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileSave->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &f_out_name);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        f_out_name_allocd = true;
                    }
                    pItem->Release();
                }
            }
            pFileSave->Release();
        }

        if (convert(f_in_name, f_out_name) == 0)
            MessageBoxW(NULL, L"Conversion Is Done.", L"Info", MB_OK);
        else
            MessageBoxW(NULL, L"Conversion Failed.", L"Error", MB_OK);

        if(f_in_name_allocd)
            CoTaskMemFree(f_in_name);

        if(f_out_name_allocd)
            CoTaskMemFree(f_out_name);

        CoUninitialize();
    }
    return 0;
}

#endif

