import sphere_vis
import numpy as np
from time import sleep

def circle_vis(position, radius):
    root = sphere_vis.__path__[0]
    vshader = f'{root}/resources/shaders/circle_vis.vs'
    fshader = f'{root}/resources/shaders/circle_vis.fs'

    rmax = np.max(radius)
    xmin = np.min(position[...,0]) - rmax
    xmax = np.max(position[...,0]) + rmax
    ymin = np.min(position[...,1]) - rmax
    ymax = np.max(position[...,1]) + rmax
    dims = np.array([[xmin, xmax], [ymin, ymax]], dtype=float)

    sphere_vis.circle_vis(pos, radii, dims, vshader, fshader)

N = 1000
Nt = 100
xmax = 30

pos_i = np.random.uniform(-xmax, xmax, size=(N,2)).astype(np.float32)
pos = np.empty([Nt, N, 2], dtype=np.float32)
t = np.linspace(0, 2*np.pi, Nt)
for i in range(Nt):
    pos[i] = pos_i + 3*np.array([np.cos(t[i]), np.sin(t[i])])

radii = np.random.uniform(.2, .3, size=(N,))

circle_vis(pos, radii)
