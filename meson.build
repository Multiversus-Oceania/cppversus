project(
    'cppversus',
    'cpp',
    version : '1.0',
    default_options : [
        'cpp_std=c++20',
        'warning_level=2'
    ]
)

cli_sources = []

cmd = run_command('sh', '-c', 'echo $MULTIVERSUS_TOKEN')
API_KEY = cmd.stdout().strip()

add_project_arguments('-DAPI_KEY="' + API_KEY + '"', language : 'cpp')

includes = [
    include_directories('include')
]

dependencies = [
    dependency('spdlog'),
    dependency('fmt'),
    dependency('nlohmann_json'),
    dependency('cpr')
]

subdir('src')


cppversus_dep = declare_dependency(
    dependencies: dependencies,
    include_directories: includes,
    link_with: [
        cppversus_common_lib,
        dokken_lib
    ]
)

meson.override_dependency('cppversus', cppversus_dep)

# if get_option('enable-tests')
#     gtest = subproject('gtest')
#     subdir('tests')
# endif
# if get_option('enable-benchmarks')
#     benchmark = subproject('google-benchmark')

#     subdir('benchmarks')
# endif
if get_option('enable-docs')
    subdir('docs')
endif