//
//  mesh.cpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "mesh.hpp"

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <moditone/jsonata/json.hpp>

#include <iostream>

const std::string VERTEX_SHADER = R"(
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

void main()
{
    mat4 viewModelMatrix = viewMatrix * modelMatrix;
	gl_Position = projectionMatrix * viewModelMatrix * vec4(position, 1.0);
    fragPosition = vec3(viewModelMatrix * vec4(position, 1.0));
    fragNormal = normal;
}

)";

const std::string FRAGMENT_SHADER = R"(
#version 330

in vec3 fragPosition;
in vec3 fragNormal;

layout(location = 0) out vec4 fragmentColour;

uniform vec3 albedo;
vec3 light = vec3(1, 1, 4);

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(light);
    float I = dot(N, L);
    fragmentColour = vec4(albedo, 1.0) * I;
}

)";

StaticMeshUpdater::StaticMeshUpdater(World& w): System(w)
{
    
}

void StaticMeshUpdater::operator()(World& w, double dt)
{ 

}

void StaticMeshUpdater::updateMesh(World::Entity e) {
	scheduleStateUpdate([&, e](World& w, double dt) {
		
		if(!w.has<StaticMesh>(e))
		{
			w.attach<StaticMesh>(e);
			createMeshForEntity(w, e);
		}
		
		updateMeshFromFilter(w, e);
	});
}

void StaticMeshUpdater::createMeshForEntity(World& w, World::Entity e)
{
	w.attach<StaticMesh>(e);
	auto& m = w.getAll<StaticMesh>()[e];
    
    m->primitiveType = GL_TRIANGLES;
    uint32_t error = 0;
    assert( (error = glGetError()) == GL_NO_ERROR);
    
	glGenVertexArrays(1, &m->vao);
    assert(m->vao != 0);
    assert( (error = glGetError()) == GL_NO_ERROR);
	glGenBuffers(1, &m->vbo);
    assert(m->vbo != 0);
    assert( (error = glGetError()) == GL_NO_ERROR);
	glGenBuffers(1, &m->ibo);
    assert(m->ibo != 0);
    assert( (error = glGetError()) == GL_NO_ERROR);
	
	glBindVertexArray(m->vao);
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);
	
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, false,
						  sizeof(Vertex), (void*)offsetof(Vertex, position));
	
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, false,
						  sizeof(Vertex), (void*)offsetof(Vertex, normal));
	
	glEnableVertexAttribArray(2);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, false,
						  sizeof(Vertex), (void*)offsetof(Vertex, uv));
	
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    assert( (error = glGetError()) == GL_NO_ERROR);
	
	m->material.vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m->material.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	m->material.program = glCreateProgram();
	
	const char* s = (const GLchar*)VERTEX_SHADER.c_str();
	glShaderSource(m->material.vertexShader, 1, &s, 0);
	glCompileShader(m->material.vertexShader);
	
	GLint success = 0;
	glGetShaderiv(m->material.vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		GLint logSize = 0;
		glGetShaderiv(m->material.vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetShaderInfoLog(m->material.program, logSize, &logSize, log.data());
		for(const auto c: log)
			std::cout << c;
		
		std::cout << '\n';
	}
	
	const char* fs = FRAGMENT_SHADER.c_str();
	glShaderSource(m->material.fragmentShader, 1, &fs, 0);
	glCompileShader(m->material.fragmentShader);
	
	success = 0;
	glGetShaderiv(m->material.fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		GLint logSize = 0;
		glGetShaderiv(m->material.fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetShaderInfoLog(m->material.fragmentShader, logSize, &logSize, log.data());
		for(const auto c: log)
			std::cout << c;
		
		std::cout << '\n';
	}
	
	glAttachShader(m->material.program, m->material.vertexShader);
	glAttachShader(m->material.program, m->material.fragmentShader);
	
	glLinkProgram(m->material.program);
	
	success = 0;
	glGetProgramiv(m->material.program, GL_LINK_STATUS, &success);
	if(!success)
	{
		GLint logSize = 0;
		glGetProgramiv(m->material.program, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetProgramInfoLog(m->material.program, logSize, &logSize, log.data());
		for(const auto c: log)
			std::cout << c;
		
		std::cout << '\n';
	}
    
    assert( (error = glGetError()) == GL_NO_ERROR);
}

void StaticMeshUpdater::updateMeshFromFilter(World& w, World::Entity e)
{
	auto& mesh = w.getAll<StaticMesh>()[e];
    auto filter = &mesh->filter;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, filter->positions.size() * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
	
	Vertex* vertices = static_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE));
    if(!vertices)
        return;
    
	assert(vertices);
	
    auto& pos = filter->positions;
    auto& normal = filter->normals;
    auto& uv = filter->uv;
	
    for(auto i = 0; i < filter->positions.size(); i++)
		vertices[i].position = pos[i];

	for(auto i = 0; i < normal.size(); i++)
		vertices[i].normal = normal[i];
	
	for(auto i = 0; i < uv.size(); i++)
		vertices[i].uv = uv[i];
	
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, filter->faces.size() * sizeof(vec3i), filter->faces.data(), GL_STATIC_DRAW);
    mesh->numFaces = static_cast<uint32_t>(filter->faces.size());
    mesh->numVertices = static_cast<uint32_t>(filter->positions.size());
    mesh->primitiveType = filter->primitiveType;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void StaticMeshUpdater::setPrimitiveType(World::Entity e, uint32_t primitiveType)
{
    if(!world.has<StaticMesh>(e))
    {
        world.attach<StaticMesh>(e);
        createMeshForEntity(world, e);
    }
    
    world.getAll<StaticMesh>()[e]->filter.primitiveType = primitiveType;
}

