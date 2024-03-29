#pragma once

#include "BPlusNode.hpp"

namespace Ng {

    template <typename Key>
    class BPlusInternalNode final : public BPlusNode<Key> {
    public:
        using ChildContainer     = std::vector<BPlusNode<Key>*>;
        using ChildIterator      = typename ChildContainer::iterator;

    private:
        using BPlusNode = BPlusNode<Key>;

    public:
        explicit BPlusInternalNode(BPlusInternalNode* parent = nullptr);
        ~BPlusInternalNode() override;

        [[nodiscard]] inline std::size_t GetChildCount() const { return m_Children.size(); }
        [[nodiscard]] inline BPlusNode* GetChildByIndex(std::size_t index) const { return m_Children[index]; }
        [[nodiscard]] inline BPlusNode* GetMinChild() const { return m_Children.front(); }
        [[nodiscard]] inline BPlusNode* GetMaxChild() const { return m_Children.back(); }

        [[nodiscard]] BPlusNode* GetChildByKey(const Key& key) const;

        void UpdateKeys();

        void PushChild(BPlusNode* child);
        void PopChild(BPlusNode* child);

        BPlusNode* Split() override;

        void BorrowLeft() override;
        void BorrowRight() override;
        void MergeLeft() override;
        void MergeRight() override;

        BPlusNode* GetMinNode() override;
        BPlusNode* GetMaxNode() override;

        const BPlusNode* GetMinNode() const override;
        const BPlusNode* GetMaxNode() const override;

    private:
        [[nodiscard]] BPlusNode* GetLeftSibling(ChildIterator it) const;
        [[nodiscard]] BPlusNode* GetRightSibling(ChildIterator it) const;
        [[nodiscard]] ChildIterator GetChildMedianIterator();

        void PushChild(ChildIterator it, BPlusNode* child);

    private:
        ChildContainer m_Children;

    }; // class BPlusInternalNode

} // namespace Ng

#include "BPlusInternalNode.inl"