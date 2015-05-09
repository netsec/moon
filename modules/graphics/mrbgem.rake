#encoding:UTF-8
MRuby::Gem::Specification.new('mruby-moon-graphics') do |spec|
  spec.license = 'MIT'
  spec.authors = ['Blaž Hrastnik', 'Corey Powell']
  spec.version = '0.0.0'
  spec.summary = 'Moon graphics modules'
  spec.description = 'Moon Engine\'s graphics modules'
  spec.homepage = 'https://github.com/IceDragon200/mruby-moon'
  # compiler config
  spec.cc.include_paths << ["#{build.root}/src", "#{build.root}/include"]
  # I think we inherited the parent flags, might as well flatten the array to
  # be sure we're working with Array<String> and not Array<String, Array<String>>
  spec.cc.flags = spec.cc.flags.flatten
  # Treat all warnings as errors, forces you to write compliant code
  [spec.cc, spec.cxx].each do |cc|
    cc.flags << '-Wall'
    cc.flags << '-Wextra'
  end

  #spec.cxx do |cxx|
  #  cxx.flags << '-std=c++11'
  #end

  spec.add_dependency 'mruby-moon-system'
end
