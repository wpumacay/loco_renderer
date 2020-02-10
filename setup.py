#!/usr/bin/env python

import os
import sys
import glob
import subprocess

from setuptools import find_packages, setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install

# @todo: fix issue of not removing the shared libraries even after pip uninstall is run

VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_MICRO = 4
VERSION = '%d.%d.%d' % ( VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO )
PREFIX = 'wp_tinyrenderer_%s_' % ( VERSION )

def BuildBindings( sourceDir, buildDir, cmakeArgs, buildArgs, env ):
    if not os.path.exists( buildDir ) :
        os.makedirs( buildDir )

    subprocess.call( ['cmake', sourceDir] + cmakeArgs, cwd=buildDir, env=env )
    subprocess.call( ['cmake', '--build', '.'] + buildArgs, cwd=buildDir )

# get installation path: https://stackoverflow.com/questions/36187264/how-to-get-installation-directory-using-setuptools-and-pkg-ressources
def GetInstallationDir() :
    py_version = '%s.%s' % ( sys.version_info[0], sys.version_info[1] )
    install_path_candidates = ( path % (py_version) for path in (
                        sys.prefix + '/lib/python%s/dist-packages/',
                        sys.prefix + '/lib/python%s/site-packages/',
                        sys.prefix + '/local/lib/python%s/dist-packages/',
                        sys.prefix + '/local/lib/python%s/site-packages/',
                        '/Library/Python/%s/site-packages/' ) )
    for path_candidate in install_path_candidates :
        if os.path.exists( path_candidate ) :
            return path_candidate

    print( 'ERROR >>> No installation path found', file=sys.stderr )
    return None

def GetFilesUnderPath( path, extension ) :
    cwd_path = os.getcwd()
    target_path = os.path.join( cwd_path, path )
    if not os.path.exists( target_path ) :
        return ( '', [] )

    os.chdir( target_path )
    files = glob.glob( '**/*.%s' % ( extension ), recursive=True )
    files_paths = [ os.path.join( target_path, fpath ) for fpath in files ]
    os.chdir( cwd_path )
    return ( PREFIX + path, files_paths )

class CMakeExtension( Extension ) :

    def __init__( self, name, sourceDir ) :
        super( CMakeExtension, self ).__init__( name, sources=[] )
        self.sourceDir = os.path.abspath( sourceDir )

class BuildCommand( build_ext ) :

    user_options = [ ( 'headless=', None, 'Whether to build in headless mode (uses EGL) or not (uses GLFW)' ),
                     ( 'debug=', None, 'Whether to build in debug-mode or not' ) ]
    boolean_options = [ 'headless', 'debug' ]

    def initialize_options( self ) :
        super( BuildCommand, self ).initialize_options()
        self.headless = 0 # Build using non-headless mode (GLFW) by default
        self.debug = 0 # Build in release mode by default

    def run( self ) :
        try :
            _ = subprocess.check_output( ['cmake', '--version'] )
        except OSError :
            raise RuntimeError( 'CMake must be installed to build the following extensions: ' +
                                ', '.join( e.name for e in self.extensions ) )

        for _extension in self.extensions :
            self.build_extension( _extension )

    def build_extension( self, extension ) :
        _extensionFullPath = self.get_ext_fullpath( extension.name )
        _extensionDirName = os.path.dirname( _extensionFullPath )
        _extensionDirPath = os.path.abspath( _extensionDirName )

        _cfg = 'Debug' if self.debug else 'Release'
        _egl = 'ON' if self.headless else 'OFF'
        _buildArgs = ['--config', _cfg, '--', '-j4']
        _cmakeArgs = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + _extensionDirPath,
                      '-DCMAKE_BUILD_RPATH=' + GetInstallationDir(),
                      '-DCMAKE_INSTALL_RPATH=' + GetInstallationDir(),
                      '-DPYTHON_EXECUTABLE=' + sys.executable,
                      '-DCMAKE_BUILD_TYPE=' + _cfg,
                      '-DTINYRENDERER_RESOURCES_PATH=' + sys.prefix + '/' + PREFIX + 'res/',
                      '-DTINYRENDERER_BUILD_HEADLESS_EGL=' + _egl,
                      '-DTINYRENDERER_BUILD_DOCS=OFF',
                      '-DTINYRENDERER_BUILD_EXAMPLES=OFF',
                      '-DTINYRENDERER_BUILD_PYTHON_BINDINGS=ON',
                      '-DTINYRENDERER_BUILD_WITH_LOGS=OFF',
                      '-DTINYRENDERER_BUILD_WITH_TRACK_ALLOCS=OFF']

        _env = os.environ.copy()
        _env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format( _env.get( 'CXXFLAGS', '' ),
                                                                self.distribution.get_version() )

        _sourceDir = extension.sourceDir
        _buildDir = self.build_temp

        BuildBindings( _sourceDir, _buildDir, _cmakeArgs, _buildArgs, _env )

class InstallCommand( install ) :

    user_options = install.user_options + BuildCommand.user_options
    boolean_options = install.boolean_options + BuildCommand.boolean_options

    def initialize_options( self ) :
        super( InstallCommand, self ).initialize_options()
        self.headless = 0 # Build using non-headless mode (GLFW) by default
        self.debug = 0 # Build in release mode by default

    def run( self ) :
        self.reinitialize_command( 'build_ext', headless = self.headless, debug = self.debug )
        self.run_command( 'build_ext' )
        super( InstallCommand, self ).run()

with open( 'README.md', 'r' ) as fh :
    longDescriptionData = fh.read()

with open( 'requirements.txt', 'r' ) as fh :
    requiredPackages = [ line.replace( '\n', '' ) for line in fh.readlines() ]

setup(
    name                            = 'wp-tinyrenderer',
    version                         = VERSION,
    description                     = 'A minimal renderer for prototyping 3D applications',
    long_description                = longDescriptionData,
    long_description_content_type   = 'text/markdown',
    author                          = 'Wilbert Santos Pumacay Huallpa',
    license                         = 'MIT License',
    author_email                    = 'wpumacay@gmail.com',
    url                             = 'https://github.com/wpumacay/tiny_renderer',
    keywords                        = 'graphics opengl',
    classifiers                     = [ "License :: OSI Approved :: MIT License",
                                        "Operating System :: POSIX :: Linux" ],
    zip_safe                        = False,
    install_requires                = requiredPackages,
    package_dir                     = { '' : './python' },
    packages                        = find_packages( './python' ),
    data_files                      = [ GetFilesUnderPath( 'res/imgs', 'png' ),
                                        GetFilesUnderPath( 'res/imgs', 'jpg' ),
                                        GetFilesUnderPath( 'res/imgs/skyboxes', 'tga' ),
                                        GetFilesUnderPath( 'res/imgs/skyboxes', 'jpg' ),
                                        GetFilesUnderPath( 'res/models', 'stl' ),
                                        GetFilesUnderPath( 'res/models/fox', 'png' ),
                                        GetFilesUnderPath( 'res/models/fox', 'obj' ),
                                        GetFilesUnderPath( 'res/models/fox', 'mtl' ),
                                        GetFilesUnderPath( 'res/models/pokemons/lizardon', 'png' ),
                                        GetFilesUnderPath( 'res/models/pokemons/lizardon', 'obj' ),
                                        GetFilesUnderPath( 'res/models/pokemons/lizardon', 'mtl' ),
                                        GetFilesUnderPath( 'res/shaders', 'glsl' ) ],
    ext_modules                     = [ CMakeExtension( 'tr_core', '.' ) ],
    cmdclass                        = dict( build_ext = BuildCommand, install = InstallCommand ) 
)