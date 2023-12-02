#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoord;

out vec4 FragColor; 

uniform vec3 lightPos; 
uniform vec3 lightColor; 
uniform vec3 objectColor; 
uniform vec3 viewPos; 
uniform sampler2D outTexture; //--- 텍스처 샘플러

void main()
{
	vec3 ambientLight = vec3(0.7f, 0.7f, 0.7f); 
	vec3 ambient = ambientLight * lightColor;

	float d = distance(FragPos, lightPos);

	vec3 normalVector = normalize (Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);
	vec3 diffuse = diffuseLight * lightColor; 

	int shininess = 128; 
	vec3 viewDir = normalize(viewPos - FragPos); 
	vec3 reflectDir = reflect(-lightDir, normalVector); 
	float specularLight = max(dot(viewDir, reflectDir), 0.0); 
	specularLight = pow(specularLight, shininess); 
	vec3 specular = specularLight * lightColor / d; 

	vec3 result = (ambient + diffuse + specular) * objectColor; 

	FragColor = vec4 (result, 1.0); 
	FragColor = texture(outTexture, TextCoord) * FragColor;
}