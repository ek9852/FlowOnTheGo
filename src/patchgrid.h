// Class implements step (3.) in Algorithm 1 of the paper:
// I holds all patch objects on a specific scale, and organizes 1) the dense inverse search, 2) densification

#ifndef PATGRID_HEADER
#define PATGRID_HEADER

#include "patch.h"
#include "oflow.h" // For camera intrinsic and opt. parameter struct


namespace OFC {

  class PatGridClass {

    public:
      PatGridClass(const img_params* _i_params, const opt_params* _op);

      ~PatGridClass();

      void InitializeGrid(const float * _I0, const float * _I0x, const float * _I0y);
      void SetTargetImage(const float * _I1, const float * _I1x, const float * _I1y);
      void InitializeFromCoarserOF(const float * flow_prev);

      void AggregateFlowDense(float *flowout) const;

      // Optimizes grid to convergence of each patch
      void Optimize();
      //Optimize each patch in grid for one iteration, visualize displacement vector, repeat
      //void OptimizeAndVisualize(const float sc_fct_tmp);  // needed for verbosity >= 3, DISVISUAL

      inline const int GetNumPatches() const { return n_patches; }
      inline const int GetNumPatchesW() const { return n_patches_width; }
      inline const int GetNumPatchesH() const { return n_patches_height; }

      inline const Eigen::Vector2f GetRefPatchPos(int i) const { return midpoints_ref[i]; } // Get reference  patch position
      inline const Eigen::Vector2f GetQuePatchPos(int i) const { return patches[i]->GetTargMidpoint(); } // Get target/query patch position
      inline const Eigen::Vector2f GetQuePatchDis(int i) const { return midpoints_ref[i]-patches[i]->GetTargMidpoint(); } // Get query patch displacement from reference patch

    private:

      const float * I0, * I0x, * I0y;
      const float * I1, * I1x, * I1y;

      Eigen::Map<const Eigen::MatrixXf> * I0_eg, * I0x_eg, * I0y_eg;
      Eigen::Map<const Eigen::MatrixXf> * I1_eg, * I1x_eg, * I1y_eg;

      const img_params* i_params;
      const opt_params* op;

      int steps;
      int n_patches_width;
      int n_patches_height;
      int n_patches;

      std::vector<OFC::PatClass*> patches; // Patch Objects
      std::vector<Eigen::Vector2f> midpoints_ref; // Midpoints for reference patches
      std::vector<Eigen::Vector2f> p_init; // starting parameters for query patches
  };

}

#endif /* PATGRID_HEADER */