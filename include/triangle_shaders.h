// Shaders always begin with a version declaration followed by a list of input and output varibles, uniforms and then the main function (entry point)
// Each input variable is known as the vertex attribute
// The amount of vertex attributes allowed is hardware depended
// GLSL allows swizzling:
// vec4 differentVec = someVec.xyxx;


const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"}";

const char *fragmentShaderSourceRed = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f,0.0f,0.0f,1.0f);\n"
"}\n";

const char *fragmentShaderSourceYellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.0f,1.0f,1.0f,1.0f);\n"
"}\n";

// To show how passing data works from shader to shader, lets let the vertex shader decide the color in the fragment shader

const char *vertexCShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos,1.0);\n"
"	vertexColor = vec4(0.5,0.0,0.0,1.0);\n"
"}\n";

const char *fragmentShaderNCSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(vertexColor);\n"
"}\n";

// Uniforms are another way to pass data to shaders
// They are global and can be accessed by any shader
// Uniforms keep their value until reset or updated
// Lets use a uniform to set the color in the fragment shader as an example
// Note: the uniform is empty here. We have to set it to something in the cpu program

const char *fragmentUniformShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 Color;\n"
"void main ()\n"
"{\n"
"	FragColor = vec4(Color);\n"
"}\n";
