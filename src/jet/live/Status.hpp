
#pragma once

#include <set>
#include <string>

namespace jet
{
    struct JetStatus
    {
        std::set<std::string> compilingFiles;
        std::set<std::string> successfulFiles;
        std::set<std::string> failedFiles;
    };
}
