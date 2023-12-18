#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "include/glm/glm/glm.hpp"
#include "include/glm/glm/ext.hpp"

using namespace glm;
using namespace std;

#define MAX_DEPTH 20
#define EPSILON 0.0001f

class Sphere {
    public:
        string name;
        vec4 position;
        vec3 scale;
        vec3 color;
        float ambient;
        float diffuse;
        float specular;
        float reflectivity;
        int specularExp;
};

class Light {
    public:
        string name;
        vec4 position;
        vec3 intensity;
        
};

class Ray {
    public:
        vec4 position;
        vec4 direction;
        int depth;

        Ray(vec4 pos, vec4 dir, int d) {
            position = pos;
            direction = dir;
            depth = d;
        }
};

class Collision {
    public:
        bool collided;
        vec4 position;
        vec4 normal;
        float time;
};

class Input {
    public:
        string prefix;
        float near, left, right, bottom, top;
        int resX, resY;
        vector<Sphere> sphereList;
        vector<Light> lightList;
        vec3 backgroundColor;
        vec3 ambient;
        string outputName;
};

void save_imageP3(int Width, int Height, char* fname,unsigned char* pixels) {
	FILE *fp;
	const int maxVal=255;
	
	printf("Saving image %s: %d x %d\n", fname,Width,Height);
	fp = fopen(fname,"w");
	if (!fp) {
		printf("Unable to open file '%s'\n",fname);
		return;
	}
	fprintf(fp, "P3\n");
	fprintf(fp, "%d %d\n", Width, Height);
	fprintf(fp, "%d\n", maxVal);
	
	int k = 0 ;
	for(int j = 0; j < Height; j++) {
		
		for( int i = 0 ; i < Width; i++)
		{
			fprintf(fp," %d %d %d", pixels[k],pixels[k+1],pixels[k+2]) ;
			k = k + 3 ;
		}
		fprintf(fp,"\n") ;
	}
	fclose(fp);
}

Input readInputFile(char* fileName) {
    ifstream infile(fileName);
    if (!infile) {
        cout << "Error: Bad input file." << endl;
        exit(-1);
    }

    Input input;
    string prefix;

    infile >> prefix;
    while (!infile.eof()) {
        if (prefix == "NEAR") {
            infile >> input.near;
        } else if (prefix == "LEFT") {
            infile >> input.left;
        } else if (prefix == "RIGHT") {
            infile >> input.right;
        } else if (prefix == "BOTTOM") {
            infile >> input.bottom;
        } else if (prefix == "TOP") {
            infile >> input.top;
        } else if (prefix == "RES") {
            infile >> input.resX >> input.resY;
        } else if (prefix == "AMBIENT") {
            infile >> input.ambient.r >> input.ambient.g >> input.ambient.b;
        } else if (prefix == "BACK") {
            infile >> input.backgroundColor.r >> input.backgroundColor.g >> input.backgroundColor.b;
        } else if (prefix == "SPHERE") {
            Sphere s;
            infile >> s.name >> s.position.x >> s.position.y >> s.position.z;
            infile >> s.scale.x >> s.scale.y >> s.scale.z;
            infile >> s.color.r >> s.color.g >> s.color.b;
            infile >> s.ambient >> s.diffuse >> s.specular >> s.reflectivity >> s.specularExp;
            s.position.w = 1;
            input.sphereList.push_back(s);
        } else if (prefix == "LIGHT") {
            Light l;
            infile >> l.name >> l.position.x >> l.position.y >> l.position.z;
            infile >> l.intensity.r >> l.intensity.g >> l.intensity.b;
            l.position.w = 1;
            input.lightList.push_back(l);
        } else if (prefix == "OUTPUT") {
            infile >> input.outputName;
        }
        infile >> prefix;
    }

    cout << "Read input file successfully." << endl;

    return input;
}

vec4 coordsAtPixel(vec2 pixel, Input input) {
    if (pixel.x >= input.resX || pixel.y >= input.resY) {
        cout << "Bad input to function: coordsAtPixel()" << endl;
        exit(-1);
    }

    int viewportWidth = input.right - input.left;
    int viewportHeight = input.top - input.bottom;
    float xPos = input.left + (pixel.x+0.5) / input.resX * viewportWidth;
    float yPos = input.bottom + (pixel.y+0.5) / input.resY * viewportHeight;

    vec4 pixelCoords = vec4(xPos, yPos, -input.near, 1.0);
    return pixelCoords;
}

