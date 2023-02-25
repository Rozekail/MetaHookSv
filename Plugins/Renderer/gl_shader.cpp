#include "gl_local.h"
#include "gl_shader.h"
#include <string>
#include <sstream>
#include <regex>

std::vector<glshader_t> g_ShaderTable;

void GL_InitShaders(void)
{
	
}

void GL_FreeShaders(void)
{
	for(size_t i = 0;i < g_ShaderTable.size(); ++i)
	{
		auto &objs = g_ShaderTable[i].shader_objects;
		for (size_t j = 0; j < objs.size(); ++j)
		{
			glDetachShader(g_ShaderTable[i].program, objs[j]);
			glDeleteShader(objs[j]);
		}
		glDeleteProgram(g_ShaderTable[i].program);
	}

	g_ShaderTable.clear();
}

void GL_CheckShaderError(GLuint shader, const char *code, const char *filename)
{
	int iStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &iStatus); 

	if(GL_FALSE == iStatus)
	{
		int nInfoLength = 0;
		glGetObjectParameterivARB(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &nInfoLength);
		
		std::string info;
		info.resize(nInfoLength);

		glGetInfoLogARB(shader, nInfoLength, NULL, (char *)info.c_str());

		g_pMetaHookAPI->SysError("Shader %s compiled with error:\n%s", filename, info.c_str());
		return;
	}
}

GLuint R_CompileShaderObject(int type, const char *code, const char *filename)
{
	//gEngfuncs.Con_DPrintf("glCreateShader...");

	if (developer->value >= 255)
	{
		g_pFileSystem->CreateDirHierarchy("logs");
		g_pFileSystem->CreateDirHierarchy("logs/renderer");
		g_pFileSystem->CreateDirHierarchy("logs/renderer/shader");
		
		char filepath[256];
		snprintf(filepath, 255, "logs\\%s", filename);
		filepath[255] = 0;

		//gEngfuncs.Con_DPrintf("writing %s...", filepath);

		auto FileHandle = g_pFileSystem->Open(filepath, "wb");
		if (FileHandle)
		{
			g_pFileSystem->Write(code, strlen(code), FileHandle);
			g_pFileSystem->Close(FileHandle);
		}
	}

	auto obj = glCreateShader(type);

	//gEngfuncs.Con_DPrintf("glShaderSource...");

	glShaderSource(obj, 1, &code, NULL);

	//gEngfuncs.Con_DPrintf("glCompileShader...");

	glCompileShader(obj);

	GL_CheckShaderError(obj, code, filename);

	return obj;
}

GLuint R_CompileShader(const char *vscode, const char *fscode, const char *vsfile, const char *fsfile, ExtraShaderStageCallback callback)
{
	GLuint shader_objects[32];
	int shader_object_used = 0;

	//gEngfuncs.Con_DPrintf("R_CompileShaderObject GL_VERTEX_SHADER...");

	shader_objects[shader_object_used] = R_CompileShaderObject(GL_VERTEX_SHADER, vscode, vsfile);
	shader_object_used++;

	if(callback)
		callback(shader_objects, &shader_object_used);

	//gEngfuncs.Con_DPrintf("R_CompileShaderObject GL_FRAGMENT_SHADER...");

	shader_objects[shader_object_used] = R_CompileShaderObject(GL_FRAGMENT_SHADER, fscode, fsfile);
	shader_object_used++;

	//gEngfuncs.Con_DPrintf("Linking program...");

	GLuint program = glCreateProgram();
	for(int i = 0;i < shader_object_used; ++i)
		glAttachShader(program, shader_objects[i]);
	glLinkProgram(program);

	int iStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &iStatus);
	if (GL_FALSE == iStatus)
	{
		int nInfoLength;
		char szCompilerLog[1024] = { 0 };
		glGetProgramInfoLog(program, sizeof(szCompilerLog), &nInfoLength, szCompilerLog);

		g_pMetaHookAPI->SysError("Shader linked with error:\n%s", szCompilerLog);
	}

	g_ShaderTable.emplace_back(program, shader_objects, shader_object_used);

	return program;
}

