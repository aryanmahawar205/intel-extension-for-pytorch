import unittest
import torch
import model_convert


class TorchAPITests(unittest.TestCase):
    def test_is_available(self):
        torch.cuda.is_available()

    def test_init(self):
        torch.cuda.init()
        self.assertEqual(True, torch.cuda.is_initialized())

    def test_device_count(self):
        x = torch.cuda.device_count()
        y = torch.xpu.device_count()
        self.assertEqual(x, y)
        
    def test_get_device_name(self):
        device_id = torch.xpu.current_device()
        x = torch.xpu.get_device_name(device_id)
        device_id = torch.cuda.current_device()
        y = torch.cuda.get_device_name(device_id)
        self.assertEqual(x, y)

    def test_current_device(self):
        x = torch.xpu.current_device()
        y = torch.cuda.current_device()
        self.assertEqual(x, y)

    def test_set_device(self):
        device_id = torch.cuda.current_device()
        torch.cuda.set_device(device_id)

    def test_float_tensor(self):
        x = torch.cuda.FloatTensor(224, 224, 32, device=torch.device("cuda"))
        self.assertEqual(x.device.type, "xpu")

    def test_int_tensor(self):
        x = torch.cuda.IntTensor([3, 4])
        self.assertEqual(x.device.type, "xpu")

    def test_empty_cache(self):
        torch.cuda.empty_cache()

    def test_memory_snapshot(self):
        torch.cuda.memory_snapshot()


if __name__ == "__main__":
    unittest.main()