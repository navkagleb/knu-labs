#pragma once

#include <filesystem>
#include <functional>

#include "File.hpp"
#include "Directory.hpp"
#include "DataStructures/Trees/GeneralMapTree.hpp"

namespace fs = std::filesystem;

namespace ng {

    typedef GeneralMapTree<std::string, FileSystemObject*>::Node Node;
    typedef std::function<bool(const FileSystemObject&)> SearchFunc;

    class FileSystem {
    public:
        // constructor / destructor
        FileSystem();
        FileSystem(const std::string& root);
        virtual ~FileSystem();

        // accessors
        const Node* root() const;
        const Node* current() const;

        // public methods
        void pushFile(const std::string& name);
        void pushDirectory(const std::string& name);
        void pop(const std::string& name);
        void move(const std::string& path);

        std::vector<FileSystemObject> search(const std::string& path, SearchFunc func) const;
        void import(const std::string& path);

        void printCurrentPath() const;
        void printCurrentFiles() const;
        void printTree(const Node* node) const;

    private:
        // variables
        GeneralMapTree<std::string, FileSystemObject*>* _tree;
        Node* _currentNode;
        std::vector<std::string> _currentPath;

        // private methods
        void _directoryTraversal(Node* node, std::vector<FileSystemObject>& data, SearchFunc func) const;
        void _directoryTraversal(const fs::path& path, Node* node);

    }; // class FileSystem

} // namespace ng