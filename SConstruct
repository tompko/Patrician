import itertools

def create_objs(sources):
    return [env.Object(s) for s in sources]

common_files = ['board.c']
main_files = ['main.c']
test_files=  ['test.c']

test_libs = ['libcheck']
lib_path = ['/usr/lib', '/usr/local/lib']

VariantDir(variant_dir='obj', src_dir='src', duplicate=False)
env = Environment(CCFLAGS='-std=c99')

main_build = ['obj/' + p for p in itertools.chain(common_files, main_files)]
test_build = ['obj/' + p for p in itertools.chain(common_files, test_files)]

env.Program(target='bin/Patrician', source=create_objs(main_build))
env.Program(target='bin/test_suite', source=create_objs(test_build),
            LIBS=test_libs, LIBPATH=lib_path)
