#include "include\\Common.fxh"

/************* Resources *************/
//per object
float4x4 WorldViewProjection : WORLDVIEWPROJECTION; 
float4x4 World : WORLD;
float4 SpecularColor : SPECULARCOLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
float SpecularPower : SPECULARPOWER = { 25.0f };

//per frame
float3 LightDirection : LIGHTDIRECTION;
float3 CameraPosition : CAMERAPOSITION;
float4 AmbientColor : AMBIENTCOLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
float4 LightColor : LIGHTCOLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

//Texture(s)
Texture2D ColorTexture;

//State Settings
RasterizerState DisableCulling { CullMode = NONE; };
SamplerState ColorSampler { Filter = MIN_MAG_MIP_LINEAR; AddressU = WRAP; AddressV = WRAP; };

/************* Data Structures *************/

struct VS_INPUT
{
	float4 ObjectPosition: POSITION;
	float2 TextureCoordinate : TEXCOORD;
	float3 Normal : NORMAL;
};

struct VS_OUTPUT 
{
    float4 Pos: SV_Position;
	float3 Normal : NORMAL;
	float2 TextureCoordinate : TEXCOORD;
	float3 LightDirection : TEXCOORD1;
	float3 ViewDirection : TEXCOORD2;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;
    
    OUT.Pos = mul(IN.ObjectPosition, WorldViewProjection);
	OUT.TextureCoordinate = get_corrected_texture_coordinate(IN.TextureCoordinate);
	OUT.Normal = normalize(mul(float4(IN.Normal, 0), World).xyz);
	OUT.LightDirection = normalize(-LightDirection);
	
	float3 worldPosition = mul(IN.ObjectPosition, World).xyz;
	OUT.ViewDirection = normalize(CameraPosition - worldPosition);
    
    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float4 OUT = (float4)0;

	float3 normal = normalize(IN.Normal);
	float3 lightDirection = normalize(IN.LightDirection);
	float3 viewDirection = normalize(IN.ViewDirection);
	float n_dot_l = dot(lightDirection, normal);

	float4 color = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);

		float3 ambient = AmbientColor.rgb * AmbientColor.a * color.rgb;

		float3 diffuse = (float3)0;
		float3 specular = (float3)0;

		if (n_dot_l > 0)
		{
			diffuse = LightColor.rgb * LightColor.a *saturate(n_dot_l) * color.rgb;

			float3 reflectionVector = normalize(2 * n_dot_l * normal - lightDirection);

				specular = SpecularColor.rgb * SpecularColor.a * min(pow(saturate(dot(reflectionVector, viewDirection)), SpecularPower), color.w);
		}

	OUT.rgb = ambient + diffuse + specular;
	OUT.a = color.a;
	return OUT;
}

/************* Techniques *************/

technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));

        SetRasterizerState(DisableCulling);
    }
}