Collision determineCollision(Ray r, Sphere s) {

    mat4 modelMatrix = mat4(1.0);
    modelMatrix = translate(mat4(1.0), vec3(s.position)) * scale(mat4(1.0), s.scale);
    mat4 invModelMatrix = inverse(modelMatrix);
    mat4 invTransModelMatrix = inverseTranspose(modelMatrix);

    Ray sphereSpaceRay = Ray(invModelMatrix * r.position, invModelMatrix * r.direction, r.depth);

    vec3 S = vec3(sphereSpaceRay.position); //ray origin
    vec3 c = vec3(sphereSpaceRay.direction); //ray direction

    float discriminant = pow(dot(S, c), 2) - dot(c, c) * (dot(S, S) - 1);

    Collision col;
    if (discriminant <= 0) {
        col.collided = false;
        return col;
    }

    float t;
    float t1 = -dot(S, c)/dot(c, c) - sqrt(discriminant) / dot(c, c);
    float t2 = -dot(S, c)/dot(c, c) + sqrt(discriminant) / dot(c, c);
    if (t1 < 0 && t2 < 0) {
        col.collided = false;
        return col;
    }
    col.collided = true;
    if (t1 < 0) {
        t = t2;
    } else if (t2 < 0) {
        t = t1;
    }
    else {
        t = t1 < t2 ? t1 : t2; //should always be t1, just in case...
    }

    col.position = r.position + t*r.direction;
    vec4 sphereSpaceCollisionPos = sphereSpaceRay.position + t*sphereSpaceRay.direction;

    col.normal = invTransModelMatrix * (sphereSpaceCollisionPos - vec4(0, 0, 0, 1));
    col.normal.w = 0;

    //checks for internal collision
    col.normal = t1 < 0 ? -normalize(col.normal) : normalize(col.normal);
    col.time = t;
    return col;
}   

vec3 raytrace(Ray r, Input input) {
    if (r.depth > MAX_DEPTH) {
        return vec3(0, 0, 0);
    }

    Collision closestCol;
    Sphere collidedSphere;
    closestCol.time = std::numeric_limits<float>::max();
    for (Sphere s : input.sphereList) {
        Collision c = determineCollision(r, s);
        if (c.collided) {
            if (c.time < closestCol.time) {
                closestCol = c;
                collidedSphere = s;
            }
        }
    }

    if (closestCol.time == std::numeric_limits<float>::max()) {
        return r.depth == 0 ? input.backgroundColor : vec3(0, 0, 0);
    }

    vec4 reflected = normalize(r.direction - 2*dot(r.direction, closestCol.normal)*closestCol.normal);

    Ray reflectedRay = Ray(closestCol.position + EPSILON*reflected, reflected, r.depth+1);

    vec3 N = normalize(vec3(closestCol.normal)); //normal vector
    vec3 V = normalize(vec3(-r.direction)); //view vector
    vec3 O = collidedSphere.color; //object color
    vec3 Ia = input.ambient; //ambient color
    float Kd = collidedSphere.diffuse; //object diffuse val
    float Ks = collidedSphere.specular; //object specular val
    float Ka = collidedSphere.ambient; //object ambient val
    float n = collidedSphere.specularExp; //object specular exponent

    vec3 localIllumination = vec3(0, 0, 0);
    for (Light l : input.lightList) {
        Ray sr = Ray(closestCol.position + EPSILON*closestCol.normal, normalize(l.position - closestCol.position), 0);
        float distToLight = length(l.position - closestCol.position);
        bool blocked = false;
        for (Sphere &s : input.sphereList) {
            Collision c = determineCollision(sr, s);
            if (c.collided && c.time <= distToLight) {
                blocked = true;
                break;
            }
        }

        if (!blocked) {
            vec3 L = normalize(vec3(sr.direction));
            vec3 R = normalize(-L - 2*dot(-L, N)*N);    
            vec3 Ip = l.intensity;

            //add diffuse
            float dnl = dot(N, L);
            localIllumination += dnl >= 0 ? Kd * Ip * dnl * O : vec3(0, 0, 0);

            //add specular
            float drv = dot(R, V);
            localIllumination += drv >= 0 ? Ks * Ip * pow(dot(R, V), n) : vec3(0, 0, 0);
        }
    }

    //add ambient
    localIllumination += Ka * Ia * O;

    vec3 returnColor = collidedSphere.reflectivity <= EPSILON ?
        localIllumination :
        localIllumination + collidedSphere.reflectivity*raytrace(reflectedRay, input);

    returnColor.r = returnColor.r >= 1.0 ? 1.0 : returnColor.r;
    returnColor.g = returnColor.g >= 1.0 ? 1.0 : returnColor.g;
    returnColor.b = returnColor.b >= 1.0 ? 1.0 : returnColor.b;

    return returnColor;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Error: Usage RayTracer.exe <inputfile.txt>" << endl;
        exit(-1);
    }

    Input input = readInputFile(argv[1]);
    unsigned char* pixels = new unsigned char [3*input.resX*input.resY];
    int k = 0;

    for (int row = input.resY - 1; row >= 0; row--) {
        for (int col = 0; col < input.resX; col++) {
            vec4 pixelCoords = coordsAtPixel(vec2(col, row), input);
            Ray r = Ray(pixelCoords, normalize(pixelCoords - vec4(0, 0, 0, 1)), 0);
            
            vec3 color = raytrace(r, input);    
            pixels[k] = round(color.r * 255);
            pixels[k+1] = round(color.g * 255);
            pixels[k+2] = round(color.b * 255);
            k += 3;
        }
    }
    save_imageP3(input.resX, input.resY, &input.outputName[0], pixels);
}