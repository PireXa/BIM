    #version 330 core

    uniform mat4 planeMatrix;  // Uniform model matrix

    layout (location = 0) in vec3 position;  // Input: Vertex position
    layout (location = 1) in vec3 uv;     // Input: Vertex color
    layout (location = 2) in vec3 normal;  // Input: Vertex normal

    out vec2 TextureFragColor;  // Output: TextCoord for fragment shader (vec3 for no texture, vec2 for texture)

    void main() {

        // Transform vertex position to camera space
        gl_Position = planeMatrix * vec4(position, 1.0);

        // Pass color to the fragment shader
        TextureFragColor = uv.xy; // for texture
    }