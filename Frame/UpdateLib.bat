xcopy	/y/s	.\Engine\Public\*.*							.\Reference\Header\
xcopy	/y/s	.\Engine\Bin\*.lib							.\Reference\Library\
xcopy	/y/s	.\Engine\ThirdPartyLib\*.lib				.\Reference\Library\

xcopy	/y/s	.\Engine\Bin\*.dll							.\Client\Bin\
xcopy	/y/s	.\Engine\Shader_Files\*.hlsl				.\Client\Shader_Files\
xcopy	/y/s	.\Reference\Fmod\x64\*.dll					.\Client\Bin\

xcopy	/y/s	.\Engine\ShaderFiles\*.hlsl					.\Client\ShaderFiles\

xcopy	/y/s	.\Engine\Bin\*.dll							.\Converter\Bin\