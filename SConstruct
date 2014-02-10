import os

def make_package(env, tag, package) :
    objs = SConscript(os.path.join(package,"SConscript"), exports={'env' : env, 'tag' :  tag})
    return objs







# Add libs here
libs = Split( ' -lboost_regex -lboost_filesystem -lboost_system' )

# Debug options
debug = Environment(ENV = {'PATH' : os.environ['PATH']}, CXX = 'g++-4.8',
            CPPFLAGS = '-Wall -std=c++11 -O0 -g -fPIE -Iinclude -DDEBUG_ME_SOFTLY ', 
            LINKFLAGS = Split(' -fPIE'), LIBS=libs)


# Release options
release = Environment(ENV = {'PATH' : os.environ['PATH']}, CXX = 'g++-4.8', 
        CPPFLAGS = '-Wall -std=c++11 -O3 -fPIE -flto -Iinclude', 
        LINKFLAGS = Split(' -O3 -flto -fPIE'), LIBS=libs)


# Profiling options
profile = Environment(ENV = {'PATH' : os.environ['PATH']}, CXX = 'g++-4.8',
            CPPFLAGS = '-Wall -std=c++11 -fPIE  -pg -g -Iinclude',
            LINKFLAGS = Split(' -fPIE -pg'), LIBS=libs )




packages = ['src', 'src/ui', 'src/asm', 'src/core']

# Objects
d_objs = [make_package(debug, 'dbg', p) for p in packages]
r_objs = [make_package(release, 'rel', p) for p in packages]
p_objs = [make_package(profile, 'prof', p) for p in packages]



# Targets
d_target = debug.Program('RiSC-16-dbg', d_objs)
r_target = release.Program('RiSC-16', r_objs)
p_target = profile.Program('RiSC-16-profile', p_objs)
Default(d_target)
