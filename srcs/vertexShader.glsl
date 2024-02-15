	#version 330 core

//	uniform mat4 view;  // Uniform view matrix
//	uniform mat4 model;  // Uniform model matrix
//	uniform mat4 projection;  // Uniform projection matrix
    uniform mat4 vp;  // Uniform view-projection matrix

	layout (location = 0) in vec3 position;  // Input: Vertex position
	layout (location = 1) in vec3 uv;     // Input: Vertex color

	out vec2 FragColor;  // Output: TextCoord for fragment shader

	void main() {
	    // Transform vertex position to camera space
		gl_Position = vp * vec4(position, 1.0);

		vec2 texCoord = uv.xy;
	    // Pass color to the fragment shader
	    FragColor = texCoord;
		//FragColor = uv; // Pass rgb color to the fragment shader (for classic rainbow effect)
	}
