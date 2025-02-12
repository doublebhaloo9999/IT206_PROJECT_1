if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord))
        {
            throw runtime_error("Error: Unable to set console cursor position.");
     