set delBuild=number
fxc "%delBuild%.hlsl" /Od /Zi /T vs_5_0 /E "VS" /Fo "%delBuild%-vs.cso"
fxc "%delBuild%.hlsl" /Od /Zi /T ps_5_0 /E "PS" /Fo "%delBuild%-ps.cso"
set /p eob="Finished building your shaders. Press enter to GTFO"