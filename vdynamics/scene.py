from . import _vdynamics
from collections import Iterable

class Scene(_vdynamics.Scene):
    def run(self, callback=None, frames=None):
        if callback is None:
            callback = lambda t: t
        if frames is None:
            frames = 1

        if isinstance(frames, Iterable):
            Nframes = len(frames)
        elif isinstance(frames, int):
            Nframes = frames
            frames = range(Nframes)
        else:
            raise ValueError('frames must be iterable or an integer')

        def callback_wrap(arg):
            callback(frames[arg])

        super().run(callback_wrap, Nframes)
