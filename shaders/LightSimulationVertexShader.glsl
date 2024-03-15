    #version 330 core

    uniform mat4 mvp;  // Uniform view-projection matrix
    uniform mat4 model;  // Uniform model matrix
    uniform mat4 normalMatrix;  // Uniform normal matrix
    uniform	float transitionBlendFactor;

    layout (location = 0) in vec3 position;  // Input: Vertex position
    layout (location = 1) in vec3 uv;     // Input: Vertex color
    layout (location = 2) in vec3 normal;  // Input: Vertex normal

    out vec2 TextureFragColor;  // Output: TextCoord for fragment shader (vec3 for no texture, vec2 for texture)
    out vec3 NormalFragColor;  // Output: Normal for fragment shader
    out float diffuse;  // Output: Diffuse for fragment shader
    flat out float TransitionBlendFactor; // Output: Transition blend factor

    void main() {
        vec3 lightPos = vec3(0.0, 1000.0, 1000.0);
        vec3 Position = vec3(model * vec4(position, 1.0));
        vec3 lightDir = normalize(lightPos - Position);
        vec3 directionalNormal = normal * 2.0 - 1.0;
        diffuse = max(dot(mat3(model) * directionalNormal, lightDir), 0.0) * 0.9 + 0.1;

        // Transform vertex position to camera space
        gl_Position = mvp * vec4(position, 1.0);

        // Pass color to the fragment shader
        TextureFragColor = uv.xy; // for texture
        NormalFragColor = normal;
        TransitionBlendFactor = transitionBlendFactor;
    }