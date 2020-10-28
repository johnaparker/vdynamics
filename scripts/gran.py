import vdynamics
import h5py
import matplotlib as mpl

with h5py.File('gran3.h5', 'r') as f:
    pos = f['traj'][::100]
    radii = f['radii'][...]

colors = mpl.colors.TABLEAU_COLORS
colors = mpl.colors.to_rgba_array(colors)
colors[:,-1] = .5
vdynamics.animate_2d(pos, 1.2*radii, colors, background_color='white', linewidth=0.1, edge_color='k')
# vdynamics.animate_2d(pos, 1*radii, colors)
