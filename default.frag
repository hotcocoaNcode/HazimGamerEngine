#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 curPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;



// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;


uniform vec4 lightColor1;
uniform vec3 lightPos1;
uniform int lightType1;

uniform vec4 lightColor2;
uniform vec3 lightPos2;
uniform int lightType2;

uniform vec4 lightColor3;
uniform vec3 lightPos3;
uniform int lightType3;

uniform int totalLights;

// Gets the position of the camera from the main function
uniform vec3 camPos;


vec4 pointLight(vec3 lightPos, vec4 lightColor)
{	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - curPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - curPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten * lightColor;
}

vec4 direcLight(vec4 lightColor)
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - curPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (diffuse + ambient) + texture(specular0, texCoord).r * specular * lightColor;
}

vec4 spotLight(vec3 lightPos, vec4 lightColor)
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - curPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - curPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the curPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten * lightColor;
}



void main()
{
   vec4 lightTotal = vec4(0.0f, 0.0f, 0.0f, 0.0f); // janky ass fix but should be fine
   if (totalLights >= 1){
	   if (lightType1 == 1){
		lightTotal += pointLight(lightPos1, lightColor1);
	   }
	   if (lightType1 == 2){
		lightTotal += direcLight(lightColor1);
	   }
	   if (lightType1 == 3){
		lightTotal += spotLight(lightPos1, lightColor1);
	   }
   }
   if (totalLights >= 2){
	   if (lightType2 == 1){
		lightTotal += pointLight(lightPos2, lightColor2);
	   }
	   if (lightType2 == 2){
		lightTotal += direcLight(lightColor2);
	   }
	   if (lightType2 == 3){
		lightTotal += spotLight(lightPos2, lightColor2);
	   }
   }
   if (totalLights >= 3){
	   if (lightType3 == 1){
		lightTotal += pointLight(lightPos3, lightColor3);
	   }
	   if (lightType3 == 2){
		lightTotal += direcLight(lightColor3);
	   }
	   if (lightType3 == 3){
		lightTotal += spotLight(lightPos3, lightColor3);
	   }
   }

   FragColor = texture(diffuse0, texCoord) * lightTotal;
}