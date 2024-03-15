    #version 330 core

    uniform int GUITransparent;
    uniform mat4 textMatrix;

    layout (location = 0) in vec3 position;  // Input: Vertex position
    layout (location = 1) in vec3 uv;     // Input: Vertex color

    out vec2 TextureFragColor;  // Output: TextCoord for fragment shader (vec3 for no texture, vec2 for texture)
    flat out int GUITransparency; // Output: Is the GUI transparent

    void main() {

        // Transform vertex position to camera space
        gl_Position = textMatrix * vec4(position, 1.0);
        GUITransparency = GUITransparent;

        // Pass color to the fragment shader
        TextureFragColor = uv.xy; // for texture
    }