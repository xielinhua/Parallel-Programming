# Parallel-Programming
HUST-CS 2020级并行编程原理与实践

## matrx

    (1) 运行main.cpp
    (2) 运行 main.py 生成散点图


## text_search

    (1) 运行 main.py 生成测试文本
    (2) VS 2019 配置使用 PThread 库
        a. 解压 "./Parallel-Programming/text_search/C/pthreads-w32-2-9-1-release"
        b. 把 "./Pre-built.2/include" 添加到 <属性 -> C/C++ -> 常规 -> 附加包目录>
        c. 把 "./Pre-built.2/lib/x86(x64)" 添加到 <属性 -> 链接器 -> 常规 -> 附加库目录>
        d. 把 "pthreadVC2.lib" 添加到 <属性 -> 链接器 -> 输入 -> 附加依赖项>
        e. 把 "./Pre-built.2/dll/x86/" 下的 .dll 文件复制到 "C:/Windows/SysWOW64/" 目录下
        f. 把 "./Pre-built.2/dll/x64/" 下的 .dll 文件复制到 "C:/Windows/System32/" 目录下
    (3) 运行 main.c 测试

## educoder 测试 只需要文件中的部分代码，多余的代码是串行执行相同任务，方便计算加速比
