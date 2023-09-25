#include "utils/interpolate.h"

std::vector<std::vector<double>> get_linear_interpolate(const std::vector<double>& begPnt, const std::vector<double>& endPnt, double maxStep) {
  // 关节自由度
  size_t dof = endPnt.size();

  // 获取每个关节的偏移量和最大偏差量
  std::vector<double> jntDiff(dof);
  double maxDiff = 0.0;
  for (size_t i=0; i<dof; ++i) {
    jntDiff[i] = endPnt[i] - begPnt[i];
    maxDiff = std::max(std::fabs(jntDiff[i]), maxDiff);
  }

  // 步数向下取整
  size_t numStep = std::ceil(maxDiff/maxStep);
  // 每个关节的步长
  for (size_t i=0; i<dof; ++i) {
    jntDiff[i] /= numStep;
  }

  // 均匀插值
  std::vector<std::vector<double>> res(numStep+1);
  std::vector<double> curJnt = begPnt;
  for (size_t i=0; i<numStep; ++i) {
    res[i] = curJnt;
    for (size_t j=0; j<dof; ++j) {
      curJnt[j] += jntDiff[j];
    }
  }
  res[numStep] = curJnt;

  return res;
}

