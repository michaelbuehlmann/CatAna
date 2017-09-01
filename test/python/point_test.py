import numpy as np
import catana
import pytest

class TestPoint(object):
    def test_default_constructor(self):
        p = catana.Point()
        assert p.cartesian() == (0.0, 0.0, 0.0)
        assert p.spherical() == (0.0, 0.0, 0.0)
        assert np.allclose(p, np.zeros(3))

    def test_value_constructor(self):
        p = catana.Point(0, 0, 1)
        assert p.spherical() == (1, 0, 0)
        assert p.cartesian() == (0, 0, 1)


class TestPointContainer(object):
    def test_default_constructor(self):
        pc = catana.PointContainer()
        assert np.array(pc).shape == (0, 3)

    def test_add_point(self):
        pc = catana.PointContainer()
        pc.add_point(catana.Point())
        assert np.array(pc).shape == (1, 3)
        assert np.allclose(pc, np.zeros((1, 3)))

    def test_numpy_constructor(self):
        data = np.arange(9).reshape(3, 3)
        pc = catana.PointContainer(data)
        assert(np.allclose(pc, data))

    def test_getitem(self):
        pc = catana.PointContainer(np.ones((2, 3)))
        point0 = pc[0]
        point1 = pc[1]
        assert np.allclose(point0, point1)
        assert np.allclose(point0, [1, 1, 1])

        with pytest.raises(IndexError):
            point2 = pc[2]


class TestPixelizedPointContainer(object):
    def test_default_constructor(self):
        with pytest.raises(TypeError):
            ppc = catana.PixelizedPointContainer()

    def test_empty_constructor(self):
        ppc = catana.PixelizedPointContainer(4)
        cm = ppc.get_countmap()
        assert np.all(cm == np.zeros(4**2 * 12))

    def test_pc_constructor(self):
        data = np.array([[1, 0, 0], [1, 0, 0], [1, np.pi, 0]])
        pc = catana.PointContainer(data)
        ppc = catana.PixelizedPointContainer(32, pc)
        cm = ppc.get_countmap()
        assert cm.shape == (32**2 * 12, )
        assert np.sum(cm) == 3
        assert np.count_nonzero(cm) == 2
