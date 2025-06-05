#pragma once
#include <cuda_runtime.h>
#include <cassert>
#include <cstdio>

#ifndef CUDA_CHECK_ERROR
#define CUDA_CHECK_ERROR() \
  do { \
    cudaError_t err_ = cudaGetLastError(); \
    if(err_ != cudaSuccess) { \
      fprintf(stderr, "CUDA error %s:%d: %s\n", __FILE__, __LINE__, cudaGetErrorString(err_)); \
      assert(false); \
    } \
  } while(0)
#endif

static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "CudaBuffer requires little endian");

template <typename T>
class CudaBuffer {
public:
  CudaBuffer() noexcept : ptr_(nullptr), size_(0), host_(false) {}
  ~CudaBuffer() { release(); }

  CudaBuffer(const CudaBuffer&) = delete;
  CudaBuffer& operator=(const CudaBuffer&) = delete;

  CudaBuffer(CudaBuffer&& other) noexcept {
    ptr_ = other.ptr_;
    size_ = other.size_;
    host_ = other.host_;
    other.ptr_ = nullptr;
    other.size_ = 0;
  }

  CudaBuffer& operator=(CudaBuffer&& other) noexcept {
    if(this != &other) {
      release();
      ptr_ = other.ptr_;
      size_ = other.size_;
      host_ = other.host_;
      other.ptr_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  bool allocateDevice(size_t bytes) {
    release();
    host_ = false;
    size_ = bytes;
    cudaError_t err = cudaMalloc(reinterpret_cast<void**>(&ptr_), bytes);
    CUDA_CHECK_ERROR();
    return err == cudaSuccess;
  }

  bool allocateHost(size_t bytes, unsigned int flags = 0) {
    release();
    host_ = true;
    size_ = bytes;
    cudaError_t err = cudaHostAlloc(reinterpret_cast<void**>(&ptr_), bytes, flags);
    CUDA_CHECK_ERROR();
    return err == cudaSuccess;
  }

  void release() {
    if(ptr_) {
      if(host_)
        cudaFreeHost(ptr_);
      else
        cudaFree(ptr_);
      CUDA_CHECK_ERROR();
      ptr_ = nullptr;
      size_ = 0;
    }
  }

  T* get() const noexcept { return ptr_; }
  size_t size() const noexcept { return size_; }

  operator T*() const noexcept { return ptr_; }

private:
  T* ptr_;
  size_t size_;
  bool host_;
};

