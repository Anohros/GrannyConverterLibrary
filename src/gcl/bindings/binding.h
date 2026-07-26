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
    /// \brief Shared pointer alias of the derived type T
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
    ~Binding() = default;

    friend T;

private:
    Binding() = default;

    ///
    /// \brief Flag to define whether to exclude this binding.
    ///
    bool m_excluded = false;
};

}  // namespace GCL::Bindings
