/*
设计一个使用OpenMP的并行程序，用于对两个矩阵进行乘法运算。具体要求如下：

    你的程序应该能够接受两个矩阵作为输入，并计算它们的乘积。
    使用OpenMP将矩阵乘法操作并行化，以加快计算速度。
    考虑如何将矩阵数据进行划分和分配给不同的线程，以实现并行计算。
    考虑如何处理并行区域的同步，以避免竞态条件和数据一致性问题。
    考虑如何利用OpenMP的并行循环和矩阵计算指令，以进一步提高并行效率。
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <random>
#include <iostream>
#include <chrono> // 引入chrono头文件
#include <unordered_map>
#include <fstream>
#include <string>

void matrix_multiply(int A[], int B[], int C[], int dim)
{
    //======= Write Your Code Here========
    // 使用OpenMP指令并行化计算过程
#pragma omp parallel for schedule(dynamic, 1) num_threads(6)
    for (int i = 0; i < dim; i++) {
#pragma omp parallel for schedule(dynamic, 1) num_threads(6)
        for (int j = 0; j < dim; j++) {
            // 计算C[i][j]的值
            int sum = 0;
            // 规约操作：一系列操作数用同一个规约操作符重复地进行计算，中间结果都存储到同一变量(规约变量)的过程
#pragma omp parallel for reduction(+:sum)
            for (int k = 0; k < dim; k++) {
                sum += A[i * dim + k] * B[k * dim + j];
            }
            C[i * dim + j] = sum;
        }
    }
}
void matrix_serial(int A[], int B[], int C[], int dim)
{
    //======= Write Your Code Here========
    // 使用OpenMP指令并行化计算过程
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            // 计算C[i][j]的值
            int sum = 0;
            for (int k = 0; k < dim; k++) {
                sum += A[i * dim + k] * B[k * dim + j];
            }
            C[i * dim + j] = sum;
        }
    }
}


int main()
{
    // 创建一个名为engine的随机数引擎，并指定种子为当前系统时间
    std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());

    // 创建一个名为distribution的分布器，用于限制随机数范围
    std::uniform_int_distribution<int> distribution(1, 100); // 生成1到100之间的整数

    std::ofstream foutA("../ParallelTest/a.txt", std::ios::out);
    if (!foutA.is_open())
        exit(1);

    std::ofstream foutB("../ParallelTest/b.txt", std::ios::out);
    if (!foutB.is_open())
        exit(1);

    int dim;

    std::unordered_map<int, long long> map1;
    std::unordered_map<int, long long> map2;
    std::unordered_map<int, double> speedup;

    //std::cin >> dim;
    for (dim = 2; dim <= 1000; dim++)
    {
        int* A = (int*)malloc(dim * dim * sizeof(int));
        int* B = (int*)malloc(dim * dim * sizeof(int));
        int* C = (int*)malloc(dim * dim * sizeof(int));

        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                int val = distribution(engine);
                A[i * dim + j] = val;
            }
        }

        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                int val = distribution(engine);
                B[i * dim + j] = val;
            }
        }

        // 获取当前时间
        auto t1 = std::chrono::high_resolution_clock::now();
        // 进行矩阵乘法
        matrix_multiply(A, B, C, dim);
        // 获取当前时间
        auto t2 = std::chrono::high_resolution_clock::now();
        // 计算函数运行时间，单位为毫秒
        auto durationM = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);

        std::cout << "dim = " << dim << std::endl;

        // 输出运行时间
        std::cout << "M Function execution time: " << durationM.count() << " ms" << std::endl;

        map1[dim] = durationM.count();

        foutA.write(std::to_string(dim).c_str(), std::to_string(dim).length());
        foutA.write(":", 1);
        foutA.write(std::to_string(map1[dim]).c_str(), std::to_string(map1[dim]).length());
        foutA.write("\n", 1);

        // 获取当前时间
        auto t3 = std::chrono::high_resolution_clock::now();
        // 进行矩阵乘法
        matrix_serial(A, B, C, dim);
        // 获取当前时间
        auto t4 = std::chrono::high_resolution_clock::now();
        // 计算函数运行时间，单位为毫秒
        auto durationS = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3);

        // 输出运行时间
        std::cout << "S Function execution time: " << durationS.count() << " ns" << std::endl;

        map2[dim] = durationS.count();

        foutB.write(std::to_string(dim).c_str(), std::to_string(dim).length());
        foutB.write(":", 1);
        foutB.write(std::to_string(map2[dim]).c_str(), std::to_string(map2[dim]).length());
        foutB.write("\n", 1);

        //speedup[dim] = map2[dim] / map1[dim];

        // 打印结果矩阵
        //for (int i = 0; i < dim; i++)
        //{
        //    for (int j = 0; j < dim; j ++)
        //    {
        //        printf("%d ", C[i * dim + j]);
        //    }
        //    printf("\n");
        //}

        free(A);
        free(B);
        free(C);
    }



    return 0;
}