void StaticMeshUpdater::setPositions(World::Entity e, const vector_wrapper<vec3> &positions)
{
    if(!world.has<StaticMesh>(e))
    {
        world.attach<StaticMesh>(e);
        createMeshForEntity(world, e);
    }
    
    world.getAll<StaticMesh>()[e]->filter.positions = positions;
}

void StaticMeshUpdater::setNormals(World::Entity e, const vector_wrapper<vec3>& normals)
{
    if(!world.has<StaticMesh>(e))
    {
        world.attach<StaticMesh>(e);
        createMeshForEntity(world, e);
    }
    
    world.getAll<StaticMesh>()[e]->filter.normals = normals;
}

void StaticMeshUpdater::setUvs(World::Entity e, const vector_wrapper<vec2>& uvs)
{
    if(!world.has<StaticMesh>(e))
    {
        world.attach<StaticMesh>(e);
        createMeshForEntity(world, e);
    }
    
    world.getAll<StaticMesh>()[e]->filter.uv = uvs;
}

void StaticMeshUpdater::setFaces(World::Entity e, const vector_wrapper<vec3i> &faces)
{
    if(!world.has<StaticMesh>(e))
    {
        world.attach<StaticMesh>(e);
        createMeshForEntity(world, e);
    }
    
    world.getAll<StaticMesh>()[e]->filter.faces = faces;
}

void StaticMeshUpdater::setMaterial(World::Entity e, const char *name)
{
    std::ifstream stream(name);
    const auto var = json::parse(stream);
    
    const auto& vertexShaderString = var["vertex_shader"].asString();
    const auto& fragmentShaderString = var["fragment_shader"].asString();
    
    loadShaders(e, vertexShaderString, fragmentShaderString);
}

