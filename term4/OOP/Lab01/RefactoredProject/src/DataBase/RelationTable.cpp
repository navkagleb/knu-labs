#include "RelationTable.hpp"

namespace RefactoredProject {

    RelationTable::~RelationTable() noexcept {
        for (const auto& [relation, connections] : m_Table) {
            std::ofstream outfile(
                GetRelationPath(relation.first.GetHash(), relation.second.GetHash()),
                std::fstream::out | std::fstream::binary
            );

            for (const auto& [majorId, minorIds] : connections) {
                auto minorIdsCount = minorIds.size();

                outfile.write(reinterpret_cast<const char*>(&majorId), sizeof(majorId));
                outfile.write(reinterpret_cast<const char*>(&minorIdsCount), sizeof(minorIdsCount));

                for (const auto& minorId : minorIds)
                    outfile.write(reinterpret_cast<const char*>(&minorId), sizeof(minorId));
            }

            outfile.close();
        }
    }

    bool RelationTable::IsExists(const Relation& relation) const {
        return m_Table.contains(relation);
    }

    void RelationTable::DeleteMajor(const Relation& relation, int id) {
        if (!IsExists(relation))
            return;

        m_Table[relation].erase(id);
    }

    void RelationTable::DeleteMinor(const Relation& relation, int majorId, int minorId) {
        if (!IsExists(relation))
            return;

        Print();

        auto& minorIds = m_Table[relation][majorId];

        minorIds.erase(std::find(minorIds.begin(), minorIds.end(), minorId));
    }

    void RelationTable::CreateConnection(const Relation& relation, int majorId, int minorId) {
        m_Table[relation][majorId].emplace_back(minorId);
    }

    void RelationTable::DeleteConnection(const Relation& relation, int majorId, int minorId) {
        auto& connections = m_Table[relation][majorId];

        connections.erase(std::find(connections.begin(), connections.end(), minorId));
    }

    void RelationTable::Print() const {
        for (const auto& [relation, connections] : m_Table) {
            std::cout << "Relation: " << relation.first.GetHash() << " -> " << relation.second.GetHash() << std::endl;

            for (const auto& [majorId, minorIds] : connections) {
                std::cout << majorId << ": ";

                for (const auto& minorId : minorIds)
                    std::cout << minorId << " ";
                std::cout << std::endl;
            }

            std::cout << std::endl;
        }
    }

} // namespace RefactoredProject