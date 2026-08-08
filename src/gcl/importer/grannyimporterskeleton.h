#pragma once

#include <vector>

#include "gcl/bindings/bone.h"
#include "gcl/bindings/scene.h"
#include "gcl/importer/grannyformat.h"

namespace GCL::Importer {

///
/// \brief The GrannyImporterSkeleton class.
///
class GrannyImporterSkeleton {
public:
    ///
    /// \brief Constructor
    /// \param scene Scene which needs to be exported.
    ///
    explicit GrannyImporterSkeleton(Bindings::Scene::SharedPtr scene);

    ///
    /// \brief Destructor
    ///
    virtual ~GrannyImporterSkeleton() = default;

    ///
    /// \brief Load all bones from the granny model and return.
    /// \param grannyModel Granny model
    /// \return Bones of the granny model.
    ///
    std::vector<Bindings::Bone::SharedPtr> loadBones(GrannyModel* grannyModel) const;

protected:
    ///
    /// \brief Scene of the importing granny file.
    ///
    Bindings::Scene::SharedPtr m_scene;
};

}  // namespace GCL::Importer
