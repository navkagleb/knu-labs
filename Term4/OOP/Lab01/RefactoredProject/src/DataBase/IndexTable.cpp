#include "IndexTable.hpp"

namespace RefactoredProject {

    IndexTable::~IndexTable() {
        for (const auto& [hash, entitiesData] : m_Table) {
            std::ofstream outfile(TypeInfo::GetIndexTablePath(hash), std::fstream::out | std::fstream::binary);

            for (const auto& entityData : entitiesData)
                outfile.write(reinterpret_cast<const char*>(&entityData), sizeof(entityData));

            outfile.close();
        }
    }

    void IndexTable::Print() const {
        std::cout << "Size: " << m_Table.size() << std::endl;
        for (const auto& [hash, pairs] : m_Table) {
            std::cout << "Pairs: " << pairs.size() << std::endl;
            for (const auto& pair : pairs)
                std::cout << pair.first << " -> " << pair.second << std::endl;

            std::cout << std::endl;
        }
    }

} // namespace RefactoredProject