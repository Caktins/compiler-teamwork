## Decoupling Algorithms from Schedules for Easy Optimization of Image Processing Pipelines

### Abstract 　　 
&emsp;&emsp;使用现有的编程工具，编写高性能的图像处理代码需要牺牲可读性，可移植性和模块性。作者认为这是将计算定义算法与关于存储和计算顺序的决定混为一谈的结果。在此基础上，作者提出了一种前馈成像流水线的表示方法，将算法从其`schedule`(在空间和时间上将算法映射到资源)中分离出来，在不牺牲代码清晰度的情况下实现了高性能.
&emsp;&emsp;这种解耦方法简化了算法规范：图像和中间缓冲区变成了无限整数域的函数，并且没有明确的存储或边界条件。从而使得程序员可以分别指定组成算法的各种函数的调度策略，这使得他们能够在不改变算法代码的情况下有效地探索优化。即通过将intrinsic algorithm与关于如何在特定机器上有效运行的决策分开来实现更简单的高性能代码。
&emsp;&emsp;总的来说，函数式语言提供了一种将时间和地点分开的自然模型。在这个表示中，算法只定义每个函数在每个点的值，并且`schedule`是指定的：

* The order in which points in the domain of a function are evaluated, including the exploitation of parallelism, and mapping onto SIMD execution units.
* The order in which points in the domain of one function are evaluated relative to points in the domain of another function.
* The memory location into which the evaluation of a function is stored, including registers, scratchpad memories, and regions of main memory.
* Whether a value is recomputed, or from where it is loaded, at each point a function is used.

</br>
### Representing Algorithms and Schedules
&emsp;&emsp;以下将从各个方面来介绍一种将intrinsic algorithm与执行`schedule`分开的图像处理流水线的功能表示。

##### The Intrinsic Algorithm
&emsp;&emsp;将图像表示为在无限整数域上定义的纯函数，其中点上的值表示相应像素的颜色。`imaging pipelines`被规定为`chains of functions`。函数可以是参数中的简单表达式，也可以是有界域上的简化表达式。定义函数的表达式包括：    

* Arithmetic and logical operations;
* Loads from external images;
* If-then-else expressions (semantically equivalent to the ?:ternary operator in C);
* References to named values (which may be function arguments, or expressions defined by a functional let construct);
* Calls to other functions, including external C ABI functions.

这种表示比大多数功能语言更简单，其省略了高阶函数，动态递归和更丰富的数据结构，如元组和列表。 函数只是从整数坐标映射到标量结果。

##### The Schedules
&emsp;&emsp;在成像管线作为功能链条的表述中，有意地省略了何时何地计算功能的选择，而改由程序员用`schedule`来指定。`schedule`描述生产者和消费者内部点的评估顺序，以及存储的内容和重新计算的内容。目前允许四种类型 caller-callee relationships，如下图：
![](https://github.com/Caktins/OnlinePhotos/blob/master/180103_1.png?raw=true)
其中一些导致了额外的选择，包括遍历顺序和域的细分，并且可能对子区域进行递归调度决策。


##### The Fully Specified Program
&emsp;&emsp;用一个特定`schedule`来产生完全指定的命令式程序，其已经定义操作的顺序和数据的位置，以此来降低intrinsic algorithm。由此产生的程序由有序的命令性陈述组成，包括：

* Stores of expression values to array locations;
* Sequential and parallel for loops, which define a range of variable values over which a statement should be executed;
* Producer-consumer edges, which define an array to be allocated (its size given by a potentially dynamic expression), a　block of statements which may write to it, and a block of statements which may read from it, after which it may be freed.

</br>
### The Language
&emsp;&emsp;使用嵌入在C++中的原型语言(`Halide`)构建成像管道。 `Halide`函数链可以被立即编译和使用，也可以编译成一个目标文件和头文件，供其他程序使用，且无需与之进行链接。
具体的使用与形式详见paper的section 4，包含表达式、方程、语法和指定`schedule`等。

</br>
### Compiler Implementation
The Halide compiler lowers imaging pipelines into machine code for ARM, x86, and PTX. It uses the LLVM compiler infrastructure for conventional scalar optimizations, register allocation, and ma chine code generation [LLVM]. While LLVM provides some de gree of platform neutrality, the final stages of lowering must be architecture-specific to produce high-performance machine code. 编译过程如下图所示：
![](https://github.com/Caktins/OnlinePhotos/blob/master/180103_2.png?raw=true)