void R_CompileShaderAppendInclude(std::string &str, const char *filename)
{
	std::regex pattern("#include[< \"]+([a-zA-Z_\\.]+)[> \"]");
	std::smatch result;
	std::regex_search(str, result, pattern);

	std::string skipped;

	std::string::const_iterator searchStart(str.cbegin());

	while (std::regex_search(searchStart, str.cend(), result, pattern) && result.size() >= 2)
	{
		std::string prefix = result.prefix();
		std::string suffix = result.suffix();

		auto includeFileName = result[1].str();

		char slash;

		std::string includePath = filename;
		for (size_t j = includePath.length() - 1; j > 0; --j)
		{
			if (includePath[j] == '\\' || includePath[j] == '/')
			{
				slash = includePath[j];
				includePath.resize(j);
				break;
			}
		}

		includePath += slash;
		includePath += includeFileName;

		auto pFile = gEngfuncs.COM_LoadFile((char *)includePath.c_str(), 5, NULL);
		if (pFile)
		{
			std::string wbinding((char *)pFile);

			gEngfuncs.COM_FreeFile(pFile);

			if (searchStart != str.cbegin())
			{
				str = skipped + prefix;
			}
			else
			{
				str = prefix;
			}
			str += wbinding;

			auto currentLength = str.length();

			str += suffix;

			skipped = str.substr(0, currentLength);
			searchStart = str.cbegin() + currentLength;
			continue;
		}

		searchStart = result.suffix().first;
	}
}

void R_CompileShaderAppendDefine(std::string &str, const std::string &def)
{
	std::regex pattern("(#version [0-9a-z ]+)");
	std::smatch result;
	std::regex_search(str, result, pattern);

	if (result.size() >= 1)
	{
		std::string prefix = result[0];
		std::string suffix = result.suffix();

		str = prefix;
		str += "\n\n";
		str += def;
		str += "\n\n";
		str += suffix;
	}
	else
	{
		std::string suffix = str;

		str = def;
		str += "\n\n";
		str += suffix;
	}
}

GLuint R_CompileShaderFileEx(
	const char *vsfile, const char *fsfile, 
	const char *vsdefine, const char *fsdefine,
	ExtraShaderStageCallback callback)
{
	auto vscode = (char *)gEngfuncs.COM_LoadFile((char *)vsfile, 5, 0);
	if (!vscode)
	{
		g_pMetaHookAPI->SysError("R_CompileShaderFileEx: %s file not found!", vsfile);
	}

	gEngfuncs.Con_DPrintf("R_CompileShaderFileEx: compiling %s...\n", vsfile);

	std::string vs(vscode);

	R_CompileShaderAppendDefine(vs, "#define IS_VERTEX_SHADER\n");
	if (vsdefine)
	{
		R_CompileShaderAppendDefine(vs, vsdefine);
	}

	gEngfuncs.COM_FreeFile(vscode);

	auto fscode = (char *)gEngfuncs.COM_LoadFile((char *)fsfile, 5, 0);
	if (!fscode)
	{
		g_pMetaHookAPI->SysError("R_CompileShaderFileEx: %s file not found!", fsfile);
	}

	gEngfuncs.Con_DPrintf("R_CompileShaderFileEx: compiling %s...\n", fsfile);

	std::string fs(fscode);

	R_CompileShaderAppendDefine(fs, "#define IS_FRAGMENT_SHADER\n");
	if (fsdefine)
	{
		R_CompileShaderAppendDefine(fs, fsdefine);
	}

	gEngfuncs.COM_FreeFile(fscode);

	if (vs.find("#include") != std::string::npos)
	{
		R_CompileShaderAppendInclude(vs, vsfile);
	}

	if (fs.find("#include") != std::string::npos)
	{
		R_CompileShaderAppendInclude(fs, fsfile);
	}

	return R_CompileShader(vs.c_str(), fs.c_str(), vsfile, fsfile, callback);
}

GLuint R_CompileShaderFile(const char *vsfile, const char *fsfile, ExtraShaderStageCallback callback)
{
	return R_CompileShaderFileEx(vsfile, fsfile, NULL, NULL, callback);
}

