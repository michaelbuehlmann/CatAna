# distutils: libraries = besseltools gsl blas
# distutils: library_dirs = ${library_dir}
# distutils: include_dirs = ${include_dir}

from cython.operator cimport dereference as deref

cdef extern from "catana/tools/besseltools.hpp":
    cdef cppclass CPPBesselZeros "BesselZeros":
        CPPBesselZeros(double l) except +
        void compute_up_to(double z_max)
        double operator[](unsigned int n)

cdef class BesselZeros:
    cdef CPPBesselZeros *thisptr
    def __cinit__(self, double l):
        self.thisptr = new CPPBesselZeros(l)
        
    def __dealloc__(self):
        del self.thisptr
        
    def compute_up_to(self, double z_max):
        self.thisptr.compute_up_to(z_max)
        
    def __getitem__(self, unsigned int n):
        return deref(self.thisptr)[n] 