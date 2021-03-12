#pragma once

#include <torch/csrc/jit/runtime/custom_operator.h>

namespace torch { namespace jit {
// XXX: PyTorch does not support nesting namespace
// And the alias analysis is not working for namespace other than aten ...
// So we fake some op namespaces to workaround that.
namespace dpcpp {
  static auto reorder_sym = Symbol::fromQualString("dpcpp::reorder");
  static auto batch_norm_sym = Symbol::fromQualString("dpcpp::batch_norm");
  static auto conv2d_relu_sym = Symbol::fromQualString("dpcpp::conv2d_relu");
  static auto conv2d_sum_sym = Symbol::fromQualString("dpcpp::conv2d_sum");
  static auto conv2d_relu_sum_sym = Symbol::fromQualString("dpcpp::conv2d_relu_sum");
  static auto conv2d_sum_relu_sym = Symbol::fromQualString("dpcpp::conv2d_sum_relu");
  static auto conv2d_sigmoid_sym = Symbol::fromQualString("dpcpp::conv2d_sigmoid");
  static auto matmul_sum_sym = Symbol::fromQualString("dpcpp::matmul_sum");
  static auto trans_matmul_sym = Symbol::fromQualString("dpcpp::trans_matmul");
  static auto trans_matmul_scale_sym = Symbol::fromQualString("dpcpp::trans_matmul_scale");
  static auto trans_matmul_scale_sum_sym = Symbol::fromQualString("dpcpp::trans_matmul_scale_sum");
  static auto mul_add_sym = Symbol::fromQualString("dpcpp::mul_add");
  static auto q_conv2d_sum_relu_sym = Symbol::fromQualString("dpcpp::q_conv2d_sum_relu");
  static auto trans_addmm_sym = Symbol::fromQualString("dpcpp::trans_addmm");
  static auto trans_addmm_relu_sym = Symbol::fromQualString("dpcpp::trans_addmm_relu");
  static auto trans_addmm_sigmoid_sym = Symbol::fromQualString("dpcpp::trans_addmm_sigmoid");

  // Fold weights of batch_norm with conv2d's
  static auto fold_weight_sym =
    Symbol::fromQualString("dpcpp::fold_weight");
  static auto fold_bias_sym =
    Symbol::fromQualString("dpcpp::fold_bias");
}

#if 0
Operation createDNNL_reorder(const Node *node);
Operation createDNNL_relu(const Node *node);
Operation createDNNL_relu_(const Node *node);
Operation createDNNL_conv2d(const Node *node);
Operation createDNNL_conv2d_relu(const Node *node);
Operation createDNNL_batch_norm(const Node *node);
Operation createDNNL_fold_weight(const Node *node);
Operation createDNNL_fold_bias(const Node *node);
Operation createDNNL_pooling_max_2d(const Node *node);
Operation createDNNL_pooling_avg_2d(const Node *node);
Operation createDNNL_sum(const Node* node);
Operation createDNNL_sum_(const Node *node);
Operation createDNNL_conv2d_sum(const Node *node);
Operation createDNNL_conv2d_sum_relu(const Node* node);
#endif
}} // namespace torch::jit
