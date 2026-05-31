#pragma once

#include <memory>

namespace GCL::Bindings {

///
/// \brief Base class for bindings.
///
template <typename T>
class Binding {
public:
    ///
    /// \brief Shared pointer alias
    ///
    using SharedPtr = std::shared_ptr<T>;

    ///
    /// \brief Returns if binding is excluded.
    /// \return Exclusion flag
    ///
    [[nodiscard]] bool isExcluded() const {
        return m_excluded;
    }

    ///
    /// \brief Exclude this binding.
    ///
    void exclude() {
        m_excluded = true;
    }

    ///
    /// \brief Include this binding.
    ///
    void include() {
        m_excluded = false;
    }

protected:
    ///
    /// \brief Flag to define whether to exclude this binding.
    ///
    bool m_excluded = false;

    friend T;

private:
    Binding() = default;
};

}  // namespace GCL::Bindings
