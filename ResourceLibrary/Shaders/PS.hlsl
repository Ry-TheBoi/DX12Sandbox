struct PixelInput //Input parameters from VertexOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PixelInput input) : SV_TARGET
{
    return input.color;
}