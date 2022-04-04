#ifndef MUSEC_BASE_FIXEDSIZEMEMORYBLOCK
#define MUSEC_BASE_FIXEDSIZEMEMORYBLOCK

#include <cstddef>

namespace Musec
{
namespace Base
{
class FixedBlockSizeMemoryBlock
{
public:
    FixedBlockSizeMemoryBlock(std::size_t blockSize):
        blockSize_(blockSize),
        block_(new std::byte[blockSize])
    {
    }
    ~FixedBlockSizeMemoryBlock()
    {
        delete[] block_;
    }
public:
    std::byte* data() const
    {
        return block_;
    }
private:
    std::size_t blockSize_;
    std::byte* block_;
};
}
}

#endif //MUSEC_BASE_FIXEDSIZEMEMORYBLOCK
