#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include <Tape.hpp>

namespace ng {

    class PolyphaseMergeSort {
    public:
        // constructor / destructor
        PolyphaseMergeSort(std::string dataPath, int filesCount, int chunkSize);
        virtual ~PolyphaseMergeSort();

        // public methods
        void run();

    private:
        // variables
        std::string _dataPath;
        std::vector<Tape*> _tapes;
        int _ctape;
        std::vector<int> _chunk;
        int _level;

        // private methods
        void _initTapes();
        void _updateTapesCapacity();
        void _nullifyTape();
        void _nextTape();

        void _merge(const int& empty);
        void _merge();

    }; // class PolyphaseMergeSort

} // namespace ng