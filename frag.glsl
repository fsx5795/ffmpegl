#version 450
out vec4 rgbaColor;
in vec2 texCoord;
uniform sampler2D yTexture;
uniform sampler2D uTexture;
uniform sampler2D vTexture;
void main()
{
    float y = texture(yTexture, texCoord).r;
    float cb = texture(uTexture, texCoord).r;
    float cr = texture(vTexture, texCoord).r;
    //YCbCr转RGB
    float r = y + 1.403 * (cr - 0.5);
    float g = y - 0.343 * (cb - 0.5) - 0.714 * (cr - 0.5);
    float b = y + 1.770 * (cb - 0.5);
    rgbaColor = vec4(r, g, b, 1.0);
}