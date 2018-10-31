-- ----------------------------------------------------------------------------------------
-- MIT License
-- 
-- Copyright(c) 2018 Víctor Ávila
-- 
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files(the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions :
-- 
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.
-- 
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.
-- ----------------------------------------------------------------------------------------

PROJ_DIR = path.getabsolute("../")

-- // -- Options -- // --------------------------------------------------------------------------------------

newoption {
	trigger = "gl",
	description = "Set OpenGL backend.",
}

newoption {
	trigger = "dx11",
	description = "Set DirectX11 backend.",
}

newoption {
	trigger = "debug-tracing",
	description = "Enable MTR tracing.",
}

newoption {
	trigger = "no-console",
	description = "Launch a console with the window.",
}

newoption {
	trigger = "no-threading",
	description = "Set application to run on a single thread.",
}

newoption {
	trigger = "no-ui",
	description = "Disable UI building.",
}

-- // -- VXR Solution -- // ---------------------------------------------------------------------------------

solution "VXR-Engine"

	location "../project/vs/"
	configurations 		{ "Debug", "Release" }
	platforms 			{ "x32", "x64", "Native" }
	flags 				{  }

	configuration "Debug"
		defines 		{ "VXR_DEBUG" }
		flags 			{ "Symbols", "ExtraWarnings" }

	---configuration "Release"
		--defines 		{ "VXR_RELEASE" }
		--flags 			{ "Optimize" }

	configuration 		{ "vs*" }
		buildoptions{
			"/wd4100",
			"/wd4127",
			"/wd4244",
		}

	configuration "vs2015"
		windowstargetplatformversion "8.1"

	configuration "vs2017"
		windowstargetplatformversion "10.0.17134.0"

	language "C++"
	startproject "01-HelloWorld"


-- // -- VXR.lib -- // ---------------------------------------------------------------------------------

project "VXR"
	kind 		"StaticLib"
	language 	"C++"
	includedirs { 
		path.join(PROJ_DIR, "include/**.h"),
		-- // Dependencies // --
		path.join(PROJ_DIR, "deps/glm/**.h*"),
		path.join(PROJ_DIR, "deps/stb/**.h"),
		path.join(PROJ_DIR, "deps/mesh/**.h"),
		path.join(PROJ_DIR, "deps/imgui/*.h")
	}
	files {
		path.join(PROJ_DIR, "include/**.h"),
		path.join(PROJ_DIR, "src/**.h"),
		path.join(PROJ_DIR, "src/**.cpp"),
		-- // Dependencies // --
		path.join(PROJ_DIR, "deps/glm/**"),
		path.join(PROJ_DIR, "deps/stb/**"),
		path.join(PROJ_DIR, "deps/mesh/**"),
		path.join(PROJ_DIR, "deps/imgui/*.h"), 
		path.join(PROJ_DIR, "deps/imgui/*.cpp")
	}
	excludes {
		
	}

	if _OPTIONS["gl"] 				then
	includedirs { path.join(PROJ_DIR, "deps/glfw/**.h") }
	files		{ path.join(PROJ_DIR, "deps/glfw/**.h"), path.join(PROJ_DIR, "deps/glfw/src/**.c"), path.join(PROJ_DIR, "deps/glew/src/**.c"), }
	defines 	{ "VXR_OPENGL", "GLEW_STATIC", "_GLFW_WIN32" }
	end
	if _OPTIONS["dx11"] 			then
	defines 	{ "VXR_DX11", "UNICODE" }
	end

	if _OPTIONS["debug-tracing"] 	then
	includedirs { path.join(PROJ_DIR, "deps/minitrace/minitrace.h") }
	files		{ path.join(PROJ_DIR, "deps/minitrace/**") }
	defines 	{ "VXR_DEBUG_TRACING", "MTR_ENABLED" }
	end

	if not _OPTIONS["no-threading"]	then
	defines 	{ "VXR_THREADING" }
	end

	if not _OPTIONS["no-ui"]		then
	defines 	{ "VXR_UI" }
	end

	links { 
	}

-- // -- Projects -- // ---------------------------------------------------------------------------------

function makeProject(name)
	project (name)
		if _OPTIONS["no-console"] 	then
			kind "WindowedApp"
		else
			kind "ConsoleApp"
		end
		debugdir(path.join(PROJ_DIR, "examples/" .. name))
		includedirs 		{ path.join(PROJ_DIR, "examples/" .. name .. "/**") }
		files 				{ path.join(PROJ_DIR, "examples/" .. name .. "/**") }
		links 				{ "VXR" }
		language        	"C++"

		if _OPTIONS["gl"] 				then
		links 		{ "opengl32" }
		end
end

makeProject("dev")
makeProject("01-HelloWorld")
makeProject("02-Instancing")
makeProject("03-Framebuffers")
makeProject("04-Mesh")

makeProject("06-PlanetEditor")