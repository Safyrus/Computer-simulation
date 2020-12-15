/*
#include <Windows.h>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "utils/fileFunction.hpp"

bool print_debug = false;
sf::Font baseFont;

DWORD WINAPI DoStuff(LPVOID lpParameter)
{
    std::cout << "Thread\n";
    // The new thread will start here
    return 0;
}

int main()
{
    // Create a new thread which will start at the DoStuff function
    std::cout << "Create Thread\n";
    HANDLE hThread = CreateThread(
        NULL,    // Thread attributes
        0,       // Stack size (0 = use default)
        DoStuff, // Thread start address
        NULL,    // Parameter to pass to the thread
        0,       // Creation flags
        NULL);   // Thread id
    if (hThread == NULL)
    {
        std::cout << "ERROR\n";
        // Thread creation failed.
        // More details can be retrieved by calling GetLastError()
        return 1;
    }

    // Wait for thread to finish execution
    WaitForSingleObject(hThread, INFINITE);

    // Thread handle must be closed when no longer needed
    CloseHandle(hThread);

    return 0;
}

*/