void StaticMeshUpdater::loadShaders(World::Entity e, const std::string& vShader, const std::string& fShader)
{
    scheduleStateUpdate([&, e](World& w, double dt) {
        
        auto& m = w.getAll<StaticMesh>()[e];
        m->material.vertexShader = glCreateShader(GL_VERTEX_SHADER);
        m->material.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        m->material.program = glCreateProgram();
        
        const char* s = (const GLchar*)vShader.c_str();
        glShaderSource(m->material.vertexShader, 1, &s, 0);
        glCompileShader(m->material.vertexShader);
        
        GLint success = 0;
        glGetShaderiv(m->material.vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            GLint logSize = 0;
            glGetShaderiv(m->material.vertexShader, GL_INFO_LOG_LENGTH, &logSize);
            std::vector<char> log(logSize);
            glGetShaderInfoLog(m->material.program, logSize, &logSize, log.data());
            for(const auto c: log)
                std::cout << c;
            
            std::cout << '\n';
        }
        
        const char* fs = fShader.c_str();
        glShaderSource(m->material.fragmentShader, 1, &fs, 0);
        glCompileShader(m->material.fragmentShader);
        
        success = 0;
        glGetShaderiv(m->material.fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            GLint logSize = 0;
            glGetShaderiv(m->material.fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
            std::vector<char> log(logSize);
            glGetShaderInfoLog(m->material.fragmentShader, logSize, &logSize, log.data());
            for(const auto c: log)
                std::cout << c;
            
            std::cout << '\n';
        }
        
        glAttachShader(m->material.program, m->material.vertexShader);
        glAttachShader(m->material.program, m->material.fragmentShader);
        
        glLinkProgram(m->material.program);
        
        success = 0;
        glGetProgramiv(m->material.program, GL_LINK_STATUS, &success);
        if(!success)
        {
            GLint logSize = 0;
            glGetProgramiv(m->material.program, GL_INFO_LOG_LENGTH, &logSize);
            std::vector<char> log(logSize);
            glGetProgramInfoLog(m->material.program, logSize, &logSize, log.data());
            for(const auto c: log)
                std::cout << c;
            
            std::cout << '\n';
        }
        
        uint32_t error = 0;
        assert( (error = glGetError()) == GL_NO_ERROR);
    });
}

void StaticMeshUpdater::reflect(chaiscript::ChaiScript &context)
{
    context.add_global(chaiscript::var(this), "meshSystem");
    
    context.add(chaiscript::user_type<MeshFilter>(), "Mesh");
    context.add(chaiscript::constructor<MeshFilter()>(), "Mesh");
    
    using position_list = std::vector<vec3>;
    using normal_list = std::vector<vec3>;
    using uv_list = std::vector<vec2>;
    using face_list = std::vector<vec3i>;
    
    context.add(chaiscript::bootstrap::standard_library::vector_type<position_list>("PositionList"));
    context.add(chaiscript::bootstrap::standard_library::vector_type<normal_list>("NormalList"));
    context.add(chaiscript::bootstrap::standard_library::vector_type<uv_list>("UvList"));
    context.add(chaiscript::bootstrap::standard_library::vector_type<face_list>("FaceList"));
    
    context.add(chaiscript::fun(&MeshFilter::positions), "positions");
    context.add(chaiscript::fun(&MeshFilter::normals), "normals");
    context.add(chaiscript::fun(&MeshFilter::uv), "uv");
    context.add(chaiscript::fun(&MeshFilter::faces), "faces");
    context.add(chaiscript::fun(&MeshFilter::primitiveType), "primitiveType");
    
    context.add(chaiscript::fun(&StaticMeshUpdater::updateMesh), "updateMesh");
    context.add(chaiscript::fun(&StaticMeshUpdater::setMaterialProperty<float>), "setMaterialProperty");
    context.add(chaiscript::fun(&StaticMeshUpdater::setMaterialProperty<vec2>), "setMaterialProperty");
    context.add(chaiscript::fun(&StaticMeshUpdater::setMaterialProperty<vec3>), "setMaterialProperty");
    context.add(chaiscript::fun(&StaticMeshUpdater::setMaterialProperty<vec4>), "setMaterialProperty");
    
    context.add_global(chaiscript::const_var(GL_TRIANGLES), "TRIANGLES");
    context.add_global(chaiscript::const_var(GL_LINES), "LINES");
    context.add_global(chaiscript::const_var(GL_POINTS), "POINTS");
}

void StaticMeshUpdater::reflect(lua_State* context)
{
    using position_list = vector_wrapper<vec3>;
    using normal_list = vector_wrapper<vec3>;
    using uv_list = vector_wrapper<vec2>;
    using face_list = vector_wrapper<vec3i>;
    
    luabridge::getGlobalNamespace(context).
    beginClass<StaticMeshUpdater>("MeshSystem").
    addFunction("setMaterialPropertyFloat", &StaticMeshUpdater::setMaterialProperty<float>).
    addFunction("setMaterialPropertyVec2", &StaticMeshUpdater::setMaterialProperty<vec2>).
    addFunction("setMaterialPropertyVec3", &StaticMeshUpdater::setMaterialProperty<vec3>).
    addFunction("setMaterialPropertyVec4", &StaticMeshUpdater::setMaterialProperty<vec4>).
    addFunction("updateMesh", &StaticMeshUpdater::updateMesh).
    addFunction("setPositions", &StaticMeshUpdater::setPositions).
    addFunction("setNormals", &StaticMeshUpdater::setNormals).
    addFunction("setUvs", &StaticMeshUpdater::setUvs).
    addFunction("setFaces", &StaticMeshUpdater::setFaces).
    addFunction("setPrimitiveType", &StaticMeshUpdater::setPrimitiveType).
    addFunction("buildSphere", &StaticMeshUpdater::buildSphere).
    endClass().
    beginClass<position_list>("PositionList").
    addConstructor<void(*)(void)>().
    addFunction<void (position_list::*) (const vec3&)>("pushBack", &position_list::push_back).
    endClass().
    beginClass<normal_list>("NormalList").
    addConstructor<void(*)(void)>().
    addFunction<void (normal_list::*) (const vec3&)>("pushBack", &normal_list::push_back).
    endClass().
    beginClass<uv_list>("UvList").
    addConstructor<void(*)(void)>().
    addFunction<void (uv_list::*) (const vec2&)>("pushBack", &uv_list::push_back).
    endClass().
    beginClass<face_list>("FaceList").
    addConstructor<void(*)(void)>().
    addFunction<void (face_list::*) (const vec3i&)>("pushBack", &face_list::push_back).
    endClass().
    beginClass<MeshFilter>("MeshFilter").
    addData("positions", &MeshFilter::positions).
    addData("normals", &MeshFilter::normals).
    addData("texCoords", &MeshFilter::uv).
    addData("faces", &MeshFilter::faces).
    endClass();
    
    luabridge::setGlobal(context, this, "meshSystem");
    luabridge::setGlobal(context, GL_POINTS, "POINTS");
    luabridge::setGlobal(context, GL_LINES, "LINES");
    luabridge::setGlobal(context, GL_TRIANGLES, "TRIANGLES");
}

void StaticMeshUpdater::buildSphere(World::Entity e, size_t tessLevel)
{
    if(!world.has<StaticMesh>(e))
    {
        world.attach<StaticMesh>(e);
        createMeshForEntity(world, e);
    }
    
    scheduleStateUpdate([&, e, tessLevel](World& w, double dt) {
        builder.clear();
        builder.buildIcosahedron();
        builder.tesselateAsSphere(tessLevel);
        builder.recalculateNormals();
        auto mesh = &world.getAll<StaticMesh>()[e]->filter;
        mesh->positions.value = builder.vertices;
        mesh->normals.value = builder.normals;
        mesh->faces.value = builder.triangles;
        mesh->primitiveType = GL_TRIANGLES;
        
        updateMesh(e);
    });
}

