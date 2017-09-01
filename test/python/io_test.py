import numpy as np
import catana
import pytest
import os


@pytest.fixture
def datadir():
    return os.path.join(os.path.dirname(__file__), '..', 'test_data')


class TestText(object):
    def test_source(self, datadir):
        s = catana.io.CartesianTextSource(os.path.join(datadir, "mock_data.txt"))
        pc = s.get_point_container()
        pc2 = s.get_point_container()

        assert len(pc) == len(pc2) == 20

    def test_sinks(self, datadir):
        pass


class TestGadget(object):
    pass


class TestFilter(object):
    def test_tophat(self):
        data = np.zeros((5, 3))
        data[:, 0] = np.arange(5, 0, -1)
        pc = catana.PointContainer(data)
        tf = catana.io.TophatRadialWindowFunctionFilter(3.1)
        tf(pc)

        assert np.array(pc).shape == (3, 3)

    def test_gaussian(self):
        N = 1<<24
        data = np.zeros((N, 3))
        data[:, 0] = 1000 * np.random.uniform(0, 1, N)**(1/3)
        pc = catana.PointContainer(data)
        gf = catana.io.GaussianRadialWindowFunctionFilter(100)
        gf(pc)

        assert np.array(pc).shape[0]/N == pytest.approx(0.001329340388, rel=0.1)

    def test_angular(self):
        pc = catana.PointContainer(np.array([[1, 0, -1], [1, 0, 1]]))
        af = catana.io.AngularMaskFilter(np.array([0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]))

        af(pc)
        assert np.array(pc).shape[0] == 1
        assert pc[0].spherical()[2] == -1

    def test_angular_file(self, datadir):
        pc = catana.PointContainer(np.array([[1, 0, -1], [1, 0, 1]]))
        af = catana.io.AngularMaskFilter(os.path.join(datadir, 'testmap.fits'))

        af(pc)
        assert np.array(pc).shape[0] == 1
        assert pc[0].spherical()[2] == -1

    def test_angular_invalid_map(self):
        with pytest.raises(RuntimeError):
            af = catana.io.AngularMaskFilter(np.zeros(11))


class TestFilterStream(object):
    def test_tophat_stream(self):
        N = 1000
        data = np.zeros((N, 3))
        data[:, 0] = np.random.permutation(np.arange(N))
        pc = catana.PointContainer(data)
        source = catana.io.PointContainerSource(pc)

        pc2 = catana.PixelizedPointContainer(16)
        sink = catana.io.PixelizedPointContainerSink(pc2)

        f = catana.io.TophatRadialWindowFunctionFilter(222.4)

        fs = catana.io.FilterStream(source, sink)
        fs.add_filter(f)
        fs.run()

        assert np.sum(pc2.get_countmap()) == 223

    def test_multisource_stream(self):
        N = 1000
        data = np.zeros((N, 3))
        data[:, 0] = np.random.permutation(np.arange(N))
        pc = catana.PointContainer(data)
        source = catana.io.PointContainerSource(pc)

        pc2 = catana.PixelizedPointContainer(16)
        sink = catana.io.PixelizedPointContainerSink(pc2)

        f = catana.io.TophatRadialWindowFunctionFilter(222.4)

        fs = catana.io.FilterStream(source, sink)
        fs.add_filter(f)

        fs.run_totemp()
        source.reset()
        fs.run_totemp()

        fs.run_fromtemp()
        assert np.sum(pc2.get_countmap()) == 446

    def test_filterscope(self):
        # This test was added because if the filter was not saved in a variable, this caused SIGSEGV
        N = 1000
        data = np.zeros((N, 3))
        data[:, 0] = np.random.permutation(np.arange(N))
        pc = catana.PointContainer(data)
        source = catana.io.PointContainerSource(pc)

        pc2 = catana.PixelizedPointContainer(16)
        sink = catana.io.PixelizedPointContainerSink(pc2)

        fs = catana.io.FilterStream(source, sink)

        fs.add_filter(catana.io.TophatRadialWindowFunctionFilter(222.4))
        fs.run()

        assert np.sum(pc2.get_countmap()) == 223