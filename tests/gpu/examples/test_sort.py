import torch
import intel_extension_for_pytorch
import numpy as np
import random
import pytest
from torch.testing._internal.common_utils import TestCase


class TestNNMethod(TestCase):
    def test_sort(self, dtypes=[torch.bfloat16, torch.half]):
        for dtype in dtypes:
            for i in range(20):
                a = random.randint(1, 3)
                b = random.randint(1, 5)
                c = random.randint(1, 7)
                d = random.randint(1, 6384)
                abcd = [a, b, c, d]
                random.shuffle(abcd)
                x_cpu = torch.randn(abcd).to(dtype)
                x_cpu = torch.testing.make_non_contiguous(x_cpu)
                x_xpu = x_cpu.clone().xpu()
                x_xpu = torch.testing.make_non_contiguous(x_xpu)
                dim = random.randint(0, 3)
                descending = b % 2 == 0
                x_cpu = torch.sort(x_cpu, dim=dim, descending=descending)[0]
                x_xpu = torch.sort(x_xpu, dim=dim, descending=descending)[0]
                maxdiff = float((x_cpu - x_xpu.cpu().float()).abs().max())
                print(abcd, ', dim:', dim, ', maxdiff:', maxdiff)
                assert(maxdiff < 1e-5)

    def test_sort_focus_case(self, dtype=torch.float):
        ''' There is no `stable` option in sort Op
            Sort of IPEX backend supports stable, we construct a stable index
            to check case result. The case could be improved after Py1.10 rebase
        '''

        x = torch.randn(4, 8193, 3)
        idx_ref = torch.empty((8193), dtype=torch.long)
        for i in range(8193):
            x[0][i][0] = 2.22
            idx_ref[i] = i
        y = x.to("xpu")
        res_cpu, _ = torch.sort(x, dim=1)
        res, idx = torch.sort(y, dim=1)
        self.assertEqual(res_cpu, res.cpu())
        self.assertEqual(idx_ref, idx.cpu()[0, :, 0])

        x = torch.randn(4, 2049, 3)
        idx_ref = torch.empty((2049), dtype=torch.long)
        for i in range(2049):
            x[0][i][0] = 2.22
            idx_ref[i] = i
        y = x.to("xpu")
        res_cpu, _ = torch.sort(x, dim=1)
        res, idx = torch.sort(y, dim=1)
        self.assertEqual(res_cpu, res.cpu())
        self.assertEqual(idx_ref, idx.cpu()[0, :, 0])

        x = torch.randn(4, 511, 3)
        idx_ref = torch.empty((511), dtype=torch.long)
        for i in range(511):
            x[0][i][0] = 2.22
            idx_ref[i] = i
        y = x.to("xpu")
        res_cpu, _ = torch.sort(x, dim=1)
        res, idx = torch.sort(y, dim=1)
        self.assertEqual(res_cpu, res.cpu())
        self.assertEqual(idx_ref, idx.cpu()[0, :, 0])