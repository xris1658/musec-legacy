#ifndef MUSEC_DKWTP_GRAPH
#define MUSEC_DKWTP_GRAPH

#include <algorithm>
#include <map>
#include <vector>

namespace Musec
{
namespace Audio
{
namespace Engine
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
    typename NodeMap::iterator findNode(T* const data)
    {
        return nodes_.find(data);
    }
public:
    bool nodeExists(T* const data) const
    {
        return findNode(data) != nodes_.end();
    }
    // Adds a node, and directly links this node to the previous and the next node.
    // The node will be linked even if loops will be introduced.
    // If needed, please check before calling this function.
    void addNode(T* const data, const std::vector<T*>& prev = {}, const std::vector<T*>& next = {})
    {
        if(!nodeExists(data))
        {
            nodes_.emplace(data, Route {prev, next});
            // Link to the previous point
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
            // Link to the next point
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
    // Removes the link of the given point and the previous & next point, then removes the given point.
    // The node will be removed even if the previous or next point is isolated.
    // If needed, please check before calling this function.
    void removeNode(T* const data)
    {
        // Remove the link to the previous point
        // Remove the link to the next point
        nodes_.erase(data);
    }
    // Link the two nodes even if loops will be introduced.
    // If needed, please check before calling this function.
    void addNodeConnection(T* const from, T* const to)
    {
        auto& next = findNode(from)->second.next_;
        next.emplace_back(to);
        auto& prev = findNode(to)->second.prev_;
        prev.emplace_back(from);
    }
    // Remove a link of two nodes even if any nodes will be isolated.
    // If needed, please check before calling this function.
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

// Check if a (direct or indirect) path from one point to another point is included.
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

// Check if a link between `prev` and `next` will introduce loops to the graph.
template<typename T>
bool introduceLoop(const Graph<T>& graph, const std::vector<T*>& prev, const std::vector<T*>& next)
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

// Check if the graph contains loops.
template<typename T>
bool containsLoop(const Graph<T>& graph)
{
    return std::any_of(
        graph.begin(), graph.end(), [&graph](const auto& nodePrev)
        {
            const auto& prevData = nodePrev.second.prev_;
            return std::any_of(
                graph.begin(), graph.end(), [&graph, &prevData, &nodePrev](const auto& nodeNext)
                {
                    auto& prevPtr = nodePrev.first;
                    auto& nextPtr = nodeNext.first;
                    const auto& nextData = nodeNext.second.next_;
                    return (prevPtr != nextPtr)
                        && (containsPath(graph, prevPtr, nextPtr))
                        && (introduceLoop(graph, prevData, nextData));
                }
            );
        }
    );
}
}
}
}

#endif //MUSEC_DKWTP_GRAPH
