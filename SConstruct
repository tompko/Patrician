import itertools
import os.path
import subprocess

def create_objs(environ, sources):
    return [environ.Object(s) for s in sources]

sources = subprocess.check_output(["find", "src", "-name", "*.c"]).split()
sources = [os.path.relpath(s, "src") for s in sources]

lib_files = ["rt", "pthread"]
c_flags = "-Wall -Werror -g -ggdb"

debug_files = [os.path.join("obj", "debug", s) for s in sources]

debug_env = Environment(CCFLAGS=c_flags, CPPPATH="src")
debug_env.VariantDir(variant_dir='obj/debug', src_dir='src', duplicate=False)
debug = debug_env.Program(target='bin/debug',
			source=create_objs(debug_env, debug_files),
			LIBS=lib_files)
debug_env.Alias("debug", "bin/debug")

c_flags = "-Wall -Werror -O3 -g -ggdb -DNDEBUG"

release_files = [os.path.join("obj", "release", s) for s in sources]

release_env = Environment(CCFLAGS=c_flags, CPPPATH="src")
release_env.VariantDir(variant_dir='obj/release', src_dir='src', duplicate=False)
release = release_env.Program(target='#bin/Patrician',
			source=create_objs(release_env, release_files),
			LIBS=lib_files)
release_env.Alias("release", "bin/Patrician")
Default(release)

