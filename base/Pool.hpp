#ifndef MUSEC_BASE_POOL
#define MUSEC_BASE_POOL

#include "base/Base.hpp"

#include <functional>
#include <memory>
#include <map>

namespace Musec
{
namespace Base
{
template<typename KeyType, typename ContentType>
class Pool: public std::enable_shared_from_this<Pool<KeyType, ContentType>>
{
    using Self = Pool<KeyType, ContentType>;
    using RefCountSelf = std::shared_ptr<Self>;
    using RefCountContent = std::shared_ptr<ContentType>;
    using WeakRefCountSelf = std::weak_ptr<Self>;
    using WeakRefCountContent = std::weak_ptr<ContentType>;
    using PoolType = std::map<KeyType, WeakRefCountContent>;
private:
    Pool(): pool_() {}
public:
    static RefCountSelf makePool()
    {
        struct MakeSharedEnabler: public Self {};
        return std::make_shared<MakeSharedEnabler>();
    }
public:
    ~Pool()
    {
        for(auto& weak: pool_)
        {
            if(!weak.second.expired())
            {
                weak.second.lock().reset();
            }
        }
    }
private:
    WeakRefCountSelf weakThis()
    {
        return this->weak_from_this();
    }
    std::function<void(ContentType*)> generateDeleter(KeyType key)
    {
        return [key, weakPool = weakThis()](ContentType* ptr)
        {
            if(!weakPool.expired())
            {
                auto sharedPool = weakPool.lock();
                auto& pool = sharedPool->pool_;
                pool.erase(key);
            }
            delete ptr;
        };
    }
public:
    RefCountContent insert(KeyType key, ContentType* raw)
    {
        if(raw)
        {
            auto ret = RefCountContent(raw, generateDeleter(key));
            pool_.insert(std::make_pair(key, WeakRefCountContent(ret)));
            return ret;
        }
        return RefCountContent();
    }
    template<typename... Args>
    RefCountContent emplace(KeyType key, Args&&... args)
    {
        RefCountContent ret(new ContentType(std::forward<Args>(args)...), generateDeleter(key));
        pool_.emplace(key, WeakRefCountContent(ret));
        return ret;
    }
private:
    PoolType pool_;
};
}
}

#endif //MUSEC_BASE_POOL
