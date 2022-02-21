#ifndef MUSEC_BASE_POOL
#define MUSEC_BASE_POOL

#include "base/Base.hpp"

#include <functional>
#include <map>
#include <memory>
#include <random>

namespace Musec
{
namespace Base
{
template<typename T>
class Pool: public std::enable_shared_from_this<Pool<T>>
{
    using Self = Pool<T>;
    using PoolType = std::map<std::uint_fast32_t, std::weak_ptr<T>>;
private:
    Pool(): std::enable_shared_from_this<Pool<T>>(), pool_(), keygen_()
    {
    }
public:
    static std::shared_ptr<Self> makePool()
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
    Pool(const Self&) = delete;
    Pool(Self&&) = delete;
private:
    std::weak_ptr<Pool<T>> weakThis()
    {
        return this->weak_from_this();
    }
    std::function<void(T*)> getDeleter(std::uint_fast32_t key)
    {
        return [key, weakPool = weakThis()](T* ptr)
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
    std::uint_fast32_t getKey()
    {
        while(true)
        {
            auto ret = keygen_();
            if(pool_.find(ret) == pool_.end())
            {
                return ret;
            }
        }
    }
public:
    std::shared_ptr<T> insert(T* raw)
    {
        if(raw)
        {
            auto key = getKey();
            auto ret = std::shared_ptr<T>(raw, getDeleter(key));
            pool_.insert(key, std::weak_ptr<T>(ret));
            return ret;
        }
        return std::shared_ptr<T>();
    }
    template<typename... Args>
    std::shared_ptr<T> emplace(Args&&... args)
    {
        auto key = getKey();
        std::shared_ptr<T> ret(new T(std::forward<Args>(args)...), getDeleter(key));
        pool_.emplace(key, std::weak_ptr<T>(ret));
        return ret;
    }
private:
    PoolType pool_;
    std::minstd_rand0 keygen_;
};
}
}


#endif //MUSEC_BASE_POOL
