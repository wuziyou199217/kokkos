/*
//@HEADER
// ************************************************************************
//
//                        Kokkos v. 2.0
//              Copyright (2014) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#include <gtest/gtest.h>

#include <Kokkos_Core.hpp>
#include <default/TestDefaultDeviceType_Category.hpp>

#if !defined( KOKKOS_ENABLE_CUDA ) || defined( __CUDACC__ )

namespace Test {


TEST_F( TEST_CATEGORY, host_space_access )
{
  typedef Kokkos::HostSpace::execution_space host_exec_space;
  typedef Kokkos::Device< host_exec_space, Kokkos::HostSpace > device_space;
  typedef Kokkos::Impl::HostMirror< Kokkos::DefaultExecutionSpace >::Space mirror_space;

  static_assert(
    Kokkos::Impl::SpaceAccessibility< host_exec_space, Kokkos::HostSpace >::accessible, "" );

  static_assert(
    Kokkos::Impl::SpaceAccessibility< device_space, Kokkos::HostSpace >::accessible, "" );

  static_assert(
    Kokkos::Impl::SpaceAccessibility< mirror_space, Kokkos::HostSpace >::accessible, "" );
}

TEST_F( TEST_CATEGORY, swap_min_max )
{
  // implement bubble sort using our swap()
  int A[] = {5, 1, 4, 2, 8};
  auto n = int(sizeof(A) / sizeof(int));
  bool swapped;
  do {
    swapped = true;
    for (int i = 1; i < n; ++i) {
      if (A[i-1] > A[i]) {
        Kokkos::kokkos_swap(A[i-1], A[i]);
        swapped = false;
      }
    }
    n = n - 1;
  } while (!swapped);
  // check that the result is sorted
  n = int(sizeof(A) / sizeof(int));
  bool is_sorted = true;
  for (int i = 0; i < n - 1; ++i) {
    if (!(A[i] < A[i + 1])) is_sorted = false;
  }
  ASSERT_EQ(is_sorted, true);
  // find the min and max values:
  int min_val = 1000, max_val = -1000;
  for (int i = 0; i < n; ++i) {
    min_val = Kokkos::kokkos_min(min_val, A[i]);
    max_val = Kokkos::kokkos_max(max_val, A[i]);
  }
  ASSERT_EQ(min_val, 1);
  ASSERT_EQ(max_val, 8);
}

} // namespace Test

#endif
