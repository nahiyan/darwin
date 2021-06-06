from os import system
from os.path import join
# from watchgod import watch, RegExpWatcher, Change
import watchgod
import sys

def format_all():
    system('gdformat ' + join('scripts', '*.gd'))

def watch():
    format_all()
    for changes in watchgod.watch('scripts', watcher_cls=watchgod.RegExpWatcher, watcher_kwargs=dict(re_files=r'^.*(\.gd)$')):
       for change in changes:
            if change[0] != watchgod.Change.deleted:
                system('gdformat ' + change[1])

if len(sys.argv) == 2 and sys.argv[1] == 'watch':
    watch()
else:
    format_all()