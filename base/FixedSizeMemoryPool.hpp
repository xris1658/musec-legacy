#ifndef MUSEC_BASE_FIXEDSIZEMEMORYPOOL
#define MUSEC_BASE_FIXEDSIZEMEMORYPOOL

#include "base/FixedSizeMemoryBlock.hpp"

#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace Musec
{
namespace Base
{
class FixedSizeMemoryPool
{
private:
    using SinglePool = Musec::Base::FixedSizeMemoryBlock;
    using SinglePoolAllocationStatus = std::vector<bool>;
    using LockGuard = std::lock_guard<std::mutex>;
    struct IteratorOfLists
    {
        std::list<SinglePool>::iterator poolsIterator;
        std::list<SinglePoolAllocationStatus>::iterator vacantIterator;
        std::list<std::mutex>::iterator mutexesIterator;
        IteratorOfLists& operator++();
        IteratorOfLists& operator--();
        bool operator==(const IteratorOfLists& rhs) const
        {
            return poolsIterator == rhs.poolsIterator
                && vacantIterator == rhs.vacantIterator
                && mutexesIterator == rhs.mutexesIterator;
        }
        bool operator!=(const IteratorOfLists& rhs) const
        {
            return !(*this == rhs);
        }
    };
    IteratorOfLists begin();
    IteratorOfLists end();
    IteratorOfLists beforeEnd();
public:
    FixedSizeMemoryPool(std::size_t memoryBlockSize, std::size_t initialBlockCount);
    ~FixedSizeMemoryPool() noexcept(false);
private:
    void returnMemoryBlock(IteratorOfLists iterators, std::size_t blockIndex);
    bool empty() const;
    bool full() const;
    void expandPool();
    void* rawPointerAt(std::list<SinglePool>::iterator poolIterator, std::size_t blockIndex)
    {
        return reinterpret_cast<void*>(poolIterator->data() + memoryBlockSize_ * blockIndex);
    }
public:
    std::shared_ptr<void> lendMemoryBlock();
private:
    std::size_t memoryBlockSize_;
    std::size_t initialBlockCount_;
    std::list<SinglePool> pools_;
    std::list<SinglePoolAllocationStatus> vacant_;
    std::list<std::mutex> mutexes_;
};
}
}

#endif //MUSEC_BASE_FIXEDSIZEMEMORYPOOL
