#pragma once
#ifdef Q_OS_WIN
#include <windows.h>
#endif

class EncodingFix
{
public:
    EncodingFix()
    {
#ifdef Q_OS_WIN
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
#endif
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    }
};

static EncodingFix encodingFix; // Глобальная инициализация
