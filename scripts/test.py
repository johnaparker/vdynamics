import vdynamics
import numpy as np

scene = vdynamics.Scene([1]*4, (800,600))
pos = np.random.random((5,3))
radii = .01*np.ones(5)
colors = [[1,0,0,1]]*5

light = vdynamics.PointLight([0,0,4])
scene.add_light(light)

scene.draw(vdynamics.Sphere([0,0,0], .1, [0,0,1,1]))
spheres = vdynamics.SphereCollection(pos, radii, colors, resolution=1)
scene.draw(spheres)
scene.run()
