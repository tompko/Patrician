import itertools

common_files = []
main_files = ['main.c']
test_files=  ['test.c']

VariantDir(variant_dir='obj', src_dir='src', duplicate=False)
env = Environment()

main_build = ['obj/' + p for p in itertools.chain(common_files, main_files)]
test_build = ['obj/' + p for p in itertools.chain(common_files, test_files)]

env.Program(target='bin/Patrician', source=main_build)
env.Program(target='bin/test_suite', source=test_build)
