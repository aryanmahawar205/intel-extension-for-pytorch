#pragma once

#include <stddef.h>

#include <sycl/sycl.hpp>

namespace gpu::xetla {

void fmha_forward_op(
    sycl::queue& q,
    void* query,
    void* key,
    void* value,
    void* out,
    uint32_t num_batches,
    uint32_t num_heads,
    uint32_t head_size,
    uint32_t num_queries,
    uint32_t num_keys);

void fmha_forward_op_causal(
    sycl::queue& q,
    void* query,
    void* key,
    void* value,
    void* out,
    uint32_t num_batches,
    uint32_t num_heads,
    uint32_t head_size,
    uint32_t num_queries,
    uint32_t num_keys);

void fmha_forward_op_strided(
    sycl::queue& q,
    void* query,
    void* key,
    void* value,
    void* out,
    uint32_t num_batches,
    uint32_t num_heads,
    uint32_t head_size,
    uint32_t num_queries,
    uint32_t num_keys);

// * General interface kernel for FSDP
// * causal
// * permutation t, n, h
// * alibi
void fmha_forward_kernel(
    sycl::queue& q,
    void* query,
    void* key,
    void* value,
    void* alibi,
    void* attn_mask,
    uint8_t* dropout,
    void* out,
    float alpha,
    float beta,
    float dropout_prob,
    uint32_t num_batches,
    uint32_t num_heads,
    uint32_t head_dim,
    uint32_t num_queries,
    uint32_t num_keys,
    uint32_t alibi_padded_block_size,
    uint32_t attn_mask_padded_block_size,
    bool is_causal,
    bool seq_last);

void fmha_forward_index_kernel(
    sycl::queue& q,
    void* query,
    void* key,
    void* value,
    void* key_cache,
    void* value_cache,
    int32_t* index,
    void* alibi,
    void* attn_mask,
    uint8_t* dropout,
    void* out,
    uint32_t timestep,
    float alpha,
    float beta,
    float dropout_p,
    uint32_t num_batches,
    uint32_t beam_width,
    uint32_t num_heads,
    uint32_t head_dim,
    uint32_t num_queries,
    uint32_t num_keys_in,
    uint32_t num_keys_out,
    uint32_t attn_mask_padding,
    bool is_causal);

} // namespace gpu::xetla