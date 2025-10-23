#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D tex;

in vec3 Normal;
in vec3 FragPos;
uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 norm = normalize(Normal);

	vec3 ambient = 0.1 * lightColor;

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 reflectedDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = pow(max(dot(reflectedDir, viewDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	float distance   = length(lightPos - FragPos);
	float constant = 1.0;
	float linear    = 0.045;
	float quadratic = 0.0075;
	float attenuation = 1.0 / (constant + linear * distance + 
    		    quadratic * (distance * distance));   

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 lighting = ambient +  diffuse + specular;

	vec3 result = lighting * texture(tex, TexCoords).rgb;
	FragColor = vec4(result, 1.0);
}