#pragma once

#include <vector>
#include <cmath>

/* 
* @brief : 向量线性插值
* @param : begPnt - 起点向量
* @param : endPnt - 终点向量
* @param : maxstep - 最大步长
* @return: 插值点上的关节角
*/
std::vector<std::vector<double>> get_linear_interpolate(const std::vector<double>& begPnt, const std::vector<double>& endPnt, double maxStep);

