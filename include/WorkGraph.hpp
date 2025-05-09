#pragma once

#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <unordered_set>
#include <memory>
#include <any>

class Node{
    std::unique_ptr<std::once_flag> flag;
    std::function<void(Node&)> func;

    std::unordered_set<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<std::any>> in, out;

    void reset_flag(){
        flag = std::make_unique<std::once_flag>();
        for(auto& i : nodes){
            i->reset_flag();
        }
    }
    void execute(){
        std::vector<std::future<void>> futures;
        for(auto& i : nodes)
            futures.push_back(std::async(std::launch::async, [&i]{ i->execute(); }));
        
        for(auto& i : futures)
            i.wait();

        std::call_once(*flag, func, *this);
    }
public:
    Node(const std::function<void(Node&)>& func, 
            const size_t in_size, 
            const size_t out_size) : 
                func(func), in(in_size), out(out_size), flag(std::make_unique<std::once_flag>()) {}

    static std::shared_ptr<Node> make_node(const std::function<void(Node&)>& func, const size_t in_size, const size_t out_size){
        return std::make_shared<Node>(func, in_size, out_size);
    }
    friend std::shared_ptr<Node>& operator > (std::shared_ptr<Node>& prev, std::shared_ptr<Node> crnt);

    static void map(std::shared_ptr<Node> nodeA, std::shared_ptr<Node> nodeB, size_t _out, size_t _in){
        if(nodeA->out.at(_out)){
            nodeB->in.at(_in) = nodeA->out.at(_out);
            return;
        }
        nodeA->out.at(_out) = std::make_shared<std::any>();
        map(nodeA, nodeB, _out, _in);
    }

    template<typename T>
    void set_input(const size_t i, const T& val){
        if(in.at(i)){
            *in.at(i) = val;
            return;
        }
        in.at(i) = std::make_shared<std::any>(val); 
    }

    template<typename T>
    T& get_output(const size_t i){
        return std::any_cast<T&>(*out.at(i));
    }

    template<typename T>
    void set_output(const size_t i, const T& val){
        if(out.at(i)){
            *out.at(i) = val;
            return;
        }
        out.at(i) = std::make_shared<std::any>(val);
    }

    template<typename T>
    T& get_input(const size_t i){
        return std::any_cast<T&>(*in.at(i));
    }

    static void execute(std::shared_ptr<Node> node){
        node->execute();
        node->reset_flag();
    }
};

inline std::shared_ptr<Node>& operator > (std::shared_ptr<Node>& prev, std::shared_ptr<Node> crnt){
    crnt->nodes.insert(prev);
    return prev;
}
