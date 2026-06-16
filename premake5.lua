workspace "unitest"
    configurations { "Debug", "Release" }
    location "build/"

    project "unitest"
        kind "None"
        location ("build/%{prj.name}/" .. _ACTION)
        language "C++"
        cppdialect "C++11"
        warnings "Extra"
        fatalwarnings "All"
        files "src/**.hpp"

    project "unitest.tests"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++11"
        warnings "Extra"
        fatalwarnings "All"
        location ("build/%{prj.name}/" .. _ACTION)
        targetdir "bin/%{prj.name}/%{cfg.buildcfg}"
        objdir "obj/"
        files {
            "tests/**.hpp",
            "tests/**.cpp"
        }
        includedirs {
            "src/",
            "tests/"
        }

    project "unitest.tests.file_output"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++11"
        warnings "Extra"
        fatalwarnings "All"
        location ("build/%{prj.name}/" .. _ACTION)
        targetdir "bin/%{prj.name}/%{cfg.buildcfg}"
        objdir "obj/"
        files {
            "tests/**.hpp",
            "tests/**.cpp"
        }
        includedirs {
            "src/",
            "tests/"
        }
        defines "UNITEST_TESTS_FILE_OUTPUT"
        