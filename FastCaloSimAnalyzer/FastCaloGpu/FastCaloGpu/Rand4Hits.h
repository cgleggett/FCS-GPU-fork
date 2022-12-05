/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#ifndef RAND4HITS_H
#define RAND4HITS_H

#include <vector>
#include <random>

#ifdef USE_KOKKOS
#include <Kokkos_Core.hpp>
#include <Kokkos_Random.hpp>
#endif

#ifdef USE_STDPAR
#include <atomic>
#endif

#include "GpuGeneral_structs.h"

class Rand4Hits {
public:
  Rand4Hits() {
    m_rand_ptr = 0;
    m_total_a_hits = 0;
  };
  ~Rand4Hits();

  float *rand_ptr(int nhits) {
    if (over_alloc(nhits)) {
      rd_regen();
      return m_rand_ptr;
    } else {
      float *f_ptr = &(m_rand_ptr[3 * m_current_hits]);
      return f_ptr;
    }
  };
  float *rand_ptr_base() { return m_rand_ptr; }
  void set_rand_ptr(float *ptr) {
    m_rand_ptr = ptr;
  };
  void set_t_a_hits(int nhits) {
    m_total_a_hits = nhits;
  };
  void set_c_hits(int nhits) {
    m_current_hits = nhits;
  };
  unsigned int get_c_hits() {
    return m_current_hits;
  };
  unsigned int get_t_a_hits() {
    return m_total_a_hits;
  };

  void create_gen(unsigned long long seed, size_t numhits, bool useCPU = false);

  void allocate_simulation(int maxbins, int maxhitct, unsigned long n_cells);

#ifdef USE_STDPAR
  void deallocate();
#endif

  CELL_ENE_T *get_cells_energy() {
    return m_cells_energy;
  };
  Cell_E *get_cell_e() {
    return m_cell_e;
  };
  Cell_E *get_cell_e_h() {
    return m_cell_e_h;
  };

  CELL_CT_T *get_ct() {
    return m_ct;
  };
  int *get_ct_h() {
    return m_ct_h;
  };

  unsigned long *get_hitcells() {
    return m_hitcells;
  };
  int *get_hitcells_ct() {
    return m_hitcells_ct;
  };

  // void       set_hitparams_h( HitParams* hp ) { m_hitparams_h = hp; }

  HitParams *get_hitparams() {
    return m_hitparams;
  };
  // HitParams* get_hitparams_h() { return m_hitparams_h; };
  long *get_simbins() {
    return m_simbins;
  };

#ifdef USE_KOKKOS
  Kokkos::View<HitParams *> *get_hitparams_v() {
    return &m_hitparams_v;
  };
  Kokkos::View<long *> *get_simbins_v() {
    return &m_simbins_v;
  };
#endif

  void rd_regen();

  void add_a_hits(int nhits) {
    if (over_alloc(nhits))
      m_current_hits = nhits;
    else
      m_current_hits += nhits;
  };
  bool over_alloc(int nhits) {
    return m_current_hits + nhits > m_total_a_hits;
  }; // return true if hits over spill, need regenerat rand..

private:
  float *genCPU(size_t num);
  void createCPUGen(unsigned long long seed);
  void allocateGenMem(size_t num);
  void destroyCPUGen();

  float *m_rand_ptr{ nullptr };
  unsigned int m_total_a_hits;
  unsigned int m_current_hits;
  void *m_gen{ nullptr };
  bool m_useCPU{ false };

  // patch in some GPU pointers for cudaMalloc
  CELL_ENE_T *m_cells_energy{ 0 };
  Cell_E *m_cell_e{ 0 };
  CELL_CT_T *m_ct{ 0 };

  HitParams *m_hitparams; // on device
  // HitParams* m_hitparams_h;                      // on host
  long *m_simbins;

  // host side ;
  unsigned long *m_hitcells{ nullptr };
  int *m_hitcells_ct{ nullptr };
  Cell_E *m_cell_e_h{ nullptr };
  int *m_ct_h;

  std::vector<float> *m_rnd_cpu{ nullptr };

#ifdef USE_KOKKOS
  Kokkos::View<CELL_ENE_T *> m_cells_energy_v;
  Kokkos::View<Cell_E *> m_cell_e_v;
  Kokkos::View<int *> m_ct_v;
  Kokkos::View<float *> m_rand_ptr_v;
  Kokkos::View<long *> m_simbins_v;
  Kokkos::View<HitParams *> m_hitparams_v;
#endif
};

#endif
