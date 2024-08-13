project(
    'cppversus',
    'cpp',
    version : '1.0',
    default_options : [
        'cpp_std=c++20',
        'warning_level=2'
    ]
)

cmd = run_command('sh', '-c', 'echo $MULTIVERSUS_TOKEN')
API_KEY = cmd.stdout().strip()

add_project_arguments('-DAPI_KEY="' + API_KEY + '"', language : 'cpp')

project_test_sources = []
project_benchmark_sources = []

includes = [
    include_directories('include')
]

json = subproject('json')
dependencies = [
    dependency('spdlog'),
    dependency('fmt'),
    json.get_variable('nlohmann_json_dep'),
    dependency('cpr')
]

cppversus_lib = shared_library('cppversus', 
    [
        'src/dokken/main.cpp',
        'src/dokken/player_info.cpp'
    ], 
    include_directories : includes,
    dependencies : dependencies,
    link_with : [

    ],
    install : true
)


if get_option('enable-tests')
    gtest = subproject('gtest')
    subdir('tests')
endif
if get_option('enable-benchmarks')
    benchmark = subproject('google-benchmark')

    subdir('benchmarks')
endif
if get_option('enable-docs')
    subdir('docs')
endif