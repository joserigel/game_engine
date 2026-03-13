#ifndef __SHADER_HPP__
#define __SHADER_HPP__

class Shader {
    private:
        unsigned int _program;
    public:
        void use();
        unsigned int uniformLocation(const char* name);
        Shader(
            const char* vertexPath,
            const char* fragmentPath
            );
};

#endif
