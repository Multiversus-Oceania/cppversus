cdata = configuration_data()
cdata.set('TOP_SRCDIR', meson.project_source_root())
cdata.set('TOP_BUILDDIR', meson.project_build_root())
cdata.set('OUTPUT_DIR', join_paths(meson.project_build_root(),'docs'))
cdata.set('README_PATH', join_paths(meson.project_source_root(),'README.md'))
cdata.set('VERSION', meson.project_version())
cdata.set('PROJECT_NAME', meson.project_name())

doxyfile = configure_file(input: 'Doxyfile.in',
                          output: 'Doxyfile',
                          configuration: cdata,
                          install: false)

doxygen = find_program('doxygen', required : false)

if doxygen.found()
    run_target(
        'doc',
        command: [
            doxygen,
            doxyfile
        ]
    )
endif