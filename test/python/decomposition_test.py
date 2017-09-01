import numpy as np
import catana
import pytest
import os


@pytest.fixture
def datadir():
    return os.path.join(os.path.dirname(__file__), '..', 'test_data')


@pytest.fixture
def gaussian_source(datadir):
    return {
        'correction_factor': 5535184878.03/(2500**3 * 4/3 * np.pi),
        'source': catana.io.CartesianTextSource(os.path.join(datadir, "gaussian_catalog.txt")),
        'c_ln': np.loadtxt(os.path.join(datadir, "gaussian_catalog.python.c_ln")),
        'c_ln_rev': np.loadtxt(os.path.join(datadir, "gaussian_catalog.pythonrev.c_ln")),
        'k_ln': np.loadtxt(os.path.join(datadir, "gaussian_catalog.python.k_ln"))
    }


class TestSFB(object):
    def test_raw(self, gaussian_source):
        pc = gaussian_source['source'].get_point_container()
        assert(np.array(pc).shape == (445, 3))

        kclkk = catana.sfb_decomposition(pc, 10, 10, 2500, False, False)

        # correct volume (gaussian filter was applied on data)
        k_ln = kclkk.k_ln
        c_ln = kclkk.c_ln * gaussian_source['correction_factor']**2

        assert np.allclose(k_ln, gaussian_source['k_ln'])
        assert np.allclose(c_ln, gaussian_source['c_ln'], rtol=1e-2)

    def test_rev(self, gaussian_source):
        pc = gaussian_source['source'].get_point_container()
        assert(np.array(pc).shape == (445, 3))
        ppc = catana.PixelizedPointContainer(64, pc)

        kclkk = catana.sfb_decomposition(ppc, 10, 10, 2500, False, False)

        # correct volume (gaussian filter was applied on data)
        k_ln = kclkk.k_ln
        c_ln = kclkk.c_ln * gaussian_source['correction_factor']**2

        assert np.allclose(k_ln, gaussian_source['k_ln'])
        assert np.allclose(c_ln, gaussian_source['c_ln_rev'], rtol=1e-2)


class TestAnalyzer(object):
    def test_raw(self, gaussian_source):
        a = catana.Analyzer(gaussian_source['source'])
        kclkk = a.compute_sfb(10, 10, 2500, False, False)

        # correct volume (gaussian filter was applied on data)
        k_ln = kclkk.k_ln
        c_ln = kclkk.c_ln * gaussian_source['correction_factor']**2

        assert np.allclose(k_ln, gaussian_source['k_ln'])
        assert np.allclose(c_ln, gaussian_source['c_ln'], rtol=1e-2)

    def test_rev(self, gaussian_source):
        a = catana.Analyzer(gaussian_source['source'])
        kclkk = a.compute_sfb_pixelized(10, 10, 2500, 64, False, False)

        # correct volume (gaussian filter was applied on data)
        k_ln = kclkk.k_ln
        c_ln = kclkk.c_ln * gaussian_source['correction_factor']**2

        assert np.allclose(k_ln, gaussian_source['k_ln'])
        assert np.allclose(c_ln, gaussian_source['c_ln_rev'], rtol=1e-2)
