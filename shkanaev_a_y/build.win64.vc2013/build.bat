set PATH="C:\Program Files (x86)\CMake\bin\";%PATH%
call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x64

call cmake.exe -G"Visual Studio 14 Win64" -DOpenCV_DIR="C:\opencv\build" ..
msbuild shkanaev_a_y.sln /property:Configuration=Debug /m
@rem msbuild polevoy_d_v.sln /property:Configuration=Release /m