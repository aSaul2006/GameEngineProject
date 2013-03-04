//=============================================================================
// PhongDirLtTex.fx by Frank Luna (C) 2004 All Rights Reserved.
//
// Phong directional light & texture.
//=============================================================================

struct Mtrl
{
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float  specPower;
};

struct DirLight
{
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 dirW;  
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 pos;
	float3 att;
	float  range;
};

struct SpotLight
{
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 pos;
	float3 att;
	float3 dirW;
	float  range;
	float  spot;
};


uniform extern float4x4		gWorld;
uniform extern float4x4		gWorldInvTrans;
uniform extern float4x4		gWVP;
uniform extern float3		gEyePosW;
uniform extern Mtrl			gMtrl;
uniform extern DirLight		gDirLight;
uniform extern PointLight	gPointLight;
uniform extern SpotLight	gSpotLight;
uniform extern Texture		gTex;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

// Computes Directional light
void ComputeDirLight(Mtrl mtrl, DirLight dirLight, float3 normal, float3 toEye,
				out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs. 
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -dirLight.dirW;

	// Add ambient term.
	ambient = mtrl.ambient * dirLight.ambient;

	// Add diffuse and specular term, provided the surface is in the light of sght of the light.
	float diffuseFactor = dot(lightVec, normal);
	
	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v	= reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mtrl.spec.w);

		diffuse = diffuseFactor * mtrl.diffuse * dirLight.diffuse;
		spec	= specFactor * mtrl.spec * dirLight.spec;
	}

};
void ComputePointLight (Mtrl mtrl, PointLight pLight, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = pLight.pos - pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	// Range test.
	if( d > pLight.range )
		return;
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = mtrl.ambient * pLight.ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mtrl.spec.w);
					
		diffuse = diffuseFactor * mtrl.diffuse * pLight.diffuse;
		spec    = specFactor * mtrl.spec * pLight.spec;
	}

	// Attenuate
	float att = 1.0f / dot(pLight.att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec    *= att;
};


 
struct OutputVS
{
    float4 posH    : POSITION0;
    float3 normalW : TEXCOORD0;
    float3 toEyeW  : TEXCOORD1;
    float2 tex0    : TEXCOORD2;
};

OutputVS PhongDirLtTexVS(float3 posL : POSITION0, float3 normalL : NORMAL0, float2 tex0: TEXCOORD0)
{
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;
	
	// Transform normal to world space.
	outVS.normalW = mul(float4(normalL, 0.0f), gWorldInvTrans).xyz;
	
	// Transform vertex position to world space.
	float3 posW  = mul(float4(posL, 1.0f), gWorld).xyz;
	
	// Compute the unit vector from the vertex to the eye.
	outVS.toEyeW = gEyePosW - posW;
	
	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), gWVP);
	
	// Pass on texture coordinates to be interpolated in rasterization.
	outVS.tex0 = tex0;

	// Done--return the output.
    return outVS;
}

float4 PhongDirLtTexPS(OutputVS pin) : COLOR
{
	// Interpolated normals can become unnormal--so normalize.
	float3 posW = pin.posH.xyz;
	pin.normalW = normalize(pin.normalW);
	pin.toEyeW  = normalize(pin.toEyeW);
	
	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	float4 A, D, S;

	ComputeDirLight(gMtrl, gDirLight, pin.normalW, pin.toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec	+= S;
	//ComputePointLight(gMtrl, gPointLight, posW, pin.normalW, pin.toEyeW, A, D, S);
	//ambient += A;
	//diffuse += D;
	//spec	+= S;

	// Get the texture color.
	float4 texColor = tex2D(TexS, pin.tex0);
	
	// Combine the color from lighting with the texture color.
	float3 color = (ambient + diffuse)*texColor.rgb + spec;
		
	// Sum all the terms together and copy over the diffuse alpha.
    return float4(color, gMtrl.diffuse.a*texColor.a);

}

technique DirLightTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 PhongDirLtTexVS();
        pixelShader  = compile ps_2_0 PhongDirLtTexPS();
    }
}