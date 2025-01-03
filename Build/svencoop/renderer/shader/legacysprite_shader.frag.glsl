#version 430 compatibility

#include "common.h"

layout(binding = 0) uniform sampler2D diffuseTex;

in vec3 v_worldpos;
in vec2 v_diffusetexcoord;
in vec4 v_color;
in vec4 v_projpos;

layout(location = 0) out vec4 out_Diffuse;

void main()
{
	ClipPlaneTest(v_worldpos.xyz, -SceneUBO.vpn.xyz);

	vec4 baseColor = texture(diffuseTex, v_diffusetexcoord.xy);

	baseColor = ProcessDiffuseColor(baseColor);

	vec4 lightmapColor = v_color;

	lightmapColor.r = clamp(lightmapColor.r, 0.0, 1.0);
	lightmapColor.g = clamp(lightmapColor.g, 0.0, 1.0);
	lightmapColor.b = clamp(lightmapColor.b, 0.0, 1.0);
	lightmapColor.a = clamp(lightmapColor.a, 0.0, 1.0);
	
	lightmapColor = ProcessOtherGammaColor(lightmapColor);

	#if !defined(ADDITIVE_BLEND_ENABLED)
		vec4 finalColor = CalcFog(baseColor * lightmapColor);
	#else
		vec4 finalColor = baseColor * lightmapColor;
	#endif

	#if defined(OIT_BLEND_ENABLED)
		
		GatherFragment(finalColor);

	#endif

	out_Diffuse = finalColor;
	
}