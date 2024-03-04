	#version 330 core

//	uniform mat4 view;  // Uniform view matrix
//	uniform mat4 model;  // Uniform model matrix
//	uniform mat4 projection;  // Uniform projection matrix
    uniform mat4 vp;  // Uniform view-projection matrix
    uniform int renderTexture;
	uniform int GUITransparent;

	layout (location = 0) in vec3 position;  // Input: Vertex position
	layout (location = 1) in vec3 uv;     // Input: Vertex color
	layout (location = 2) in vec3 normal;  // Input: Vertex normal

	out vec2 TextureFragColor;  // Output: TextCoord for fragment shader (vec3 for no texture, vec2 for texture)
    out vec3 NormalFragColor;  // Output: Normal for fragment shader
    flat out int RenderTexture; // Output: Which render mode to use
	flat out int GUITransparency; // Output: Is the GUI transparent

	void main() {
	    // Transform vertex position to camera space
		gl_Position = vp * vec4(position, 1.0);
		GUITransparency = GUITransparent;

		vec2 texCoord = uv.xy;
	    // Pass color to the fragment shader
	    TextureFragColor = texCoord; // for texture
		NormalFragColor = normal; // Pass rgb color to the fragment shader (for no texture)
		RenderTexture = renderTexture;
	}
