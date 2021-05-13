project "shaderc"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"shaderc/libshaderc/src/**.cc",
		"shaderc/libshaderc_util/src/**.cc",
		"shaderc/glslc/src/**.cc",
	}

	includedirs
	{
		"shaderc/libshaderc/include",
		"shaderc/libshaderc_util/include",
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