void GL_UseProgram(GLuint program)
{
	static int currentprogram = -1;

	if (currentprogram != program)
	{
		currentprogram = program;
		glUseProgramObjectARB(program);
	}
}

GLuint GL_GetUniformLoc(GLuint program, const char *name)
{
	return glGetUniformLocationARB(program, name);
}

GLuint GL_GetAttribLoc(GLuint program, const char *name)
{
	return glGetAttribLocationARB(program, name);
}

void GL_Uniform1i(GLuint loc, int v0)
{
	glUniform1i(loc, v0);
}

void GL_Uniform2i(GLuint loc, int v0, int v1)
{
	glUniform2iARB(loc, v0, v1);
}

void GL_Uniform3i(GLuint loc, int v0, int v1, int v2)
{
	glUniform3iARB(loc, v0, v1, v2);
}

void GL_Uniform4i(GLuint loc, int v0, int v1, int v2, int v3)
{
	glUniform4iARB(loc, v0, v1, v2, v3);
}

void GL_Uniform1f(GLuint loc, float v0)
{
	glUniform1f(loc, v0);
}

void GL_Uniform2f(GLuint loc, float v0, float v1)
{
	glUniform2f(loc, v0, v1);
}

void GL_Uniform3f(GLuint loc, float v0, float v1, float v2)
{
	glUniform3f(loc, v0, v1, v2);
}

void GL_Uniform4f(GLuint loc, float v0, int v1, int v2, int v3)
{
	glUniform4f(loc, v0, v1, v2, v3);
}

void GL_VertexAttrib3f(GLuint index, float x, float y, float z)
{
	glVertexAttrib3f(index, x, y, z);
}

void GL_VertexAttrib3fv(GLuint index, float *v)
{
	glVertexAttrib3fv(index, v);
}

void GL_MultiTexCoord2f(GLenum target, float s, float t)
{
	glMultiTexCoord2fARB(target, s, t);
}

void GL_MultiTexCoord3f(GLenum target, float s, float t, float r)
{
	glMultiTexCoord3fARB(target, s, t, r);
}

void R_SaveProgramStatesCaches(const char *filename, const std::vector<program_state_t> &ProgramStates, const program_state_mapping_t *mapping, size_t mapping_size)
{
	std::stringstream ss;

	for (auto &p : ProgramStates)
	{
		if (p == 0)
		{
			ss << "NONE";
		}
		else
		{
			for (int i = 0; i < mapping_size; ++i)
			{
				if (p & mapping[i].state)
				{
					ss << mapping[i].name << " ";
				}
			}
		}
		ss << "\n";
	}

	auto FileHandle = g_pFileSystem->Open(filename, "wt");
	if (FileHandle)
	{
		auto str = ss.str();
		g_pFileSystem->Write(str.data(), str.length(), FileHandle);
		g_pFileSystem->Close(FileHandle);
	}
}

void R_LoadProgramStateCaches(const char *filename, const program_state_mapping_t *mapping, size_t mapping_size, void(*callback)(program_state_t state))
{
	auto FileHandle = g_pFileSystem->Open("renderer/shader/studio_cache.txt", "rt");
	if (FileHandle)
	{
		char szReadLine[4096];
		while (!g_pFileSystem->EndOfFile(FileHandle))
		{
			g_pFileSystem->ReadLine(szReadLine, sizeof(szReadLine) - 1, FileHandle);
			szReadLine[sizeof(szReadLine) - 1] = 0;

			program_state_t ProgramState = 0;
			bool filled = false;
			bool quoted = false;
			char token[256];
			char *p = szReadLine;
			while (1)
			{
				p = g_pFileSystem->ParseFile(p, token, &quoted);
				if (token[0])
				{
					if (!strcmp(token, "NONE"))
					{
						ProgramState = 0;
						filled = true;
						break;
					}
					else
					{
						for (int i = 0; i < mapping_size; ++i)
						{
							if (!strcmp(token, mapping[i].name))
							{
								ProgramState |= mapping[i].state;
								filled = true;
							}
						}
					}
				}
				else
				{
					break;
				}

				if (!p)
					break;
			}

			if (filled)
			{
				callback(ProgramState);
			}
		}
		g_pFileSystem->Close(FileHandle);
	}
}