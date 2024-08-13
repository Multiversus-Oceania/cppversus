message('api_key: ' + API_KEY)

all_benchmark_deps = []
all_benchmark_dep_libs = []
all_benchmark_sources = []

gbenchmark_dep = benchmark.get_variable('google_benchmark_dep')
thread_dep = dependency('threads')
benchmark_main = files('benchmarks_main.cpp')

benchmark_deps = [
    gbenchmark_dep, 
    thread_dep,
    dependencies
]

benchmark_dep_libs = []

all_benchmark_sources += benchmark_main
all_benchmark_deps += benchmark_deps
all_benchmark_dep_libs += benchmark_dep_libs

subdir('dokken')

all_benchmarkes = executable('all_benchmarks', 
    all_benchmark_sources, 
    include_directories : includes, 
    dependencies : all_benchmark_deps, 
    link_with : all_benchmark_dep_libs
)