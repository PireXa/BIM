    #version 330 core

    in vec2 TextureFragColor;  // Input: Texture color

    uniform sampler2D textureSampler; // Texture sampler

    out vec4 FinalColor;  // Output: Final fragment color

    void main() {

        FinalColor = texture(textureSampler, TextureFragColor);
    }