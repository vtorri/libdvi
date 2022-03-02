
dvi_bin_src = [
  'dvi.c'
]

inc_dir = include_directories('../lib')

dvi_bin = executable('dvi', dvi_bin_src,
  dependencies : [ libdvi ],
  include_directories : inc_dir,
  install : true
)
