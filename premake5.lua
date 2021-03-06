--
-- Premake script (http://premake.github.io)
--

solution 'WinsockTut'
    configurations  {'Debug', 'Release'}
    targetdir       'bin'
	architecture 	'x64'
    language 		'C++'

    filter 'configurations:Debug'
        defines 'DEBUG'
        symbols 'On'
        

    filter 'configurations:Release'
        defines 'NDEBUG'
        symbols 'On'
        optimize 'On'

    filter 'action:vs*'
        defines
        {
            'WIN32',
            'WIN32_LEAN_AND_MEAN',
            '_WIN32_WINNT=0x0600',
            '_CRT_SECURE_NO_WARNINGS',
            'NOMINMAX',
        }
        links 'ws2_32'

    project 'echo'
        location    'build'
        kind        'ConsoleApp' 
        files
        {
            'examples/echo/*.h',
            'examples/echo/*.cpp',
        }
        includedirs 'src'
        links 'libtutnet'
		
    project 'pingpong'
        location    'build'
        kind        'ConsoleApp' 
        files
        {
            'examples/pingpong/*.h',
            'examples/pingpong/*.cpp',
        }
        includedirs 'src'
        links 'libtutnet'
		
	project 'libtutnet'
        location 'build'
        kind 'StaticLib'

        files
        {
            'src/**.cpp',
            'src/**.h',
        }
        includedirs 'src'
            