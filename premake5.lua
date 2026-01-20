workspace "unitest"
    configurations { "Debug", "Release" }
    location "build/"

    project "unitest"
        kind "None"
        location ("build/%{prj.name}/" .. _ACTION)
        language "C++"
        cppdialect "C++11"
        files "src/**.hpp"

    project "unitest.tests"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++11"
        location ("build/%{prj.name}/" .. _ACTION)
        targetdir "bin/%{prj.name}/${prj.cfg}"
        objdir "obj/"
        files {
            "tests/**.hpp",
            "tests/**.cpp"
        }
        includedirs {
            "src/",
            "tests/"
        }
        