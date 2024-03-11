    #version 330 core

    in vec2 TextureFragColor;  // Input: Vertex color (vec3 if using classic rainbow effect,vec2 for texture)
    in vec3 NormalFragColor;  // Input: Normal color
    flat in int	RenderTexture; // Input: Which texture to use (0 for no texture, 1 for texture)
    flat in int GUITransparency; // Input: Whether to use transparency or not

	uniform sampler2D textureSampler; // Texture sampler

    out vec4 FinalColor;  // Output: Final fragment color

    void main() {
		// Sample the texture using the texture coordinates
        vec4 texColor = texture(textureSampler, TextureFragColor);

        // Output the color directly
        if (RenderTexture == 0)
			FinalColor = vec4(NormalFragColor, 1.0f); // for no texture
		else
        	FinalColor = texColor; //for texture

        // If transparency is enabled, use the alpha value from the texture
        if (GUITransparency == 1)
            FinalColor.a = 0.3f;
    }
