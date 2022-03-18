#ifndef MUSEC_DKWTP_GRAPH
#define MUSEC_DKWTP_GRAPH

#include <algorithm>
#include <map>
#include <vector>

namespace Musec
{
namespace DKWTP
{
template<typename T>
class Graph
{
private:
    struct Route
    {
        using TPtrVector = std::vector<T*>;
        TPtrVector prev_;
        TPtrVector next_;
    };
    using NodeMap = std::map<T*, Route>;
    using TPtrFunction = void(T* const);
public:
    Graph(): nodes_() {}
    Graph(const Graph&) = default;
    Graph& operator=(const Graph&) = default;
    Graph(Graph&&) noexcept = default;
    Graph& operator=(Graph&&) noexcept = default;
    ~Graph() noexcept = default;
public:
    bool empty() const noexcept
    {
        return nodes_.empty();
    }
    typename NodeMap::size_type size() const noexcept
    {
        return nodes_.size();
    }
    auto begin() const noexcept
    {
        return nodes_.begin();
    }
    auto end() const noexcept
    {
        return nodes_.end();
    }
    auto cbegin() const noexcept
    {
        return nodes_.cbegin();
    }
    auto cend() const noexcept
    {
        return nodes_.cend();
    }
    void clear() noexcept
    {
        nodes_.clear();
    }
private:
    typename NodeMap::const_iterator findNode(T* const data) const
    {
        return nodes_.find(data);
    }
    // CLion 的分析有问题 - 实际上此处的 findNode 是使用了的
    typename NodeMap::iterator findNode(T* const data)
    {
        return nodes_.find(data);
    }
public:
    bool nodeExists(T* const data) const
    {
        return findNode(data) != nodes_.end();
    }
    // 添加一个节点，并添加与上游节点和下游节点的直接连接。
    // 没有附加操作。即使完成这些操作后会引入环，操作仍会正常完成。
    // 请用户自行检查是否引入环，然后选择是否继续操作。
    void addNode(T* const data, const std::vector<T*>& prev = {}, const std::vector<T*>& next = {})
    {
        if(!nodeExists(data))
        {
            nodes_.emplace(data, Route {prev, next});
            // 更新上游节点
            for(T* i: prev)
            {
                auto it = nodes_.find(i);
                if(it != nodes_.end())
                {
                    auto& nexts = it->second.next_;
                    if(nexts.empty() || std::find(nexts.begin(), nexts.end(), data) == nexts.end())
                    {
                        nexts.emplace_back(data);
                    }
                }
            }
            // 更新下游节点
            for(T* i: next)
            {
                auto it = nodes_.find(i);
                if(it != nodes_.end())
                {
                    auto& prevs = it->second.prev_;
                    if(prevs.empty() || std::find(prevs.begin(), prevs.end(), data) == prevs.end())
                    {
                        prevs.emplace_back(data);
                    }
                }
            }
        }
    }
    // 移除一个节点与上游节点和下游节点的连接，并移除此节点。
    // 没有附加操作。即使完成这些操作后有前驱或后继节点被孤立，操作仍会正常完成。
    // 请用户自行检查是否有节点被孤立，然后手动移除节点。
    void removeNode(T* const data)
    {
        // 更新上游节点
        // 更新下游节点
        nodes_.erase(data);
    }
    // 在两个节点之间添加直接连接。
    // 没有附加操作。即使添加直接连接后会引入环，节点之间仍会添加直接连接。
    // 请用户自行检查是否引入环，然后选择是否添加这一直接连接。
    void addNodeConnection(T* const from, T* const to)
    {
        auto& next = findNode(from)->second.next_;
        next.emplace_back(to);
        auto& prev = findNode(to)->second.prev_;
        prev.emplace_back(from);
    }
    // 移除两个节点之间的直接连接。
    // 没有附加操作。即使移除连接后节点被孤立，节点也不会从图中移除。
    // 请用户自行检查节点是否被孤立，然后手动移除节点。
    void removeNodeConnection(T* const from, T* const to)
    {
        auto& next = findNode(from)->second.next_;
        std::remove(next.begin(), next.end(), to);
        auto& prev = findNode(to)->second.prev_;
        std::remove(prev.begin(), prev.end(), from);
    }
    const typename Route::TPtrVector& prevNodes(T* const data) const
    {
        return findNode(data)->second.prev_;
    }
    const typename Route::TPtrVector& nextNodes(T* const data) const
    {
        return findNode(data)->second.next_;
    }
private:
    NodeMap nodes_;
};

// 检查图中中是否包含一个点到另一个点的直接或间接路径
template<typename T>
bool containsPath(const Graph<T>& graph, T* const from, T* const to)
{
    static std::vector<T*> stack;
    stack.emplace_back(from);
    const auto& fromNext = graph.nextNodes(from);
    auto ret = 
    (std::find(fromNext.begin(), fromNext.end(), to) != fromNext.end())
    ||
    (
        std::any_of
        (
            fromNext.begin(), fromNext.end(), [&](const auto& item)
            {
                return std::find(stack.begin(), stack.end(), item) == stack.end()
                    && containsPath(graph, item, to);
            }
        )
    );
    stack.pop_back();
    return ret;
}

// 检查添加的点是否会向图引入环
template<typename T>
bool introduceCycle(const Graph<T>& graph, const std::vector<T*>& prev, const std::vector<T*>& next)
{
    return std::any_of(next.begin(), next.end(),
       [&graph, &prev](const auto& nextData)
       {
           return std::any_of(prev.begin(), prev.end(),
              [&](const auto& prevData)
              {
                  return containsPath(graph, nextData, prevData);
              }
           );
       }
    );
}

// 检查图中是否有环
template<typename T>
bool containsCycle(const Graph<T>& graph)
{
    return std::any_of
    (
        graph.begin(), graph.end(), [&graph](const auto& nodePrev)
        {
            const auto& prevData = nodePrev.second.prev_;
            return std::any_of
            (
                graph.begin(), graph.end(), [&graph, &prevData, &nodePrev](const auto& nodeNext)
                {
                    auto& prevPtr = nodePrev.first;
                    auto& nextPtr = nodeNext.first;
                    const auto& nextData = nodeNext.second.next_;
                    return (prevPtr != nextPtr)
                        && (containsPath(graph, prevPtr, nextPtr))
                        && (introduceCycle(graph, prevData, nextData));
                }
            );
        }
    );
}
}
}

#endif //MUSEC_DKWTP_GRAPH
