# WorkGraph

This is an example of Work Graph / Graph of Execution. You can use this project if you wanna make program which will work as pipeline in OpenGL or Vulkan. And this project apply feature like fence if you separate one node to difference nodes, difference nodes will execute parallel and node which need with nodes will wait that. And then will use results of nodes to execute itself. 

# Sample of code

```cpp
#include "WorkGraph.hpp"

#include <iostream>

int main(){
    auto node1 = Node::make_node([](Node& node){
        std::cout << "Big Mac prepared successfully!" << std::endl;
    }, 0, 0);

    // etc

    auto node2 = Node::make_node([](Node& node){
        std::cout << "Preparing salad" << std::endl;
    }, 0, 0);

    auto node3 = Node::make_node([](Node& node){
        std::cout << "Preparing meat" << std::endl;
    }, 0, 0);

    auto node4 = Node::make_node([](Node& node){
        std::cout << "Preparing Big Mac" << std::endl;
    },0,0);

    node4 > (node2 > node1);
    node4 > (node3 > node1);

    Node::execute(node1);
    // execution from begining (node4) to the end (node1)
}
```

Output:

```
Preparing Big Mac
Preparing meat
Preparing salad
Big Mac prepared successfully!
```

after another execution output can be:
```
Preparing Big Mac
Preparing salad
Preparing meat
Big Mac prepared successfully!
```

# How to use?

Bro, just git clone this project and copy **WorkGraph.hpp** to your include directory or use wget (or other util) to download this header.

:3

# UPD

original workgraph [WorkGraph](https://github.com/CyberFlex1984/WorkGraph)

fake / copy-pasted workgraph [D3D12 Work Graphs](https://devblogs.microsoft.com/directx/d3d12-work-graphs/)
