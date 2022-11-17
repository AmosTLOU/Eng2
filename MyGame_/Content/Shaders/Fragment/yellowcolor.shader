/*
	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

DECLARE_FRAGMENT_INPUT_VARIABLES

// Entry Point
//============

// Using GL Syntax by default
FragmentShader_MainFunctionHead
{
	// Output solid white
	o_color = vec4(
		// RGB (color)
		1.0, 1.0, 0.0,
		// Alpha (opacity)
		1.0 );
}