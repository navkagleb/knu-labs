#include "FileSystem/FileSystem.hpp"

#include <sys/stat.h>
#include <chrono>

namespace fs = std::filesystem;
namespace ch = std::chrono;

namespace ng {

    // constructor / destructor
    FileSystem::FileSystem() : _tree(new GeneralMapTree<std::string, FileSystemObject*>), _currentNode(nullptr) {}

    FileSystem::FileSystem(const std::string& root) {

        this->_tree = new GeneralMapTree<std::string, FileSystemObject*>(new Directory(root));
        this->_currentNode = this->_tree->root();

    }

    FileSystem::~FileSystem() { delete this->_tree; }

    // accessors
    const Node* FileSystem::root() const { return this->_tree->root(); }

    const Node* FileSystem::current() const { return this->_currentNode; }

    // public methods
    void FileSystem::pushFile(const std::string& name) {

        this->_tree->push(name, new File(this->_currentNode->value()->path() / name), this->_currentNode);

    }

    void FileSystem::pushDirectory(const std::string& name) {

        if (this->_currentNode)
            this->_tree->push(name, new Directory(this->_currentNode->value()->path() / name), this->_currentNode);
        else
            this->_tree->push(name, new Directory(name), this->_currentNode);

        if (!this->_currentNode)
            this->_currentNode = this->_tree->root();

    }

    void FileSystem::pop(const std::string& name) { this->_tree->pop(name, this->_currentNode); }

    void FileSystem::move(const std::string& path) {

        if (path == "..") {

            if (this->_currentNode == this->_tree->root()) {

                std::cerr << "current pos is root" << std::endl;
                return;

            }

            this->_currentPath.pop_back();
            this->_currentNode = this->_currentNode->parent();
            return;

        }

        if (this->_currentNode->children().count(path) == 1) {

            this->_currentPath.emplace_back(path);
            this->_currentNode = this->_currentNode->children().at(path);

        } else {

            std::cerr << "no such path" << std::endl;

        }

    }

    std::vector<FileSystemObject> FileSystem::search(const std::string& path, SearchFunc func) const {

        std::vector<FileSystemObject> data;
        Node* root = this->_tree->root();

        if (long long start = 0, end; root && !path.empty() && path != ".") {

            for (std::size_t i = 0, depth = std::count(path.begin(), path.end(), '/'); i <= depth; ++i) {

                end = path.find("/");

                if (std::string current = path.substr(start, end); root->children().count(current) == 1)
                    root = root->children().at(current);
                else
                    return {};

                start = end + 1;

            }

        }

        this->_directoryTraversal(root, data, func);

        return data;
    }

    void FileSystem::import(const std::string& path) {

        fs::path workdir;

        workdir = path == "." ? fs::current_path().parent_path() : path;

        if (!this->_tree->empty())
            delete this->_tree;

        struct stat info;
        stat(workdir.string().c_str(), &info);

        this->_tree = new GeneralMapTree<std::string, FileSystemObject*>(new Directory(
            workdir.filename().string(),
            DateTime(&info.st_ctime)
        ));

        Node* root = this->_tree->root();

        this->_directoryTraversal(workdir, root);

    }

    void FileSystem::printCurrentPath() const {

        for (std::size_t i = 0; i < this->_currentPath.size() - 1; ++i)
            std::cout << this->_currentPath[i] << "/";

        std::cout << this->_currentPath.back() << "~" << std::endl;

    }

    void FileSystem::printCurrentFiles() const {

        for (const auto& [key, child] : this->_currentNode->children())
            std::cout << std::setw(10) << key;

        std::cout << std::endl;

    }

    void FileSystem::printTree(const Node* node) const {

        this->_tree->tprint(node);

    }

    // private methods
    void FileSystem::_directoryTraversal(Node* node, std::vector<FileSystemObject>& data, SearchFunc func) const {

        for (const auto& [key, child] : node->children()) {

            if (func(*child->value()))
                data.emplace_back(*child->value());

            if (child->value()->type() == FileSystemObject::Type::Directory)
                this->_directoryTraversal(child, data, func);

        }

    }

    void FileSystem::_directoryTraversal(const fs::path& path, Node* parent) {

        for (const auto& elem : fs::directory_iterator(path)) {

            struct stat info;
            stat(elem.path().string().c_str(), &info);

            if (elem.is_directory()) {

                Node* node = this->_tree->push(
                    elem.path().filename().string(),
                    new Directory(
                        parent->value()->path() / elem.path().filename(),
                        DateTime(&info.st_ctime)
                    ),
                    parent
                );
                this->_directoryTraversal(elem.path(), node);

            } else {

                this->_tree->push(
                    elem.path().filename().string(),
                    new File(
                        parent->value()->path() / elem.path().filename(),
                        info.st_size,
                        DateTime(&info.st_ctime),
                        DateTime(&info.st_mtime)
                    ),
                    parent
                );

            }

        }

    }

} // namespace ng