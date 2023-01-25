#ifndef MUSEC_UTIL_FILEHASH
#define MUSEC_UTIL_FILEHASH

#include <QString>

#include <vector>

namespace Musec
{
namespace Util
{
std::vector<char> fileHash(const QString& filePath);
}
}

#endif //MUSEC_UTIL_FILEHASH
