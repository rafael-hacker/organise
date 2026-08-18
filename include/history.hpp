#pragma once
#include 

namespace organise::history {
    // Records a change in the log file.
    void logMove(const std::filesystem::path& original, const std::filesystem::path& destination);
    
    // Reads the log backwards and undo everything
    void undo();
}
