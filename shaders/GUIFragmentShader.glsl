    #version 330 core

    in vec2 TextureFragColor;  // Input: Texture color
    flat in int GUITransparency; // Input: Whether to use transparency or not

    uniform sampler2D textureSampler; // Texture sampler

    out vec4 FinalColor;  // Output: Final fragment color

    void main() {

        FinalColor = texture(textureSampler, TextureFragColor); // Set the color to the texture color

        if (GUITransparency == 1)
            FinalColor.a = 0.3f;
    }