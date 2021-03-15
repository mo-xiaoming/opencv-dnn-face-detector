set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
set(CMAKE_CUDA_ARCHITECTURES 53 CACHE STRING "CUDA architectures" FORCE)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -G")
    #set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -O0 -g -G)
endif()