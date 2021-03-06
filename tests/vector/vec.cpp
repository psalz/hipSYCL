/*
 * This file is part of hipSYCL, a SYCL implementation based on CUDA/HIP
 *
 * Copyright (c) 2018 Aksel Alpay
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>

#define SYCL_SIMPLE_SWIZZLES
#include <CL/sycl.hpp>


void print_vector(const cl::sycl::vec<float,1>& v)
{
  printf("%f\n", v.x());
}


void print_vector(const cl::sycl::vec<float,2>& v)
{
  printf("%f %f\n", v.x(), v.y());
}


void print_vector(const cl::sycl::vec<float,3>& v)
{
  printf("%f %f %f\n", v.x(), v.y(), v.z());
}


void print_vector(const cl::sycl::vec<float,4>& v)
{
  printf("%f %f %f %f\n", v.x(), v.y(), v.z(), v.w());
}

void print_vector(const cl::sycl::vec<float,8>& v)
{
  printf("%f %f %f %f %f %f %f %f\n",
         v.s0(), v.s1(), v.s2(), v.s3(),
         v.s4(), v.s5(), v.s6(), v.s7());
}


void print_vector(const cl::sycl::vec<int,8>& v)
{
  printf("%d %d %d %d %d %d %d %d\n",
         v.s0(), v.s1(), v.s2(), v.s3(),
         v.s4(), v.s5(), v.s6(), v.s7());
}

int main()
{
  cl::sycl::queue q;

  q.submit([&](cl::sycl::handler& cgh) {
    cgh.single_task<class vec_test>(
          [=] () {

      cl::sycl::vec<float, 4> v1(1.0f);
      cl::sycl::vec<float, 8> v2(1.f, 2.f, 3.f, v1, 4.f);

      print_vector(v1);
      print_vector(v2);

      // Broadcasting math functions over vector elements
      print_vector(cl::sycl::sin(v2));

      // Should cause error:
      //cl::sycl::vec<float, 4> v3(1.0f, 2.0f);
      //cl::sycl::vec<float, 4> v4(1.0f, 2.0f, v2);

      v2 = cl::sycl::fma(v2, v2, v2);
      print_vector(v2 + v2);
      print_vector(cl::sycl::cos(v2));
      print_vector(cl::sycl::sin(v2));
      print_vector(cl::sycl::cos(v2) < cl::sycl::sin(v2));

      // Swizzles!
      cl::sycl::vec<float, 4> v2_lo = v2.lo();
      //cl::sycl::vec<float, 4> v2_lo(1.f, 2.f, 3.f, 4.f);
      print_vector(v2_lo);

      cl::sycl::vec<float, 4> v3 = v2_lo.wzyx();
      print_vector(v3);
      v3.xwyz() = v2_lo.xxyy();
      print_vector(v3);
      // Nested swizzles
      v3.yxzw().lo() = v2_lo.xxyy().hi();
      print_vector(v3);

    });
  });
}


