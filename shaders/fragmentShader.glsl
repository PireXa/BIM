    #version 330 core

    in vec2 TextureFragColor;  // Input: Texture color
    in vec3 NormalFragColor;  // Input: Normal color
    flat in float TransitionBlendFactor; // Input: The blend factor for the transition effect

	uniform sampler2D textureSampler; // Texture sampler

    out vec4 FinalColor;  // Output: Final fragment color

    void main() {

        FinalColor = mix(vec4(NormalFragColor, 1.0f), texture(textureSampler, TextureFragColor), TransitionBlendFactor);
    }
