#include <ATen/ATen.h>

#include <utils/MathReduce.h>
#include <utils/Numerics.h>


namespace at {
namespace AtenIpexTypeDPCPP {

std::tuple<Tensor &, Tensor &>
_min_out(Tensor & min, Tensor & min_indices, const Tensor & self, int64_t dim, bool keepdim) {
  AT_DISPATCH_ALL_TYPES(min.scalar_type(), "_min_out", [&] {
    std::pair<scalar_t, int64_t> init =
        std::make_pair<scalar_t, int64_t>(Numerics<scalar_t>::upper_bound(), 0);

    dim = maybe_wrap_dim(dim, TensorImpl_Unwrap(self));

    reduceDimIndex<scalar_t, int64_t>(min, min_indices, self,
        dim, keepdim, init, MinValuePair<scalar_t, int64_t>());
    }
  );

  return std::tuple<Tensor &, Tensor &>(min, min_indices);
}

std::tuple<Tensor, Tensor>
_min(const Tensor & self, int64_t dim, bool keepdim) {
  auto min = empty({0}, self.options());
  auto min_indices = empty({0}, self.options().dtype(kLong));

  return AtenIpexTypeDPCPP::_min_out(min, min_indices, self, dim, keepdim);
}

std::tuple<Tensor, Tensor>
min(const Tensor & self, int64_t dim, bool keepdim) {
  return AtenIpexTypeDPCPP::_min(self, dim, keepdim);
}

std::tuple<Tensor &, Tensor &>
min_out(Tensor & min, Tensor & min_values, const Tensor & self, int64_t dim, bool keepdim) {
  return AtenIpexTypeDPCPP::_min_out(min, min_values, self, dim, keepdim);
}

std::tuple<Tensor &, Tensor &>
_max_out(Tensor & max, Tensor & max_indices, const Tensor & self, int64_t dim, bool keepdim) {
  AT_DISPATCH_ALL_TYPES(max.scalar_type(), "_max_out", [&] {
    std::pair<scalar_t, int64_t> init =
        std::make_pair<scalar_t, int64_t>(Numerics<scalar_t>::lower_bound(), 0);

    dim = maybe_wrap_dim(dim, TensorImpl_Unwrap(self));

    reduceDimIndex<scalar_t, int64_t>(max, max_indices, self,
        dim, keepdim, init, MaxValuePair<scalar_t, int64_t>());
    }
  );

  return std::tuple<Tensor &, Tensor &>(max, max_indices);
}

std::tuple<Tensor, Tensor>
_max(const Tensor & self, int64_t dim, bool keepdim) {
  auto max = empty({0}, self.options());
  auto max_indices = empty({0}, self.options().dtype(kLong));

  return AtenIpexTypeDPCPP::_max_out(max, max_indices, self, dim, keepdim);
}

std::tuple<Tensor, Tensor>
max(const Tensor & self, int64_t dim, bool keepdim) {
  return AtenIpexTypeDPCPP::_max(self, dim, keepdim);
}

std::tuple<Tensor &, Tensor &>
max_out(Tensor & max, Tensor & max_values, const Tensor & self, int64_t dim, bool keepdim) {
  return AtenIpexTypeDPCPP::_max_out(max, max_values, self, dim, keepdim);
}

} // namespace AtenIpexTypeDPCPP
} // namespace at
