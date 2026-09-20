#pragma once

#include <QString>

class Logger
{
public:
    static bool initialize();
    static void log(const QString& message);
    static void shutdown();
};
