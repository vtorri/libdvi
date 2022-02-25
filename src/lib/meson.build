
libdvi_src = files([
  'dvi_document.c',
  'dvi_font.c',
  'dvi_interpreter.c',
  'dvi_kpathsea.c',
  'dvi_log.c',
  'dvi_main.c',
  'dvi_map.c',
  'dvi_page.c',
#  'dvi_process.c',
  'dvi_stack.c'
])

inc_dir = include_directories('.')

libdvi_cflags = dev_cflags

if sys_windows
  libdvi_cflags += '-DDVI_BUILD'
endif

if get_option('buildtype') == 'release'
  libdvi_cflags += '-DNDEBUG'
else
  libdvi_cflags += '-D_DEBUG'
endif

if get_option('default_library') == 'shared'
  libdvi_cflags += '-DDLL_EXPORT'
endif

# kpathsea

kpathsea_dep = cc.find_library(get_option('kpathsea-name'),
  dirs : get_option('kpathsea-path'),
  required : true
)

have_texlive = cc.has_function('kpathsea_new',
  dependencies : kpathsea_dep
)

config_h.set10('HAVE_TEXLIVE', have_texlive)

libdvi_lib = library('libdvi', libdvi_src,
  c_args : libdvi_cflags,
  dependencies : kpathsea_dep,
  gnu_symbol_visibility: 'hidden',
  include_directories : inc_config_dir,
  install : true,
  pic : true,
  version : meson.project_version()
)

libdvi = declare_dependency(
  dependencies : kpathsea_dep,
  include_directories : inc_dir,
  link_with : libdvi_lib,
)

install_headers('Dvi.h',
  install_dir : dir_pkginclude + '-' + v_maj
)
