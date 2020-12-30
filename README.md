# VDynamics
VDynamics uses OpenGL to create realtime animations of moving objects.
It is inspired by VPython, but differs in its design: it uses `pybind11` for the C++ binding, it runs as an application rather than in the browser (webGL), and NumPy arrays or Python lists can be used as input to build primitives.
Additionally, a large number of objects can be drawn using instanced drawing, e.g. `SphereCollection`.

## Gallery
![](https://github.com/johnaparker/vdynamics/blob/master/img/spheres.gif)

## Usage
```python
import vdynamics
import numpy as np
import matplotlib as mpl

### create a scene
scene = vdynamics.Scene(background=[.6]*3, window_size=(800,600))

### add a light
light = vdynamics.PointLight([0,0,4])
scene.add_light(light)

### draw objects
rectangle = vdynamics.Rectangle([0,0,-5], 10, 5, [0,0,0,1], material=vdynamics.Material(specular=.2))
scene.draw(rectangle) 

spheres = [vdynamics.Sphere([0,0,-2], .5,  [.2, .9, .8, alpha]), 
           vdynamics.Sphere([0,0,-0], .5,  [.2, .2, .8, alpha]),
           vdynamics.Sphere([0,0,2],  .5,  [.9, .2, .2, alpha])]

for s in spheres:
    scene.draw(s)

def update(theta):
    """update the properties of the objects"""
    spheres[0].position[1] = .5*np.cos(theta)
    spheres[-1].color[:-1] = mpl.colors.hsv_to_rgb(((.13*theta)%1, 1, 1))
    rectangle.color[:-1] = mpl.colors.hsv_to_rgb(((.13*theta)%1, 1, .1))

### execute the animation update loop
scene.run(update, np.linspace(0,4*np.pi, 200))
```

## Installation
Make sure you have the required dependencies:
- GCC compiler
- CMake
- Eigen

Clone this respository 
```shell
git clone https://github.com/johnaparker/vdynamics.git --recurse-submodules
```


Install with
```shell
pip install .
```
