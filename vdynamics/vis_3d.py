import vdynamics
import matplotlib as mpl
import numpy as np

def get_texture_path(name):
    root = vdynamics.__path__[0]

    textures = dict(smiley='smiley.png',
                    metal='metal.png',
                    asphalt='asphalt.png',
                    wheel='wheel.png')

    texure_file = textures[name]

    return f'{root}/resources/textures/{texure_file}'

def animate_3d(position, radius, colors, background_color='k', textures=None, texture_mix=0):
    """
    Create a 3D animation of a trajectory of spheres

    Arguments:
        position[T,N,3]       position array of T frames, N particles in 2D
        radius[N]             radius of N particles
        colors                colors of the particles
        background_color      color of the background (default: black)
        textures              list of textures (default: none)
        texture_mix           blend texture with color data (between 0 and 1; default 0)
    """
    rgba = mpl.colors.to_rgba_array(colors)
    background_color = mpl.colors.to_rgb(background_color)

    if textures is None:
        textures = []
    else:
        textures = [get_texture_path(name) for name in textures]

    root = vdynamics.__path__[0]
    if textures:
        shader_type = 'sphere_textured'
    else:
        shader_type = 'sphere'
    vshader = f'{root}/resources/shaders/{shader_type}.vs'
    fshader = f'{root}/resources/shaders/{shader_type}.fs'

    rmax = np.max(radius)
    xmin = np.min(position[...,0]) - rmax
    xmax = np.max(position[...,0]) + rmax
    ymin = np.min(position[...,1]) - rmax
    ymax = np.max(position[...,1]) + rmax
    dims = np.array([[xmin, xmax], [ymin, ymax]], dtype=float)

    vdynamics.sphere_vis(position=position,
                         radii=radius,
                         dims=dims,
                         colors=rgba,
                         background_color=background_color,
                         texture_files=textures,
                         texture_mix=texture_mix,
                         vshader=vshader,
                         fshader=fshader)
