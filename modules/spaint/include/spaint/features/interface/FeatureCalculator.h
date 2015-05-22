/**
 * spaint: FeatureCalculator.h
 */

#ifndef H_SPAINT_FEATURECALCULATOR
#define H_SPAINT_FEATURECALCULATOR

#include <boost/shared_ptr.hpp>

#include <ITMLib/Objects/ITMScene.h>

#include "../../util/SpaintVoxel.h"

namespace spaint {

/**
 * \brief An instance of a class deriving from this one can be used to calculate feature descriptors for voxels sampled from a scene.
 */
class FeatureCalculator
{
  //#################### DESTRUCTOR ####################
public:
  /**
   * \brief Destroys the feature calculator.
   */
  virtual ~FeatureCalculator() {}

  //#################### PUBLIC ABSTRACT MEMBER FUNCTIONS ####################
public:
  /**
   * \brief Calculates VOP feature descriptors for the specified voxels (grouped by label).
   *
   * \param voxelLocationsMB        A memory block containing the locations of the voxels for which to calculate feature descriptors.
   * \param scene                   The scene.
   * \param featuresMB              A memory block into which to store the calculated feature descriptors (packed sequentially).
   */
  virtual void calculate_features(const ORUtils::MemoryBlock<Vector3s>& voxelLocationsMB,
                                  const ITMLib::Objects::ITMScene<SpaintVoxel,ITMVoxelIndex> *scene,
                                  ORUtils::MemoryBlock<float>& featuresMB) const = 0;

  /**
   * \brief Gets the size of feature vector generated by this feature calculator.
   *
   * \return  The size of feature vector generated by this feature calculator.
   */
  virtual size_t get_feature_count() const = 0;
};

//#################### TYPEDEFS ####################

typedef boost::shared_ptr<const FeatureCalculator> FeatureCalculator_CPtr;

}

#endif
