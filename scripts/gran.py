import vdynamics
import numpy as np
from time import sleep
import h5py
import matplotlib as mpl

def circle_vis(position, radius, colors, xlim=None, ylim=None):
    root = vdynamics.__path__[0]
    vshader = f'{root}/resources/shaders/circle_vis.vs'
    fshader = f'{root}/resources/shaders/circle_vis.fs'
    rgba = mpl.colors.to_rgba_array(colors)

    rmax = np.max(radius)
    if xlim is not None:
        xmin, xmax = xlim
    else:
        xmin = np.min(position[...,0]) - rmax
        xmax = np.max(position[...,0]) + rmax
    if ylim is not None:
        ymin, ymax = ylim
    else:
        ymin = np.min(position[...,1]) - rmax
        ymax = np.max(position[...,1]) + rmax
    dims = np.array([[xmin, xmax], [ymin, ymax]], dtype=float)

    vdynamics.circle_vis(pos, radii, dims, rgba, vshader, fshader)


with h5py.File('gran3.h5', 'r') as f:
    pos = f['traj'][::100]
    radii = f['radii'][...]

colors = mpl.colors.TABLEAU_COLORS
# colors = np.random.uniform(0,1, (len(radii),4))
circle_vis(pos, radii, colors)
