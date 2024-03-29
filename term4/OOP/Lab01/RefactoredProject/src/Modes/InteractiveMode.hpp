#pragma once

#include "IMode.hpp"
#include "IEntityManager.hpp"

namespace RefactoredProject {

    class InteractiveMode_Impl : public IMode {
    public:
        void Run() override;

        friend class Singleton<InteractiveMode_Impl>;

    private:
        InteractiveMode_Impl();
        ~InteractiveMode_Impl() noexcept override;

        void UpdateCommand();
        [[nodiscard]] bool ParseCommand() const;

        template <typename T>
        void Get(std::istream& istream, T& t);

    private:
        int m_Command;

    }; // class InteractiveMode

    using InteractiveMode = Singleton<InteractiveMode_Impl>;

    template <typename T>
    void InteractiveMode_Impl::Get(std::istream& istream, T& t) {
        istream >> t;
    }

} // namespace RefactoredProject