/**
 * grove: ExampleClusterer_CPU.tpp
 * Copyright (c) Torr Vision Group, University of Oxford, 2016. All rights reserved.
 */

#include "ExampleClusterer_CPU.h"

#include "../shared/ExampleClusterer_Shared.h"

namespace grove {

//#################### CONSTRUCTORS ####################

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::ExampleClusterer_CPU(float sigma, float tau, uint32_t maxClusterCount, uint32_t minClusterSize)
: ExampleClusterer<ExampleType,ClusterType,MAX_CLUSTERS>(sigma, tau, maxClusterCount, minClusterSize)
{}

//#################### PRIVATE MEMBER FUNCTIONS ####################

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
void ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::compute_cluster_indices(uint32_t exampleSetCapacity, uint32_t exampleSetCount)
{
  int *clusterIndices = this->m_clusterIndices->GetData(MEMORYDEVICE_CPU);
  int *clusterSizes = this->m_clusterSizes->GetData(MEMORYDEVICE_CPU);
  int *parents = this->m_parents->GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
#pragma omp parallel for
#endif
  for(int exampleSetIdx = 0; exampleSetIdx < static_cast<int>(exampleSetCount); ++exampleSetIdx)
  {
    for(uint32_t exampleIdx = 0; exampleIdx < exampleSetCapacity; ++exampleIdx)
    {
      compute_cluster_index(exampleSetIdx, exampleIdx, exampleSetCapacity, parents, clusterIndices, clusterSizes);
    }
  }
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
void ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::compute_cluster_size_histograms(uint32_t exampleSetCapacity, uint32_t exampleSetCount)
{
  int *clusterSizeHistograms = this->m_clusterSizeHistograms->GetData(MEMORYDEVICE_CPU);
  int *clusterSizes = this->m_clusterSizes->GetData(MEMORYDEVICE_CPU);
  int *nbClustersPerExampleSet = this->m_nbClustersPerExampleSet->GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
#pragma omp parallel for
#endif
  for(int exampleSetIdx = 0; exampleSetIdx < static_cast<int>(exampleSetCount); ++exampleSetIdx)
  {
    for(uint32_t clusterIdx = 0; clusterIdx < nbClustersPerExampleSet[exampleSetIdx]; ++clusterIdx)
    {
      update_cluster_size_histogram(exampleSetIdx, clusterIdx, clusterSizes, clusterSizeHistograms, exampleSetCapacity);
    }
  }
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
void ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::compute_densities(const ExampleType *examples, const int *exampleSetSizes,
                                                                                   uint32_t exampleSetCapacity, uint32_t exampleSetCount)
{
  float *densities = this->m_densities->GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
#pragma omp parallel for
#endif
  for(int exampleSetIdx = 0; exampleSetIdx < static_cast<int>(exampleSetCount); ++exampleSetIdx)
  {
    for(uint32_t exampleIdx = 0; exampleIdx < exampleSetCapacity; ++exampleIdx)
    {
      compute_density(exampleSetIdx, exampleIdx, examples, exampleSetSizes, exampleSetCapacity, Base::m_sigma, densities);
    }
  }
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
void ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::compute_parents(const ExampleType *exampleSets, const int *exampleSetSizes, uint32_t exampleSetCapacity,
                                                                                 uint32_t exampleSetCount, float tauSq)
{
  int *clusterIndices = this->m_clusterIndices->GetData(MEMORYDEVICE_CPU);
  const float *densities = this->m_densities->GetData(MEMORYDEVICE_CPU);
  int *nbClustersPerExampleSet = this->m_nbClustersPerExampleSet->GetData(MEMORYDEVICE_CPU);
  int *parents = this->m_parents->GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
#pragma omp parallel for
#endif
  for(int exampleSetIdx = 0; exampleSetIdx < static_cast<int>(exampleSetCount); ++exampleSetIdx)
  {
    for(uint32_t exampleIdx = 0; exampleIdx < exampleSetCapacity; ++exampleIdx)
    {
      compute_parent(
        exampleSetIdx, exampleIdx, exampleSets, exampleSetCapacity, exampleSetSizes,
        densities, tauSq, parents, clusterIndices, nbClustersPerExampleSet
      );
    }
  }
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
void ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::create_selected_clusters(const ExampleType *examples, const int *exampleSetSizes,
                                                                                          uint32_t exampleSetCapacity, uint32_t exampleSetCount,
                                                                                          Clusters *clustersData)
{
  int *clusterIndices = this->m_clusterIndices->GetData(MEMORYDEVICE_CPU);
  int *selectedClusters = this->m_selectedClusters->GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
#pragma omp parallel for
#endif
  for(int exampleSetIdx = 0; exampleSetIdx < static_cast<int>(exampleSetCount); ++exampleSetIdx)
  {
    for(uint32_t selectedClusterIdx = 0; selectedClusterIdx < this->m_maxClusterCount; ++selectedClusterIdx)
    {
      create_selected_cluster(
        exampleSetIdx, selectedClusterIdx, examples, exampleSetSizes, exampleSetCapacity,
        clusterIndices, selectedClusters, this->m_maxClusterCount, clustersData
      );
    }
  }
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
typename ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::Clusters *
ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::get_pointer_to_cluster(const ClustersBlock_Ptr &clusters, uint32_t clusterIdx) const
{
  return clusters->GetData(MEMORYDEVICE_CPU) + clusterIdx;
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
const ExampleType *ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::get_pointer_to_example_set(const ExampleImage_CPtr& exampleSets, uint32_t setIdx) const
{
  const int setCapacity = exampleSets->noDims.width;
  return exampleSets->GetData(MEMORYDEVICE_CPU) + setIdx * setCapacity;
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
const int *ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::get_pointer_to_example_set_size(const ITMIntMemoryBlock_CPtr& exampleSetSizes, uint32_t setIdx) const
{
  return exampleSetSizes->GetData(MEMORYDEVICE_CPU) + setIdx;
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
void ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::reset_cluster_containers(Clusters *clustersData, uint32_t exampleSetCount) const
{
#ifdef WITH_OPENMP
#pragma omp parallel for
#endif
  for(int exampleSetIdx = 0; exampleSetIdx < static_cast<int>(exampleSetCount); ++exampleSetIdx)
  {
    reset_cluster_container(clustersData, exampleSetIdx);
  }
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
void ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::reset_temporaries(uint32_t exampleSetCapacity, uint32_t exampleSetCount)
{
  int *clusterSizeHistograms = this->m_clusterSizeHistograms->GetData(MEMORYDEVICE_CPU);
  int *clusterSizes = this->m_clusterSizes->GetData(MEMORYDEVICE_CPU);
  int *nbClustersPerExampleSet = this->m_nbClustersPerExampleSet->GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
#pragma omp parallel for
#endif
  for(int setIdx = 0; setIdx < static_cast<int>(exampleSetCount); ++setIdx)
  {
    reset_temporaries_for_set(setIdx, exampleSetCapacity, nbClustersPerExampleSet, clusterSizes, clusterSizeHistograms);
  }
}

template <typename ExampleType, typename ClusterType, int MAX_CLUSTERS>
void ExampleClusterer_CPU<ExampleType,ClusterType,MAX_CLUSTERS>::select_clusters(uint32_t exampleSetCapacity, uint32_t exampleSetCount)
{
  const int *clusterSizeHistograms = this->m_clusterSizeHistograms->GetData(MEMORYDEVICE_CPU);
  const int *clusterSizes = this->m_clusterSizes->GetData(MEMORYDEVICE_CPU);
  const int *nbClustersPerExampleSet = this->m_nbClustersPerExampleSet->GetData(MEMORYDEVICE_CPU);
  int *selectedClusters = this->m_selectedClusters->GetData(MEMORYDEVICE_CPU);

#ifdef WITH_OPENMP
#pragma omp parallel for
#endif
  for(int exampleSetIdx = 0; exampleSetIdx < static_cast<int>(exampleSetCount); ++exampleSetIdx)
  {
    select_clusters_for_set(
      exampleSetIdx, clusterSizes, clusterSizeHistograms, nbClustersPerExampleSet,
      exampleSetCapacity, this->m_maxClusterCount, this->m_minClusterSize, selectedClusters
    );
  }
}

}
