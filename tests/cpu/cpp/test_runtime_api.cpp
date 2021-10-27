#include <torch/torch.h>
#include "gtest/gtest.h"
#include "torch_ipex/csrc/cpu/runtime/CPUPool.h"
#include "torch_ipex/csrc/cpu/runtime/Task.h"
#include "torch_ipex/csrc/cpu/runtime/TaskExecutor.h"

#define ASSERT_VARIABLE_EQ(a, b) ASSERT_TRUE(torch::allclose((a), (b)))
#define EXPECT_VARIABLE_EQ(a, b) EXPECT_TRUE(torch::allclose((a), (b)))

TEST(TestRuntimeAPI, TestMainThreadCoreBind) {
  if (!torch_ipex::runtime::is_runtime_ext_enabled()) {
    GTEST_SKIP()
        << "Skip TestRuntimeAPI::TestMainThreadCoreBind. Didn't preload IOMP.";
  }
  at::Tensor input_tensor = at::rand({100, 8276});
  // Get the reference result
  auto res_ref = at::softmax(input_tensor, -1);
  // Ping CPU Cores
  std::vector<int32_t> cpu_core_list({0});
  torch_ipex::runtime::_pin_cpu_cores(cpu_core_list);
  auto res = at::softmax(input_tensor, -1);
  ASSERT_VARIABLE_EQ(res, res_ref);
}

TEST(TestRuntimeTaskAPI, TestNativeTorchOperation) {
  if (!torch_ipex::runtime::is_runtime_ext_enabled()) {
    GTEST_SKIP()
        << "Skip TestRuntimeTaskAPI::TestNativeTorchOperation. Didn't preload IOMP.";
  }
  std::vector<int32_t> cpu_core_list({0});
  std::shared_ptr<torch_ipex::runtime::TaskExecutor> task_executor =
      std::make_shared<torch_ipex::runtime::TaskExecutor>(cpu_core_list);
  at::Tensor input_tensor = at::rand({100, 8276});
  // Get the reference result
  auto res_ref = at::softmax(input_tensor, -1);
  // Create the task
  torch_ipex::runtime::Task<
      at::Tensor (*)(const at::Tensor&, int64_t, c10::optional<at::ScalarType>),
      at::Tensor,
      int64_t,
      c10::optional<at::ScalarType>>
      task(at::softmax, task_executor);
  auto res_future = task(std::move(input_tensor), -1, c10::nullopt);
  auto res = res_future.get();
  // Assert the result
  ASSERT_VARIABLE_EQ(res, res_ref);
}

TEST(TestRuntimeTaskAPI, TestLambdaFunction) {
  if (!torch_ipex::runtime::is_runtime_ext_enabled()) {
    GTEST_SKIP()
        << "Skip TestRuntimeTaskAPI::TestLambdaFunction. Didn't preload IOMP.";
  }
  std::vector<int32_t> cpu_core_list({0});
  std::shared_ptr<torch_ipex::runtime::TaskExecutor> task_executor =
      std::make_shared<torch_ipex::runtime::TaskExecutor>(cpu_core_list);
  at::Tensor input_tensor = at::rand({100, 8276});
  // Get the reference result
  auto res_ref = at::softmax(input_tensor, -1);
  // Create the task
  torch_ipex::runtime::Task<at::Tensor (*)(const at::Tensor&), at::Tensor> task(
      [](const at::Tensor& input) -> at::Tensor {
        return at::softmax(input, -1);
      },
      task_executor);
  auto res_future = task(std::move(input_tensor));
  auto res = res_future.get();
  // Assert the result
  ASSERT_VARIABLE_EQ(res, res_ref);
}

at::Tensor taskfunction(const at::Tensor& input) {
  at::Tensor output;
  output = at::softmax(input, -1);
  return output;
}

TEST(TestRuntimeTaskAPI, TestCPPFunction) {
  if (!torch_ipex::runtime::is_runtime_ext_enabled()) {
    GTEST_SKIP()
        << "Skip TestRuntimeTaskAPI::TestCPPFunction. Didn't preload IOMP.";
  }
  std::vector<int32_t> cpu_core_list({0});
  std::shared_ptr<torch_ipex::runtime::TaskExecutor> task_executor =
      std::make_shared<torch_ipex::runtime::TaskExecutor>(cpu_core_list);
  at::Tensor input_tensor = at::rand({100, 8276});
  // Get the reference result
  auto res_ref = taskfunction(input_tensor);
  // Create the task
  torch_ipex::runtime::Task<at::Tensor (*)(const at::Tensor&), at::Tensor> task(
      taskfunction, task_executor);
  auto res_future = task(std::move(input_tensor));
  auto res = res_future.get();
  // Assert the result
  ASSERT_VARIABLE_EQ(res, res_ref);
}
