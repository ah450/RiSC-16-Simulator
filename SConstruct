import os

def make_package(env, tag, package) :
    objs = SConscript(os.path.join(package,"SConscript"), exports={'env' : env, 'tag' :  tag})
    return objs






paths =  ' -Iinclude '
#add libs here
libs = Split( '  ' )

#debug options
debug = Environment(ENV = {'PATH' : os.environ['PATH']})
debug['CXX'] = 'g++'
debug['CXXFLAGS'] = '-Wall -std=c++1y -O0 -g -fPIE' + paths
debug.Append( LINKFLAGS = Split(' -fPIE') )

#release options
release = Environment(ENV = {'PATH' : os.environ['PATH']})
release['CXX'] = 'g++-4.8'
release['CXXFLAGS'] = '-Wall -std=c++1y -O3 -fPIE -flto' + paths
release.Append( LINKFLAGS = Split(' -O3 -flto -fPIE') )


#profiling options
profile = Environment(ENV = {'PATH' : os.environ['PATH']})
profile['CXX'] = 'g++-4.8'
profile['CXXFLAGS'] = '-Wall -std=c++1y -fPIE  -pg -g' + paths
profile.Append( LINKFLAGS = Split(' -fPIE -pg') )





#targets
d_objs = make_package(debug, 'dbg', 'src') + make_package(debug, 'dbg', 'src/ui') + make_package(debug, 'dbg', 'src/asm') + make_package(debug, 'dbg', 'src/core')
d_target = debug.Program('RiSC-16-dbg', d_objs, LIBS=libs)

r_objs = make_package(release, 'rel', 'src') + make_package(release, 'rel', 'src/ui') + make_package(release, 'rel', 'src/asm') + make_package(release, 'rel', 'src/core')
r_target = release.Program('RiSC-16', r_objs, LIBS=libs)

p_objs = make_package(profile, 'prof', 'src') + make_package(profile, 'prof', 'src/ui') + make_package(profile, 'prof', 'src/asm') + make_package(profile, 'prof', 'src/core')
p_target = profile.Program('RiSC-16-profile', p_objs, LIBS=libs)

Default(d_target)
