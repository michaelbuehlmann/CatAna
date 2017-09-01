import os
import re
import sys
import subprocess
import platform
import versioneer

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))
        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >=3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]
        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j8']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''), self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)


def get_cmdclass():
    cmdclass = versioneer.get_cmdclass()
    cmdclass.update({"build_ext": CMakeBuild})
    return cmdclass

def readme():
    with open('README.rst') as f:
        return f.read()

setup(
    name='catana',
    description='A library for Spherical Fourier Bessel (SFB) Decomposition',
    long_description=readme(),
    license='GPLv2',
    author='Michael Buehlmann',
    author_email='michael.buehlmann@oca.eu',
    url='https://github.com/michaelbuehlmann/CatAna',
    keywords='SFB spherical Fourier Bessel transformation decomposition cosmology',
    version=versioneer.get_version(),
    ext_package='catana',
    packages=['catana'],
    package_dir = {'': 'python'},
    ext_modules=[CMakeExtension('basictypes'), CMakeExtension('besseltools'), CMakeExtension('decomposition'), CMakeExtension('io')],
    cmdclass=get_cmdclass(),
    zip_safe=False,
    install_requires=['numpy'],
    setup_requires=['pytest-runner'],
    tests_require=['pytest']
)