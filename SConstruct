import itertools
import os.path
import subprocess

def create_objs(environ, sources):
    return [environ.Object(s) for s in sources]

check_bld = Builder(action='checkmk < $SOURCE > $TARGET')

def builder_unit_test(target, source, env):
    app = str(source[0].abspath)
    if subprocess.call(app) == 0:
        with open(target[0].abspath, "w") as fout:
            fout.write("PASSED\n")
    else:
        return 1
unit_test = Builder(action=builder_unit_test)

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
release = release_env.Program(target='bin/Patrician',
			source=create_objs(release_env, release_files),
			LIBS=lib_files)
release_env.Alias("release", "bin/Patrician")
Default(release)

test_files = subprocess.check_output(["find", "tests", "-name", "*.check"]).split()
non_main_files = [f for f in sources if not f.endswith("main.c")]
non_main_files = [os.path.join("obj", "test", nmf) for nmf in non_main_files]

test_env = Environment(CCFLAGS=c_flags, CPPPATH="src")
test_env.VariantDir(variant_dir='obj/test', src_dir='src', duplicate=False)
test_env.Append(BUILDERS = {'UnitTest': unit_test, 'Check': check_bld})
test_env.Command(target='#obj/test/test.check',
                 source=[File(tf) for tf in test_files],
                 action="python tests/cat_tests.py $SOURCES > $TARGET")
test_src = test_env.Check("#obj/test/test.c", "#obj/test/test.check")
test_prog = test_env.Program(target='bin/unittest',
                 source=create_objs(test_env, non_main_files + [test_src]),
                 LIBS=lib_files + ["check", "m"])
test_env.UnitTest("bin/unittest.passed", test_prog)
test_env.Alias("test", "bin/unittest.passed")

utils_env = Environment(CCFLAGS=c_flags, CPPPATH="src")
magic_finder = utils_env.Program(target="utils/magic_finder", source="utils/magic_finder.c")
utils_env.Alias("magic_finder", "utils/magic_finder")

