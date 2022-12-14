add_requires("spdlog", {configs={fmt_external=true}})
add_requires('magic_enum')
add_requires("cxxopts")
target('krill')
  set_kind('static')
  set_languages('cxx17')
  add_includedirs('krill/krill/include')
  add_files('krill/krill/src/**.cpp')
  add_packages('spdlog', 'magic_enum')

target("mias")
  set_languages('cxx17')
  add_files("src/**.cpp")
  add_packages("cxxopts")
