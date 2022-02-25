
project('libdvi', 'c',
  version: '0.0.1',
  default_options : [
    'buildtype=debugoptimized',
    'b_ndebug=if-release',
    'warning_level=3',
    'werror=true',
    'c_std=c99'
  ],
  meson_version : '>=0.53'
)

v_arr = meson.project_version().split('.')
v_maj = v_arr[0]
v_min = v_arr[1]
v_mic = v_arr[2]

# install paths
dir_prefix = get_option('prefix')
dir_include = join_paths(dir_prefix, get_option('includedir'))
dir_pkginclude = join_paths(dir_include, meson.project_name())
dir_bin = join_paths(dir_prefix, get_option('bindir'))
dir_lib = join_paths(dir_prefix, get_option('libdir'))
dir_data = join_paths(dir_prefix, get_option('datadir'))
dir_pkgdata = join_paths(dir_data, meson.project_name())
dir_locale = join_paths(dir_prefix, get_option('localedir'))

# host

windows = import('windows')

host_os = host_machine.system()

win = ['windows', 'cygwin']
#bsd for meson 0.46 and 0.47
bsd = ['bsd', 'freebsd', 'dragonfly', 'netbsd', 'openbsd']
linux = ['linux']
osx = ['darwin']
sun = ['sunos']

sys_linux = linux.contains(host_machine.system())
sys_bsd = bsd.contains(host_machine.system())
sys_windows = win.contains(host_machine.system())
sys_osx = osx.contains(host_machine.system())
sys_sun = sun.contains(host_machine.system())

# compiler

cc = meson.get_compiler('c')

dev_cflags = []

dev_cflags_try = [
  '-Wcomment',
  '-Wdeclaration-after-statement',
  '-Wfloat-compare',
  '-Wformat',
  '-Wmissing-declarations',
  '-Wmissing-prototypes',
  '-Wno-long-long',
  '-Wno-missing-field-initializers',
  '-Wpointer-arith',
  '-Wredundant-decls',
  '-Wshadow',
  '-Wsign-compare',
  '-Wstrict-prototypes',
  '-Wuninitialized',
  '-fno-omit-frame-pointer'
]

if sys_windows
  if cc.get_id() != 'msvc'
    dev_cflags_try += '-Wno-pedantic-ms-format'
  endif
else
  dev_cflags_try += '-fvisibility=hidden'
endif

foreach cf: dev_cflags_try
  if cc.has_argument(cf)
    dev_cflags += cf
  endif
endforeach

inc_config_dir = include_directories('.')

# configuration

config_h = configuration_data()
config_h.set_quoted('PACKAGE_NAME', meson.project_name())
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
config_h.set_quoted('PACKAGE_BIN_DIR', dir_bin)
config_h.set_quoted('PACKAGE_LIB_DIR', dir_lib)
config_h.set_quoted('PACKAGE_DATA_DIR', dir_data)
config_h.set_quoted('LOCALEDIR', dir_locale)

subdir('src/lib')

# Use config_h after all subdirs have set values

configure_file(output : 'config.h', configuration : config_h)

# output

summary({'OS': host_os,
         'release mode': get_option('buildtype') == 'release' ? 'yes' : 'no',
         'kpathsea dist.': have_texlive ? 'TeXLive' : 'MIKTeX',
        }, section: 'Configuration Options Summary:')

summary({'prefix': dir_prefix,
         'bindir': dir_bin,
         'libdir': dir_lib,
         'incdir': dir_include,
         'pkgincdir': dir_pkginclude,
         'datadir': dir_data,
         'pkgdatadir': dir_pkgdata,
        }, section: 'Directories:')

summary({'compilation': 'ninja',
         'installation': 'ninja install',
        }, section: 'Compilation')
