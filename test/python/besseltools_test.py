import numpy as np
import catana
import pytest


class TestSphericalBesselZeros(object):
    def test_default_constructor(self):
        with pytest.raises(TypeError):
            sfb = catana.besseltools.SphericalBesselZeros()

    def test_zeros(self):
        sfb = catana.besseltools.SphericalBesselZeros(0)

        assert sfb[0] == pytest.approx(np.pi)
        assert sfb[1] == pytest.approx(6.283185307179586)
        assert sfb[2] == pytest.approx(9.42477796076938)

        sfb = catana.besseltools.SphericalBesselZeros(5)
        assert sfb[9] == pytest.approx(38.8836)

    def test_computeupto(self):
        sfb = catana.besseltools.SphericalBesselZeros(2)
        sfb.compute_up_to(100)


class TestBesselIntegrator(object):
    def test_known_values(self):
        w = lambda r: 1
        assert catana.besseltools.double_sbessel_integrator(w, 0, 100, 1, 1) == pytest.approx(1.56582, abs=1e-5)
        assert catana.besseltools.double_sbessel_integrator(w, 0, 100, 1, 2) == pytest.approx(0.785393, abs=1e-5)
        assert catana.besseltools.double_sbessel_integrator(w, 4, 100, 1, 4.2) == pytest.approx(0.000133933, abs=1e-9)

    def test_k_symmetric(self):
        w = lambda r: np.exp(-(r/40)**2) * r**2
        res1 = catana.besseltools.double_sbessel_integrator(w, 6, 100, 2.3, 4.5)
        res2 = catana.besseltools.double_sbessel_integrator(w, 6, 100, 4.5, 2.3)
        assert res1 == pytest.approx(res2)

    def test_function_interpolator(self):
        w = lambda r: np.exp(-(r/40)**2) * r**2
        fi = catana.FunctionInterpolator(w, 100000, 0, 100)

        assert fi(0) == w(0)
        assert fi(99.9) == pytest.approx(w(99.9))
        assert fi(20) == pytest.approx(fi(20))

        with pytest.raises(ValueError):
            fi(100)

        with pytest.raises(ValueError):
            fi(-1)

        with pytest.raises(ValueError):
            fi(120)

        res1 = catana.besseltools.double_sbessel_integrator(w, 6, 100, 2.3, 4.5)
        res2 = catana.besseltools.double_sbessel_integrator(fi, 6, 100, 2.3, 4.5)
        assert res1 == pytest.approx(res2, rel=1e-5)
