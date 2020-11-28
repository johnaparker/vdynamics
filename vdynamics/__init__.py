from . import _vdynamics
from ._vdynamics import (PointLight, Material, Sphere, SphereCollection, Rectangle)
from .scene import Scene

_vdynamics.set_install_direc(__path__[0])
