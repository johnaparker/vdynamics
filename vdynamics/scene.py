from . import _vdynamics
from collections import Iterable

class Scene(_vdynamics.Scene):
    def run(self, callback, frames):
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
