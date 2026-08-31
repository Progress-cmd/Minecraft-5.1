#include"shaderClass.h"

// lit les fichiers shaders, et les retournent sous forme de chaine de caractère
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// constructeur de la classe shader, avec les deux shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// lit vertexFile et fragmentFile et les range dans une chaine de caractère
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// converti les chaines de caractère des shaders en tableau de caractère
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// création du shader afin d'obtenir sa valeur par référence
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attache le code source au shader
	glShaderSource(vertexShader, 1, &vertexSource, NULL); // (valeur de référence, nombre de chaine de caractère, source du shader, NULL)
	// compile le shader pour qu'il soit utilisable par la carte graphique
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// création du shader afin d'obtenir sa valeur par référence
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// attache le code source au shader
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL); // (valeur de référence, nombre de chaine de caractère, source du shader, NULL)
	// compile le shader pour qu'il soit utilisable par la carte graphique
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// création d'un programme pour encapsuler les shaders dedans
	ID = glCreateProgram();
	// attache le Vertex et Fragment Shaders au Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// encapsulation du shaderProgram avec les shaders dedans
	glLinkProgram(ID);

	// supprime les objets Vertex et Fragment Shader, car il ne sont plus utilisé tel quel
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// activation du shaderProgram
void Shader::Activate()
{
	glUseProgram(ID);
}

// destruction de l'objet shader
void Shader::Delete()
{
	glDeleteProgram(ID);
}

// vérifie si les différents shader on été compilé correctement
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// enregistre l'état de la compilation
	GLint hasCompiled;
	// tableau de caractère pour enregistrer les messages d'erreurs
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}