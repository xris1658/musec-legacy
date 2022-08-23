#include "base/FixedSizeMemoryPool.hpp"

#include <algorithm>
#include <stdexcept>

namespace Musec::Base
{
FixedSizeMemoryPool::IteratorOfLists& FixedSizeMemoryPool::IteratorOfLists::operator++()
{
    ++poolsIterator;
    ++vacantIterator;
    ++mutexesIterator;
    return *this;
}

FixedSizeMemoryPool::IteratorOfLists& FixedSizeMemoryPool::IteratorOfLists::operator--()
{
    --poolsIterator;
    --vacantIterator;
    --mutexesIterator;
    return *this;
}

FixedSizeMemoryPool::IteratorOfLists FixedSizeMemoryPool::IteratorOfLists::operator++(int)
{
    auto ret = *this;
    operator++();
    return ret;
}

FixedSizeMemoryPool::IteratorOfLists FixedSizeMemoryPool::IteratorOfLists::operator--(int)
{
    auto ret = *this;
    operator--();
    return ret;
}

bool FixedSizeMemoryPool::IteratorOfLists::operator==(const IteratorOfLists& rhs) const
{
    return poolsIterator == rhs.poolsIterator
           && vacantIterator == rhs.vacantIterator
           && mutexesIterator == rhs.mutexesIterator;
}
bool FixedSizeMemoryPool::IteratorOfLists::operator!=(const IteratorOfLists& rhs) const
{
    return !(*this == rhs);
}

FixedSizeMemoryPool::IteratorOfLists FixedSizeMemoryPool::begin()
{
    return {pools_.begin(), vacant_.begin(), mutexes_.begin()};
}

FixedSizeMemoryPool::IteratorOfLists FixedSizeMemoryPool::end()
{
    return {pools_.end(), vacant_.end(), mutexes_.end()};
}

FixedSizeMemoryPool::IteratorOfLists FixedSizeMemoryPool::beforeEnd()
{
    return --end();
}

FixedSizeMemoryPool::FixedSizeMemoryPool(std::size_t memoryBlockSize, std::size_t initialBlockCount):
    memoryBlockSize_(memoryBlockSize),
    initialBlockCount_(initialBlockCount)
{
    pools_.emplace_back(memoryBlockSize_ * initialBlockCount_);
    vacant_.emplace_back(initialBlockCount_, true);
    mutexes_.emplace_back();
}

FixedSizeMemoryPool::~FixedSizeMemoryPool() noexcept(false)
{
    if(!empty())
    {
        throw std::runtime_error("");
    }
}

void FixedSizeMemoryPool::returnMemoryBlock(FixedSizeMemoryPool::IteratorOfLists iterators, std::size_t blockIndex)
{
    auto& [pi, vi, mi] = iterators;
    auto lg = LockGuard(*mi);
    (*vi)[blockIndex] = true;
}

bool FixedSizeMemoryPool::empty() const
{
    return std::all_of(
        vacant_.begin(), vacant_.end(),
        [](const SinglePoolAllocationStatus& status)
        {
            return std::all_of(status.begin(), status.end(), [](bool value) { return value; });
        }
    );
}

bool FixedSizeMemoryPool::full() const
{
    return std::all_of(
        vacant_.begin(), vacant_.end(),
        [](const SinglePoolAllocationStatus& status)
        {
                return std::all_of(status.begin(), status.end(), [](bool value) { return !value; });
        }
    );
}

void FixedSizeMemoryPool::expandPool()
{
    auto blockCount = initialBlockCount_ * (1i64 << pools_.size());
    pools_.emplace_back(memoryBlockSize_ * blockCount);
    vacant_.emplace_back(blockCount, true);
    mutexes_.emplace_back();
}

void* FixedSizeMemoryPool::rawPointerAt(const std::list<SinglePool>::iterator& poolIterator, std::size_t blockIndex)
{
    return reinterpret_cast<void*>(poolIterator->data() + memoryBlockSize_ * blockIndex);
}

std::size_t FixedSizeMemoryPool::memoryBlockSize() const noexcept
{
    return memoryBlockSize_;
}

std::shared_ptr<void> FixedSizeMemoryPool::lendMemoryBlock()
{
    for(auto iterators = begin(); iterators != end(); ++iterators)
    {
        auto& [poolsIterator, vacantIterator, mutexesIterator] = iterators;
        auto lg = LockGuard(*mutexesIterator);
        auto& vacant = *(vacantIterator);
        auto firstVacantIterator = std::find(vacant.begin(), vacant.end(), true);
        if(firstVacantIterator != vacant.end())
        {
            auto index = firstVacantIterator - vacant.begin();
            vacant[index] = false;
            auto rawPointer = rawPointerAt(poolsIterator, index);
            return std::shared_ptr<void>(rawPointer,
                [this, its = iterators, blockIndex = index](void*)
                {
                    this->returnMemoryBlock(its, blockIndex);
                }
            );
        }
    }
    expandPool();
    auto& poolRef = pools_.back();
    auto iterators = beforeEnd();
    std::size_t blockIndex = 0;
    vacant_.back()[0] = false;
    return std::shared_ptr<void>(reinterpret_cast<void*>(pools_.back().data()),
        [this, iterators, blockIndex](void*)
        {
            this->returnMemoryBlock(iterators, blockIndex);
        }
    );
}
}