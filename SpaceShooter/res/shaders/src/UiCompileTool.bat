set delBuild=ui-shader
fxc "%delBuild%.hlsl" /Od /Zi /T vs_5_0 /E "VS" /Fo "../compiled/%delBuild%-vs.cso"
fxc "%delBuild%.hlsl" /Od /Zi /T ps_5_0 /E "PS" /Fo "../compiled/%delBuild%-ps.cso"
set /p eob="Finished building your shaders. Press enter to GTFO"