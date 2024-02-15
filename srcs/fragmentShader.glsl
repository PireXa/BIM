    #version 330 core

    in vec2 FragColor;  // Input: Vertex color (vec3 if using classic rainbow effect)

	uniform sampler2D textureSampler;  // Texture sampler

    out vec4 FinalColor;  // Output: Final fragment color

    void main() {
		// Sample the texture using the texture coordinates
        vec4 texColor = texture(textureSampler, FragColor);

        // Output the color directly
        FinalColor = texColor;
    }
