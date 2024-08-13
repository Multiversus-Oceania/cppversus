init_benchmark_sources = [
    'init_benchmark.cpp'
]

init_benchmark_deps = [

]

init_benchmark_lib_deps = [
    cppversus_lib
]

init_benchmark = executable('init_benchmark', 
    [
        init_benchmark_sources,
        benchmark_main
    ], 
    include_directories : includes,
    dependencies : [
        init_benchmark_deps,benchmark_deps
    ], 
    link_with : [
        init_benchmark_lib_deps,
        benchmark_dep_libs
    ]
)

benchmark('Dokken Init Benchmark', init_benchmark)

all_benchmark_deps += init_benchmark_deps
all_benchmark_dep_libs += init_benchmark_lib_deps
project_benchmark_sources += files(init_benchmark_sources)