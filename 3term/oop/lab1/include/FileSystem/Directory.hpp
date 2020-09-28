#pragma once

#include "FileSystemObject.hpp"

namespace ng {

    class Directory : public FileSystemObject {
    public:
        // constructor / destructor
        Directory();
        Directory(const fs::path& path);
        Directory(const fs::path& path, const Time& creationTime, const Date& creationDate);

        // public methods

    }; // class File

} // namespace ng