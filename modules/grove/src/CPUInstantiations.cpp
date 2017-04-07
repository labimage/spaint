/**
 * grove: CPUInstantiations.cpp
 * Copyright (c) Torr Vision Group, University of Oxford, 2017. All rights reserved.
 */

#include "clustering/ExampleClustererFactory.tpp"
#include "clustering/base/Prediction3DColour.h"
#include "clustering/base/Keypoint3DColourClusteringUtils.h"
#include "clustering/cpu/ExampleClusterer_CPU.tpp"
#include "clustering/interface/ExampleClusterer.tpp"

#include "features/FeatureCalculatorFactory.tpp"
#include "features/cpu/RGBDPatchFeatureCalculator_CPU.tpp"
#include "features/interface/RGBDPatchFeatureCalculator.tpp"

#include "forests/DecisionForestFactory.tpp"
#include "forests/cpu/DecisionForest_CPU.tpp"
#include "forests/interface/DecisionForest.tpp"

#include "reservoirs/ExampleReservoirsFactory.tpp"
#include "reservoirs/cpu/ExampleReservoirs_CPU.tpp"
#include "reservoirs/interface/ExampleReservoirs.tpp"

namespace grove {

namespace {
  static const int FOREST_TREES = 5;
}

//#################### EXPLICIT INSTANTIATIONS ####################

template class ExampleClusterer<Keypoint3DColour, Prediction3DColour>;
template class ExampleClusterer_CPU<Keypoint3DColour, Prediction3DColour>;
template class ExampleClustererFactory<Keypoint3DColour, Prediction3DColour>;

template boost::shared_ptr<RGBDPatchFeatureCalculator<Keypoint2D,RGBDPatchDescriptor> >
  FeatureCalculatorFactory::make_custom_patch_feature_calculator<Keypoint2D,RGBDPatchDescriptor>(
    ITMLib::ITMLibSettings::DeviceType deviceType,
    bool depthAdaptive,
    RGBDPatchFeatureCalculatorDifferenceType depthDifferenceType,
    uint32_t depthFeatureCount,
    uint32_t depthFeatureOffset,
    uint32_t depthMinRadius,
    uint32_t depthMaxRadius,
    RGBDPatchFeatureCalculatorDifferenceType rgbDifferenceType,
    uint32_t rgbFeatureCount,
    uint32_t rgbFeatureOffset,
    uint32_t rgbMinRadius,
    uint32_t rgbMaxRadius);
template boost::shared_ptr<RGBDPatchFeatureCalculator<Keypoint3DColour,RGBDPatchDescriptor> >
  FeatureCalculatorFactory::make_custom_patch_feature_calculator<Keypoint3DColour,RGBDPatchDescriptor>(
    ITMLib::ITMLibSettings::DeviceType deviceType,
    bool depthAdaptive,
    RGBDPatchFeatureCalculatorDifferenceType depthDifferenceType,
    uint32_t depthFeatureCount,
    uint32_t depthFeatureOffset,
    uint32_t depthMinRadius,
    uint32_t depthMaxRadius,
    RGBDPatchFeatureCalculatorDifferenceType rgbDifferenceType,
    uint32_t rgbFeatureCount,
    uint32_t rgbFeatureOffset,
    uint32_t rgbMinRadius,
    uint32_t rgbMaxRadius);
template class RGBDPatchFeatureCalculator<Keypoint2D,RGBDPatchDescriptor>;
template class RGBDPatchFeatureCalculator<Keypoint3DColour,RGBDPatchDescriptor>;
template class RGBDPatchFeatureCalculator_CPU<Keypoint2D,RGBDPatchDescriptor>;
template class RGBDPatchFeatureCalculator_CPU<Keypoint3DColour,RGBDPatchDescriptor>;

template class DecisionForest<RGBDPatchDescriptor, FOREST_TREES>;
template class DecisionForest_CPU<RGBDPatchDescriptor, FOREST_TREES>;
template class DecisionForestFactory<RGBDPatchDescriptor, FOREST_TREES>;

template class ExampleReservoirs<Keypoint3DColour>;
template void ExampleReservoirs<Keypoint3DColour>::add_examples(const ExampleImage_CPtr&,
    const boost::shared_ptr<ORUtils::Image<ORUtils::VectorX<int, FOREST_TREES> > >&);
template void ExampleReservoirs<Keypoint3DColour>::add_examples(const ExampleImage_CPtr&,
    const boost::shared_ptr<const ORUtils::Image<ORUtils::VectorX<int, FOREST_TREES> > >&);
template class ExampleReservoirs_CPU<Keypoint3DColour>;
template class ExampleReservoirsFactory<Keypoint3DColour>;

template class ExampleReservoirs<Keypoint2D>;
template void ExampleReservoirs<Keypoint2D>::add_examples(const ExampleImage_CPtr&,
    const boost::shared_ptr<ORUtils::Image<ORUtils::VectorX<int, FOREST_TREES> > >&);
template void ExampleReservoirs<Keypoint2D>::add_examples(const ExampleImage_CPtr&,
    const boost::shared_ptr<const ORUtils::Image<ORUtils::VectorX<int, FOREST_TREES> > >&);
template class ExampleReservoirs_CPU<Keypoint2D>;
template class ExampleReservoirsFactory<Keypoint2D>;

}