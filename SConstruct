import itertools

def create_objs(environ, sources):
    return [environ.Object(s) for s in sources]

common_files = ['board.c']
main_files = ['main.c']
test_files=  ['test.c']

test_libs = ['libcheck']
lib_path = ['/usr/lib', '/usr/local/lib']

env = Environment(CCFLAGS='-std=c99')
env.VariantDir(variant_dir='obj', src_dir='src', duplicate=False)

main_build = ['obj/' + p for p in itertools.chain(common_files, main_files)]

env.Program(target='bin/Patrician', source=create_objs(env, main_build))

test_env = Environment(CCFLAGS='-std=c99', CPPPATH="src")
test_env.VariantDir(variant_dir='test_obj', src_dir='test', duplicate=False)
test_env.VariantDir(variant_dir='test_obj/obj', src_dir='src', duplicate=False)

test_build = ['test_obj/' + p for p in test_files] +\
             ['test_obj/obj/' + p for p in common_files]

test_env.Program(target='bin/test_suite', source=create_objs(test_env, test_build),
            LIBS=test_libs, LIBPATH=lib_path)
