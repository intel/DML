/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_UTILS_HPP
#define DML_TESTING_UTILS_HPP

#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>
#include <ostream>
#include <utility>
#include <iostream>

namespace dml::testing
{
    namespace detail
    {
        template <typename pointer_t>
        inline pointer_t* align(pointer_t* pointer, std::uint32_t alignment) noexcept
        {
            auto align_offset = static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(pointer) &
                                                         (alignment - 1));
            if (align_offset != 0)
            {
                align_offset = alignment - align_offset;
            }

            return reinterpret_cast<pointer_t*>(reinterpret_cast<std::uint8_t*>(pointer) +
                                                align_offset);
        }
    }  // namespace detail

    class RawMemory
    {
    public:
        RawMemory(): RawMemory(0, 0)
        {
        }

        explicit RawMemory(std::uint32_t size, std::uint32_t alignment):
            ptr_(nullptr),
            aligned_ptr_(nullptr),
            size_(size),
            alignment_(alignment)
        {
            if (size_)
            {
                ptr_ = static_cast<std::uint8_t*>(std::malloc(size_ + alignment_));

                if (ptr_)
                {
                    for (auto i = 0u; i < size_ + alignment_; ++i)
                    {
                        ptr_[i] = 0xFF;
                    }
                    aligned_ptr_ = detail::align(ptr_, alignment_);
                }
                else
                {
                    throw std::bad_alloc();
                }
            }
        }

        ~RawMemory() noexcept
        {
            if (ptr_)
            {
                std::free(ptr_);
            }
        }

        RawMemory(const RawMemory& other) = delete;

        RawMemory(RawMemory&& other) noexcept:
            ptr_(std::exchange(other.ptr_, nullptr)),
            aligned_ptr_(std::exchange(other.aligned_ptr_, nullptr)),
            size_(std::exchange(other.size_, 0u)),
            alignment_(std::exchange(other.alignment_, 0u))
        {
        }

        RawMemory& operator=(const RawMemory& other) = delete;

        RawMemory& operator=(RawMemory&& other) noexcept
        {
            if (this != &other)
            {
                std::swap(ptr_, other.ptr_);
                std::swap(aligned_ptr_, other.aligned_ptr_);
                std::swap(size_, other.size_);
                std::swap(alignment_, other.alignment_);
            }

            return *this;
        }

        [[nodiscard]] auto data() noexcept
        {
            return aligned_ptr_;
        }

        [[nodiscard]] bool operator==(const RawMemory& other) const noexcept
        {
            if (this->size_ != other.size_)
            {
                return false;
            }

            return std::equal(aligned_ptr_, aligned_ptr_ + size_, other.aligned_ptr_);
        }

        friend std::ostream& operator<<(std::ostream& os, const RawMemory& memory)
        {
            if (memory.size_ > 16u)
            {
                os << "<too long to print>";
            }
            else
            {
                std::for_each(memory.aligned_ptr_,
                              memory.aligned_ptr_ + memory.size_,
                              [&](auto value)
                              {
                                  os << std::hex << static_cast<int>(value) << ' ';
                              });
            }

            return os;
        }

    private:
        std::uint8_t* ptr_;
        std::uint8_t* aligned_ptr_;
        std::uint32_t size_;
        std::uint32_t alignment_;
    };

    class MemoryRegion
    {
    public:
        MemoryRegion(std::uint8_t* memory, std::uint32_t size) noexcept: data_(memory), size_(size)
        {
            std::uninitialized_default_construct(data_, data_ + size_);
        }

        ~MemoryRegion() noexcept
        {
            std::destroy(data_, data_ + size_);
        }

        MemoryRegion(const MemoryRegion& other) = delete;

        MemoryRegion(MemoryRegion&& other) noexcept:
            data_(std::exchange(other.data_, nullptr)),
            size_(std::exchange(other.size_, 0u))
        {
        }

        MemoryRegion& operator=(const MemoryRegion& other) = delete;

        MemoryRegion& operator=(MemoryRegion&& other) noexcept
        {
            if (this != &other)
            {
                std::swap(this->data_, other.data_);
                std::swap(this->size_, other.size_);
            }

            return *this;
        }

        [[nodiscard]] auto begin() noexcept
        {
            return data_;
        }

        [[nodiscard]] auto end() noexcept
        {
            return data_ + size_;
        }

        [[nodiscard]] auto data() noexcept
        {
            return data_;
        }

        [[nodiscard]] auto size() const noexcept
        {
            return size_;
        }

        bool operator==(const MemoryRegion& other) const noexcept
        {
            if (this->size_ != other.size_)
            {
                return false;
            }

            return std::equal(data_, data_ + size_, other.data_);
        }

        friend std::ostream& operator<<(std::ostream& os, const MemoryRegion& region)
        {
            std::for_each(region.data_,
                          region.data_ + region.size_,
                          [&](auto value)
                          {
                              os << std::hex << static_cast<int>(value) << ' ';
                          });
            os << '\n';

            return os;
        }

    private:
        std::uint8_t* data_;
        std::uint32_t size_;
    };

    class MemoryBuilder;

    class RegionConfiguration
    {
        friend MemoryBuilder;

    public:
        RegionConfiguration() noexcept: size_(1), alignment_(1), offset_(0), distance_(0)
        {
        }

        auto& set_size(std::uint32_t size) noexcept
        {
            size_ = size;

            return *this;
        }

        auto& set_alignment(std::uint32_t alignment) noexcept
        {
            alignment_ = alignment;

            return *this;
        }

        // This offset in step, where size of step is alignment
        // Use to test overlapping
        // Can't be used with distance
        auto& set_offset(std::int32_t offset)
        {
            if (distance_ != 0)
                throw std::logic_error("Offset shall not be used with distance");

            offset_ = offset;

            return *this;
        }

        // Distance between previous region's begin and this region's begin in bytes
        // Use to tweak distance between BEGINs of memory regions
        // Can't be used with offset
        auto& set_distance(std::uint32_t distance)
        {
            if (offset_ != 0)
                throw std::logic_error("Distance shall not be used with offset");

            distance_ = distance;

            return *this;
        }

    private:
        std::uint32_t size_;
        std::uint32_t alignment_;
        std::int32_t  offset_;
        std::uint32_t distance_;
    };

    class Memory
    {
        friend MemoryBuilder;

    public:
        explicit Memory(RawMemory&& mem, std::vector<MemoryRegion>&& mapping) noexcept:
            memory_(std::move(mem)),
            mapping_(std::move(mapping))
        {
        }

        [[nodiscard]] auto& get_region(std::size_t index) noexcept
        {
            return mapping_.at(index);
        }

        [[nodiscard]] bool operator==(const Memory& rhs) const
        {
            return memory_ == rhs.memory_;
        }

        friend std::ostream& operator<<(std::ostream& os, const Memory& memory)
        {
            os << memory.memory_;
            return os;
        }

    private:
        RawMemory                 memory_;
        std::vector<MemoryRegion> mapping_;
    };

    class MemoryBuilder
    {
    public:
        MemoryBuilder() = default;

        auto& add_region(const RegionConfiguration& configuration)
        {
            configurations_.push_back(configuration);

            return *this;
        }

        [[nodiscard]] Memory build() const;

    private:
        std::vector<RegionConfiguration> configurations_;
    };

}  // namespace dml::testing

#endif  //DML_TESTING_UTILS_HPP
