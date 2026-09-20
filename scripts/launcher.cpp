#include <windows.h>

#include <iostream>
#include <string>

int main()
{
    std::cout << "[START] Static Editor launcher" << std::endl;

    char executablePath[MAX_PATH]{};
    const DWORD length = GetModuleFileNameA(nullptr, executablePath, MAX_PATH);

    if (length == 0 || length >= MAX_PATH)
    {
        std::cerr << "[ERROR] Could not determine launcher path." << std::endl;
        return 1;
    }

    const std::string launcherPath(executablePath, length);
    const std::size_t separator = launcherPath.find_last_of("\\/");

    if (separator == std::string::npos)
    {
        std::cerr << "[ERROR] Could not determine application directory." << std::endl;
        return 1;
    }

    const std::string applicationDirectory = launcherPath.substr(0, separator);
    const std::string editorPath = applicationDirectory + "\\StaticEditorCore.exe";

    std::cout << "[INFO] Application directory: " << applicationDirectory << std::endl;
    std::cout << "[INFO] Editor executable: " << editorPath << std::endl;
    std::cout << "[INFO] DLLs are stored beside the application executables" << std::endl;

    STARTUPINFOA startupInfo{};
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInfo{};

    if (!CreateProcessA(
            editorPath.c_str(),
            nullptr,
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            applicationDirectory.c_str(),
            &startupInfo,
            &processInfo))
    {
        std::cerr << "[ERROR] Could not launch StaticEditorCore.exe. Windows error: "
                  << GetLastError() << std::endl;
        return 1;
    }

    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);

    std::cout << "[OK] StaticEditorCore.exe launched" << std::endl;
    std::cout << "[END] Static Editor launcher" << std::endl;

    return 0;
}
