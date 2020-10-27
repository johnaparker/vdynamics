import vdynamics
import matplotlib as mpl
import numpy as np

def animate_2d(position, radius, colors, xlim=None, ylim=None, background_color='k', edge_color=(.8,.8,.8), linewidth=0):
    """
    Create a 2D animation of a trajectory of circles

    Arguments:
        position[T,N,2]       position array of T frames, N particles in 2D
        radius[N]             radius of N particles
        colors                colors of the particles
        xlim[2]               view limits of x-axis (default: auto-compute)
        xlim[2]               view limits of y-axis (default: auto-compute)
        background_color      color of the background (default: black)
        edge_color            color of circle edges
        linewidth             width of the edge (between 0 and 1)
    """
    root = vdynamics.__path__[0]
    vshader = f'{root}/resources/shaders/circle_vis.vs'
    fshader = f'{root}/resources/shaders/circle_vis.fs'
    rgba = mpl.colors.to_rgba_array(colors)
    background_color = mpl.colors.to_rgb(background_color)
    edge_color = mpl.colors.to_rgba(edge_color)

    if linewidth < 0 or linewidth > 1:
        raise ValueError('linewidth must be between 0 and 1')

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

    vdynamics.circle_vis(position=position,
                         radii=radius,
                         dims=dims,
                         colors=rgba,
                         background_color=background_color,
                         edge_color=edge_color,
                         linewidth=linewidth,
                         vshader=vshader,
                         fshader=fshader,)
