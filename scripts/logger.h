#pragma once

#include <QString>

class Logger
{
public:
    static bool initialize();
    static void write(const QString& message);
    static void shutdown();
};
