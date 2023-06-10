/*
���һ��ʹ��OpenMP�Ĳ��г������ڶ�����������г˷����㡣����Ҫ�����£�

    ��ĳ���Ӧ���ܹ���������������Ϊ���룬���������ǵĳ˻���
    ʹ��OpenMP������˷��������л����Լӿ�����ٶȡ�
    ������ν��������ݽ��л��ֺͷ������ͬ���̣߳���ʵ�ֲ��м��㡣
    ������δ����������ͬ�����Ա��⾺̬����������һ�������⡣
    �����������OpenMP�Ĳ���ѭ���;������ָ��Խ�һ����߲���Ч�ʡ�
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <random>
#include <iostream>
#include <chrono> // ����chronoͷ�ļ�
#include <unordered_map>
#include <fstream>
#include <string>

void matrix_multiply(int A[], int B[], int C[], int dim)
{
    //======= Write Your Code Here========
    // ʹ��OpenMPָ��л��������
#pragma omp parallel for schedule(dynamic, 1) num_threads(6)
    for (int i = 0; i < dim; i++) {
#pragma omp parallel for schedule(dynamic, 1) num_threads(6)
        for (int j = 0; j < dim; j++) {
            // ����C[i][j]��ֵ
            int sum = 0;
            // ��Լ������һϵ�в�������ͬһ����Լ�������ظ��ؽ��м��㣬�м������洢��ͬһ����(��Լ����)�Ĺ���
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
    // ʹ��OpenMPָ��л��������
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            // ����C[i][j]��ֵ
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
    // ����һ����Ϊengine����������棬��ָ������Ϊ��ǰϵͳʱ��
    std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());

    // ����һ����Ϊdistribution�ķֲ��������������������Χ
    std::uniform_int_distribution<int> distribution(1, 100); // ����1��100֮�������

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

        // ��ȡ��ǰʱ��
        auto t1 = std::chrono::high_resolution_clock::now();
        // ���о���˷�
        matrix_multiply(A, B, C, dim);
        // ��ȡ��ǰʱ��
        auto t2 = std::chrono::high_resolution_clock::now();
        // ���㺯������ʱ�䣬��λΪ����
        auto durationM = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);

        std::cout << "dim = " << dim << std::endl;

        // �������ʱ��
        std::cout << "M Function execution time: " << durationM.count() << " ms" << std::endl;

        map1[dim] = durationM.count();

        foutA.write(std::to_string(dim).c_str(), std::to_string(dim).length());
        foutA.write(":", 1);
        foutA.write(std::to_string(map1[dim]).c_str(), std::to_string(map1[dim]).length());
        foutA.write("\n", 1);

        // ��ȡ��ǰʱ��
        auto t3 = std::chrono::high_resolution_clock::now();
        // ���о���˷�
        matrix_serial(A, B, C, dim);
        // ��ȡ��ǰʱ��
        auto t4 = std::chrono::high_resolution_clock::now();
        // ���㺯������ʱ�䣬��λΪ����
        auto durationS = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3);

        // �������ʱ��
        std::cout << "S Function execution time: " << durationS.count() << " ns" << std::endl;

        map2[dim] = durationS.count();

        foutB.write(std::to_string(dim).c_str(), std::to_string(dim).length());
        foutB.write(":", 1);
        foutB.write(std::to_string(map2[dim]).c_str(), std::to_string(map2[dim]).length());
        foutB.write("\n", 1);

        //speedup[dim] = map2[dim] / map1[dim];

        // ��ӡ�������
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